from textual.widgets import Static

class Footer(Static):
    DEFAULT_CSS = """
    ConsoleFooter {
        width: 100%;
        height: 3;
        background: $panel;
        border: solid $primary;
        padding: 0 1;
        content-align: left middle;
    }
    """

    def __init__(self, **kwargs):
        super().__init__("Console: Ready.", **kwargs)

    def log(self, message: str):
        """Update console message"""
        self.update(f"Console: {message}")