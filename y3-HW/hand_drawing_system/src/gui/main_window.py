from PySide6.QtWidgets import QMainWindow, QLabel, QVBoxLayout, QWidget, QHBoxLayout, QPushButton
from PySide6.QtGui import QPixmap, QImage, QColor, QPainter, QPen
from PySide6.QtCore import QTimer, Qt
import cv2
import numpy as np

class MainWindow(QMainWindow):
    def __init__(self, system):
        super().__init__()
        self.system = system
        self.system.initialize()
        
        self.setWindowTitle("Collaborative Air Drawing (PySide6)")
        self.setGeometry(100, 100, 1280, 800)
        
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)
        
        self.canvas_label = QLabel()
        self.canvas_label.setFixedSize(1280, 720)
        self.canvas_label.setStyleSheet("background-color: black;")
        layout.addWidget(self.canvas_label)
        
        control_layout = QHBoxLayout()
        self.clear_btn = QPushButton("Clear Canvas")
        self.clear_btn.clicked.connect(self.clear_canvas)
        control_layout.addWidget(self.clear_btn)
        control_layout.addStretch()
        
        self.status_label = QLabel("System Ready | Users: 0")
        control_layout.addWidget(self.status_label)
        layout.addLayout(control_layout)
        
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_frame)
        self.timer.start(33)

    def clear_canvas(self):
        self.system.draw_engine.clear_canvas()

    def update_frame(self):
        ret = self.system.process_frame()
        if not ret:
            return
        
        canvas = self.system.get_canvas()
        if canvas is None:
            return
        
        rgb_image = cv2.cvtColor(canvas, cv2.COLOR_BGR2RGB)
        h, w, ch = rgb_image.shape
        bytes_per_line = ch * w
        qt_image = QImage(rgb_image.data, w, h, bytes_per_line, QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(qt_image)
        
        scaled_pixmap = pixmap.scaled(self.canvas_label.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)
        self.canvas_label.setPixmap(scaled_pixmap)
        
        user_count = len(self.system.user_manager.track_to_user)
        self.status_label.setText(f"Running | Active Users: {user_count}")