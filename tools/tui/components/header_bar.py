from textual.widgets import Static
from textual.containers import Horizontal
from textual.message import Message


class PowerButton(Static):
    """Clickable power button"""
    def __init__(self, **kwargs):
        super().__init__("⏻ Power OFF", **kwargs)
        self.powered = False


class HeaderBar(Horizontal):
    """Header bar with title and power toggle button"""

    class PowerToggled(Message):
        """Posted when power button is clicked"""
        def __init__(self, powered: bool):
            super().__init__()
            self.powered = powered

    def compose(self):
        yield Static("Styx CPU Simulator v1.0", id="title")
        yield PowerButton(id="btn-power")

    def on_click(self, event) -> None:
        """Handle power button clicks"""
        clicked = event.widget
        if isinstance(clicked, PowerButton):
            clicked.powered = not clicked.powered
            if clicked.powered:
                clicked.update("⏻ Power ON")
                clicked.add_class("powered-on")
            else:
                clicked.update("⏻ Power OFF")
                clicked.remove_class("powered-on")
            self.post_message(self.PowerToggled(clicked.powered))