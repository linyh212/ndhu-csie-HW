class UserManager:
    def __init__(self, config):
        self.max_users = config['user_binding']['max_users']
        self.colors = config['user_binding']['default_colors']
        self.track_to_user = {}
        self.user_to_track = {}
        self.used_user_ids = set()

    def get_or_assign_user(self, track_id):
        if track_id in self.track_to_user:
            return self.track_to_user[track_id]

        for user_id in range(self.max_users):
            if user_id not in self.used_user_ids:
                self.track_to_user[track_id] = user_id
                self.user_to_track[user_id] = track_id
                self.used_user_ids.add(user_id)
                return user_id
        return None

    def get_color(self, track_id):
        user_id = self.track_to_user.get(track_id)
        if user_id is not None and user_id < len(self.colors):
            return tuple(self.colors[user_id])
        return (0, 0, 255)

    def release_inactive_tracks(self, active_track_ids):
        active_set = set(active_track_ids)
        to_remove = [tid for tid in self.track_to_user if tid not in active_set]
        for track_id in to_remove:
            user_id = self.track_to_user[track_id]
            del self.track_to_user[track_id]
            del self.user_to_track[user_id]
            self.used_user_ids.remove(user_id)