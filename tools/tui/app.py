from textual.app import App
from textual.containers import Horizontal
from components.footer import Footer
from components.header_bar import HeaderBar
from components.control_bar import ControlBar
from components.register_panel import RegisterPanel
from components.code_panel import CodePanel
from components.memory_panel import MemoryPanel

class StyxTUI(App):
    """Styx CPU Simulator TUI"""

    CSS_PATH = "styx.tcss"

    def compose(self):
        yield HeaderBar()
        yield ControlBar()
        # Main 3-panel content
        with Horizontal(id="main-panels"):
            yield RegisterPanel(id="registers")
            yield CodePanel(id="code")
            yield MemoryPanel(id="memory")
        yield Footer()

    def on_header_bar_power_toggled(self, event):
        "Handle power button clicks"
        footer = self.query_one(Footer)
        register_panel = self.query_one(RegisterPanel)
        if event.powered:
            footer.log("DEBUG: Power turned ON")
            register_panel.power_on()
        else:
            footer.log("DEBUG: Power turned OFF")
            register_panel.power_off()


if __name__ == "__main__":
    app = StyxTUI()
    app.run()