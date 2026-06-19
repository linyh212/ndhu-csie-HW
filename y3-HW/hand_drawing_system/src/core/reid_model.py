import torch
import torchvision.transforms as transforms
from torchvision import models
import numpy as np
import cv2

class AppearanceEmbedding:
    def __init__(self, model_path=None):
        # 使用預訓練的 MobileNetV3 (ImageNet) 作為骨幹，不進行額外訓練
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.model = models.mobilenet_v3_small(pretrained=True)
        self.model.classifier = torch.nn.Sequential(
            torch.nn.Linear(576, 128),
            torch.nn.BatchNorm1d(128)
        )
        self.model.eval()
        self.model.to(self.device)
        
        self.transform = transforms.Compose([
            transforms.ToPILImage(),
            transforms.Resize((128, 128)),
            transforms.ToTensor(),
            transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
        ])

    def encode(self, crop):
        if crop is None or crop.size == 0:
            return np.random.randn(128)
        
        rgb = cv2.cvtColor(crop, cv2.COLOR_BGR2RGB)
        tensor = self.transform(rgb).unsqueeze(0).to(self.device)
        
        with torch.no_grad():
            emb = self.model(tensor).cpu().numpy().flatten()
        
        norm = np.linalg.norm(emb) + 1e-8
        return emb / norm