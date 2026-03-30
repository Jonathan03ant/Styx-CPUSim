from textual.widgets import Static
from textual.containers import Container, Horizontal
from textual.message import Message


class ControlButton(Static):
    """Clickable control button"""
    def __init__(self, label: str, action: str, **kwargs):
        super().__init__(label, **kwargs)
        self.action = action


class ControlBar(Container):
    """Control bar with action buttons (2 rows)"""

    class ButtonClicked(Message):
        """Posted when a control button is clicked"""
        def __init__(self, action: str):
            super().__init__()
            self.action = action

    def compose(self):
        # Single row: All controls
        with Horizontal(classes="control-row"):
            yield ControlButton("Load ASM", "load-asm", classes="btn")
            yield ControlButton("Write ASM", "write-asm", classes="btn")
            yield ControlButton("Load .prg", "load-prg", classes="btn")
            yield ControlButton("Write .prg", "write-prg", classes="btn")
            yield ControlButton("▶️ Execute", "execute", classes="btn")
            yield ControlButton("Step", "step", classes="btn")
            yield ControlButton("⟳ Reset", "reset", classes="btn")

    def on_click(self, event) -> None:
        """Handle button clicks"""
        clicked = event.widget
        if isinstance(clicked, ControlButton):
            self.post_message(self.ButtonClicked(clicked.action))
