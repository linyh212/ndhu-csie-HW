import cv2
import numpy as np
import time
from src.core.detector import HandDetector
from src.core.user_manager import UserManager
from src.core.tracker import HandTracker
from src.core.draw_engine import DrawingEngine

class HandDrawingSystem:
    def __init__(self, config):
        self.config = config
        detector_config = config.get('detector', {})
        self.detector = HandDetector(
            min_detection_confidence=detector_config.get('min_detection_confidence', 0.6),
            min_tracking_confidence=detector_config.get('min_tracking_confidence', 0.6)
        )
        self.user_manager = UserManager(config)
        self.tracker = HandTracker(config)
        self.draw_engine = DrawingEngine(config)
        
        self.cap = None
        self.running = False
        self.current_canvas = None
        self.fps = 0.0
        self.frame_count = 0
        
        self.FORCE_DRAW = False

    def initialize(self):
        cam_cfg = self.config['camera']
        self.cap = cv2.VideoCapture(cam_cfg['device_id'])
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, cam_cfg['width'])
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, cam_cfg['height'])
        if not self.cap.isOpened():
            print("ERROR: Cannot open camera")
            self.running = False
            return False
        self.draw_engine.clear_canvas()
        self.running = True
        print("[INFO] Camera initialized. FORCE_DRAW =", self.FORCE_DRAW)
        return True

    def process_frame(self):
        if not self.running or self.cap is None:
            return False
        
        ret, frame = self.cap.read()
        if not ret:
            return False
        
        self.frame_count += 1
        frame = cv2.flip(frame, 1)
        start_time = time.time()
        display_frame = frame.copy()
        
        try:
            hands = self.detector.detect(frame)
            
            for hand in hands:
                x1, y1, x2, y2 = hand['bbox']
                cv2.rectangle(display_frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                for pt in hand['landmarks']:
                    cv2.circle(display_frame, (int(pt[0]), int(pt[1])), 3, (0, 0, 255), -1)

            if len(hands) > 0:
                crops = []
                for h in hands:
                    x1, y1, x2, y2 = h['bbox']
                    crop = frame[y1:y2, x1:x2]
                    crops.append(crop if crop.size > 0 else None)

                tracks = self.tracker.update(hands, crops)
                
                active_track_ids = [t['track_id'] for t in tracks]
                self.user_manager.release_inactive_tracks(active_track_ids)
                
                any_pressed = False
                text_y = 180
                
                for track in tracks:
                    track_id = track['track_id']
                    user_id = self.user_manager.get_or_assign_user(track_id)
                    if user_id is None:
                        continue

                    lm = track['landmarks']
                    if lm is None or len(lm) < 9:
                        continue
                        
                    fingertip = lm[8]
                    thumb = lm[4]
                    wrist = lm[0]
                    hand_size = np.linalg.norm(wrist - lm[9])
                    if hand_size < 1:
                        hand_size = 1
                    pinch_dist = np.linalg.norm(fingertip - thumb)
                    ratio = pinch_dist / hand_size
                    
                    is_pressed = ratio < 0.25
                    
                    if is_pressed:
                        any_pressed = True
                    
                    if track_id not in self.draw_engine.filters:
                        self.draw_engine.reset_filter(track_id)
                    
                    status_text = "DOWN" if is_pressed else "UP  "
                    cv2.putText(display_frame,
                                f"ID{track_id} ratio={ratio:.3f} [{status_text}]",
                                (50, text_y),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)
                    text_y += 30

                    color = self.user_manager.get_color(track_id)
                    if color is None:
                        color = (0, 0, 255)
                    self.draw_engine.set_user_color(track_id, color)
                    
                    self.draw_engine.update_and_draw(track_id, fingertip, is_pressed, user_id)
                
                cv2.putText(display_frame, f"PEN {'DOWN' if any_pressed else 'UP'}", (50, 50),
                            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0) if any_pressed else (0, 0, 255), 2)
            else:
                cv2.putText(display_frame, "NO HAND", (50, 150),
                            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
                
            final_canvas = self.draw_engine.render()
            mask = np.any(final_canvas > 0, axis=-1)
            display_frame[mask] = final_canvas[mask]

            cv2.putText(display_frame, f"FPS: {self.fps:.1f}", (50, 100),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)
            cv2.putText(display_frame, f"Hands: {len(hands)}, Tracks: {len(self.tracker.tracks)}", (50, 140),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)
            
            self.current_canvas = display_frame
            
        except Exception as e:
            print(f"[ERROR] {e}")
            import traceback
            traceback.print_exc()
            self.current_canvas = display_frame
        
        elapsed = time.time() - start_time
        if elapsed > 0:
            self.fps = 0.9 * self.fps + 0.1 * (1.0 / elapsed) if self.fps > 0 else 1.0 / elapsed
        return True

    def get_canvas(self):
        if self.current_canvas is None:
            h = self.config['camera']['height']
            w = self.config['camera']['width']
            return np.zeros((h, w, 3), dtype=np.uint8)
        return self.current_canvas

    def get_fps(self):
        return self.fps

    def shutdown(self):
        self.running = False
        if self.cap:
            self.cap.release()
            print("[INFO] Camera released.")