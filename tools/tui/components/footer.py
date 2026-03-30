from textual.widgets import Static

class Footer(Static):
    """Console footer - displays status messages"""

    def __init__(self, **kwargs):
        super().__init__("📟 Console: System ready. Waiting for commands...", **kwargs)

    def log(self, message: str):
        """Update console message"""
        self.update(f"📟 Console: {message}")