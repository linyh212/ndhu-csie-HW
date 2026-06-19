import math
import time

class OneEuroFilter:
    def __init__(self, min_cutoff=2.0, beta=0.05, d_cutoff=1.0):
        self.min_cutoff = min_cutoff
        self.beta = beta
        self.d_cutoff = d_cutoff
        self.prev_x = None
        self.prev_dx = 0.0
        self.prev_time = None

    def filter(self, x, y):
        now = time.time()
        if self.prev_x is None:
            self.prev_x = (float(x), float(y))
            self.prev_time = now
            return x, y
        
        dt = max(0.001, now - self.prev_time)
        dx = math.hypot(x - self.prev_x[0], y - self.prev_x[1]) / dt
        cutoff = self.min_cutoff + self.beta * abs(dx)
        alpha = 1.0 / (1.0 + cutoff * dt)
        
        smooth_x = alpha * x + (1 - alpha) * self.prev_x[0]
        smooth_y = alpha * y + (1 - alpha) * self.prev_x[1]
        
        self.prev_x = (smooth_x, smooth_y)
        self.prev_time = now
        return smooth_x, smooth_y