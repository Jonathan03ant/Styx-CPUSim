from textual.widgets import Static
from textual.containers import Horizontal

class Footer(Horizontal):
    """Console footer - displays status messages"""

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.mem_info = ""

    def compose(self):
        yield Static("", id="footer-left")
        yield Static("📟 Console: System ready. Waiting for commands...", id="footer-center")
        yield Static("", id="footer-right")

    def log(self, message: str):
        """Update console message"""
        center = self.query_one("#footer-center", Static)
        center.update(f"📟 Console: {message}")

    def set_mem_info(self, info: str):
        """Update memory info on right side"""
        self.mem_info = info
        right = self.query_one("#footer-right", Static)
        right.update(f"[dim]{info}[/dim]" if info else "")