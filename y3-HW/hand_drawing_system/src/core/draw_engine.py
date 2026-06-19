import cv2
import numpy as np
from collections import defaultdict
from src.utils.filter import OneEuroFilter

class DrawingEngine:
    def __init__(self, config):
        self.canvas = np.zeros((config['drawing']['canvas_height'],
                                config['drawing']['canvas_width'], 3), dtype=np.uint8)
        self.pen_down = defaultdict(bool)
        self.last_points = {}
        self.filters = {}
        self.thickness = config['drawing']['brush_thickness']
        self.user_colors = {}
        self.user_pressed_state = {}
        self.filter_config = config['drawing']['one_euro']

    def set_user_color(self, track_id, bgr_color):
        self.user_colors[track_id] = tuple(bgr_color)

    def reset_filter(self, track_id):
        if track_id in self.filters:
            del self.filters[track_id]
        if track_id in self.last_points:
            del self.last_points[track_id]

    def is_user_pressed(self, user_id):
        return self.user_pressed_state.get(user_id, False)

    def update_and_draw(self, track_id, raw_point, is_pressed, user_id):
        # 初始化濾波器
        if track_id not in self.filters:
            self.filters[track_id] = OneEuroFilter(
                min_cutoff=self.filter_config['min_cutoff'],
                beta=self.filter_config['beta'],
                d_cutoff=self.filter_config['d_cutoff']
            )
            self.last_points[track_id] = None

        # 濾波
        fx, fy = self.filters[track_id].filter(raw_point[0], raw_point[1])
        current_point = (fx, fy)

        # 更新筆狀態
        self.pen_down[track_id] = is_pressed
        self.user_pressed_state[user_id] = is_pressed

        # 顏色
        color = self.user_colors.get(track_id, (0, 0, 255))
        if color == (0, 0, 0):
            color = (0, 0, 255)

        # 繪圖
        if is_pressed:
            prev_point = self.last_points.get(track_id)
            if prev_point is None:
                cv2.circle(self.canvas, (int(fx), int(fy)), self.thickness // 2, color, -1)
                cv2.circle(self.canvas, (int(fx), int(fy)), self.thickness, color, 1)
            else:
                prev_x, prev_y = prev_point
                dist = np.hypot(fx - prev_x, fy - prev_y)
                if dist < 100:
                    cv2.line(self.canvas,
                             (int(prev_x), int(prev_y)),
                             (int(fx), int(fy)),
                             color, self.thickness, cv2.LINE_AA)
                else:
                    cv2.circle(self.canvas, (int(fx), int(fy)), self.thickness // 2, color, -1)

        self.last_points[track_id] = current_point

    def render(self):
        return self.canvas

    def clear_canvas(self):
        self.canvas.fill(0)
        self.last_points.clear()
        self.pen_down.clear()
        self.user_pressed_state.clear()