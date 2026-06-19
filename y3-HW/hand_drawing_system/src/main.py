import sys
import os
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import argparse
import yaml
from PySide6.QtWidgets import QApplication
from src.core.system import HandDrawingSystem
from src.gui.main_window import MainWindow

def get_project_root():
    return os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def ensure_config(config_rel_path):
    abs_path = os.path.join(get_project_root(), config_rel_path)
    if os.path.exists(abs_path):
        return abs_path
    
    default_config = {
        'camera': {
            'device_id': 0,
            'width': 1280,
            'height': 720,
            'target_fps': 30
        },
        'detection': {
            'max_hands': 3
        },
        'tracking': {
            'iou_threshold': 0.3,
            'reid_cosine_threshold': 0.65,
            'max_lost_frames': 15
        },
        'user_binding': {
            'max_users': 3,
            'default_colors': [
                [0, 0, 255],
                [0, 255, 0],
                [255, 0, 0]
            ]
        },
        'drawing': {
            'pinch_threshold_ratio': 0.15,
            'canvas_width': 1280,
            'canvas_height': 720,
            'brush_thickness': 4,
            'one_euro': {
                'min_cutoff': 1.0,
                'beta': 0.1,
                'd_cutoff': 1.0
            }
        }
    }
    os.makedirs(os.path.dirname(abs_path), exist_ok=True)
    with open(abs_path, 'w') as f:
        yaml.dump(default_config, f, default_flow_style=False)
    print(f"[INFO] Created default config file: {abs_path}")
    return abs_path

def load_config(config_rel_path):
    abs_path = ensure_config(config_rel_path)
    with open(abs_path, 'r') as f:
        return yaml.safe_load(f)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", type=str, default="config/default.yaml")
    args = parser.parse_args()

    config = load_config(args.config)
    
    system = HandDrawingSystem(config)
    
    app = QApplication(sys.argv)
    window = MainWindow(system)
    window.show()
    
    sys.exit(app.exec())

if __name__ == "__main__":
    main()