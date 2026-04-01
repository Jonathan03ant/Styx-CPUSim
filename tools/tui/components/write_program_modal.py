from textual.screen import ModalScreen
from textual.containers import Container, Horizontal
from textual.widgets import TextArea, Button, Label


class WriteProgramModal(ModalScreen):
    """Modal for writing hex programs"""

    CSS = """
    WriteProgramModal {
        align: center middle;
    }

    #write-dialog {
        width: 70;
        height: 30;
        border: thick $primary;
        background: $surface;
        padding: 1;
    }

    #write-text-area {
        height: 1fr;
        margin-bottom: 1;
    }

    #write-buttons {
        height: 3;
        align: center middle;
    }

    Button {
        margin: 0 2;
    }
    """

    def __init__(self, initial_text: str = "", **kwargs):
        super().__init__(**kwargs)
        self.initial_text = initial_text

    def compose(self):
        with Container(id="write-dialog"):
            yield Label("Write Program (hex instructions, one per line)", id="write-label")
            yield TextArea(self.initial_text, id="write-text-area")
            with Horizontal(id="write-buttons"):
                yield Button("Save", variant="primary", id="save-btn")
                yield Button("Cancel", variant="default", id="cancel-btn")

    def on_mount(self):
        """Focus text area when modal opens"""
        text_area = self.query_one(TextArea)
        text_area.focus()

    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle button clicks"""
        if event.button.id == "save-btn":
            # Get text content
            text_area = self.query_one(TextArea)
            program_text = text_area.text

            if program_text.strip():
                # Dismiss with program text to trigger filename dialog
                self.dismiss(program_text)
            else:
                # Empty program, just close
                self.dismiss(None)
        else:  # cancel-btn
            self.dismiss(None)
