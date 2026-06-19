import cv2
import numpy as np
import mediapipe as mp

class HandDetector:
    def __init__(self, config=None, min_detection_confidence=0.6, min_tracking_confidence=0.6):
        if config is not None:
            self.max_hands = config.get('detection', {}).get('max_hands', 3)
            min_detection_confidence = config.get('detection', {}).get('min_detection_confidence', 0.6)
            min_tracking_confidence = config.get('detection', {}).get('min_tracking_confidence', 0.6)
        else:
            self.max_hands = 3

        self.mp_hands = mp.solutions.hands
        self.hands = self.mp_hands.Hands(
            static_image_mode=False,
            max_num_hands=self.max_hands,
            min_detection_confidence=min_detection_confidence,
            min_tracking_confidence=min_tracking_confidence
        )

    def detect(self, frame):
        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = self.hands.process(rgb)
        hands = []
        if results.multi_hand_landmarks:
            h, w, _ = frame.shape
            for hand_lms in results.multi_hand_landmarks:
                landmarks = []
                for lm in hand_lms.landmark:
                    cx, cy = int(lm.x * w), int(lm.y * h)
                    landmarks.append((cx, cy))
                landmarks = np.array(landmarks, dtype=np.float32)
                xs = landmarks[:, 0]
                ys = landmarks[:, 1]
                x1, y1 = max(0, int(np.min(xs))), max(0, int(np.min(ys)))
                x2, y2 = min(w, int(np.max(xs))), min(h, int(np.max(ys)))
                pad = 10
                x1, y1 = max(0, x1 - pad), max(0, y1 - pad)
                x2, y2 = min(w, x2 + pad), min(h, y2 + pad)
                hands.append({
                    'bbox': (x1, y1, x2, y2),
                    'landmarks': landmarks,
                    'conf': 1.0
                })
        return hands