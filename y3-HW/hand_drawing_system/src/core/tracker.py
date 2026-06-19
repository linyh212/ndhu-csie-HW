import numpy as np
from scipy.optimize import linear_sum_assignment
from collections import deque
from src.core.reid_model import AppearanceEmbedding

class TrackState:
    def __init__(self, track_id, detection, embedding):
        self.track_id = track_id
        self.bbox = detection['bbox']
        self.landmarks = detection['landmarks']
        self.embedding = embedding
        self.lost_count = 0
        self.is_active = True
        self.emb_history = deque(maxlen=5)
        self.emb_history.append(embedding)

    def update_embedding(self, new_emb):
        self.emb_history.append(new_emb)
        self.embedding = np.mean(self.emb_history, axis=0)

class HandTracker:
    def __init__(self, config):
        self.config = config
        self.next_id = 0
        self.tracks = []
        self.lost_tracks = []
        self.reid = AppearanceEmbedding()
        self.iou_thresh = 0.4
        self.cosine_thresh = config['tracking']['reid_cosine_threshold']
        self.max_lost = 3

    def _iou(self, box1, box2):
        x1 = max(box1[0], box2[0]); y1 = max(box1[1], box2[1])
        x2 = min(box1[2], box2[2]); y2 = min(box1[3], box2[3])
        inter = max(0, x2-x1) * max(0, y2-y1)
        area1 = (box1[2]-box1[0])*(box1[3]-box1[1])
        area2 = (box2[2]-box2[0])*(box2[3]-box2[1])
        union = area1 + area2 - inter
        return inter / (union + 1e-6)

    def update(self, detections, crops):
        # 1. 提取特徵
        for i, det in enumerate(detections):
            if i < len(crops) and crops[i] is not None:
                emb = self.reid.encode(crops[i])
            else:
                emb = np.random.randn(128) * 0.1
            det['embedding'] = emb / (np.linalg.norm(emb) + 1e-8)

        # 2. 關聯比對
        matched_det_indices = set()
        if len(self.tracks) > 0 and len(detections) > 0:
            cost_matrix = np.zeros((len(self.tracks), len(detections)))
            for i, trk in enumerate(self.tracks):
                for j, det in enumerate(detections):
                    iou = self._iou(trk.bbox, det['bbox'])
                    cost_matrix[i, j] = 1 - iou
            
            row_idx, col_idx = linear_sum_assignment(cost_matrix)
            for r, c in zip(row_idx, col_idx):
                if cost_matrix[r, c] < (1 - self.iou_thresh):
                    self.tracks[r].bbox = detections[c]['bbox']
                    self.tracks[r].landmarks = detections[c]['landmarks']
                    self.tracks[r].lost_count = 0
                    self.tracks[r].update_embedding(detections[c]['embedding'])
                    matched_det_indices.add(c)
                else:
                    self.tracks[r].lost_count += 1

            for r in range(len(self.tracks)):
                if r not in row_idx:
                    self.tracks[r].lost_count += 1

        # 3. 處理未匹配的檢測（建立新軌跡）
        unmatched_det_indices = set(range(len(detections))) - matched_det_indices
        for c in unmatched_det_indices:
            det = detections[c]
            # 嚴格限制：只有當偵測信心夠高，且目前軌跡數小於偵測數+1 才建立
            if det['conf'] > 0.4:
                new_track = TrackState(self.next_id, det, det['embedding'])
                self.tracks.append(new_track)
                self.next_id += 1

        # 4. 清除：限制軌跡數量不得超過偵測數量的 2 倍（避免幽靈軌跡爆量）
        if len(self.tracks) > len(detections) * 2 + 1:
            self.tracks.sort(key=lambda x: x.lost_count, reverse=True)
            keep_count = max(1, len(detections) * 2)
            self.tracks = self.tracks[:keep_count]

        # 5. 清理 lost_count 超過上限的軌跡
        active_tracks = []
        for trk in self.tracks:
            if trk.lost_count > self.max_lost:
                self.lost_tracks.append({
                    'track_id': trk.track_id,
                    'embedding': trk.embedding,
                    'bbox': trk.bbox
                })
                if len(self.lost_tracks) > 10:
                    self.lost_tracks.pop(0)
            else:
                active_tracks.append(trk)
        self.tracks = active_tracks

        # 6. 回傳結果
        result = []
        for trk in self.tracks:
            result.append({
                'track_id': trk.track_id,
                'bbox': trk.bbox,
                'landmarks': trk.landmarks,
                'embedding': trk.embedding
            })
        return result