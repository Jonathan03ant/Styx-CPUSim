from textual.screen import ModalScreen
from textual.containers import Container, Horizontal
from textual.widgets import Input, Button, Label


class FilenameInputModal(ModalScreen):
    """Small modal for entering filename"""

    CSS = """
    FilenameInputModal {
        align: center middle;
    }

    #filename-dialog {
        width: 50;
        height: 12;
        border: thick $primary;
        background: $surface;
        padding: 1;
    }

    #filename-label {
        margin-bottom: 1;
    }

    #filename-input-container {
        height: 3;
        margin-bottom: 1;
    }

    #filename-input {
        width: 1fr;
    }

    #filename-extension {
        width: auto;
        padding: 0 1;
    }

    #filename-buttons {
        height: 3;
        align: center middle;
    }

    Button {
        margin: 0 2;
    }
    """

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.filename = None

    def compose(self):
        with Container(id="filename-dialog"):
            yield Label("Enter filename:", id="filename-label")
            with Horizontal(id="filename-input-container"):
                yield Input(placeholder="program_1", id="filename-input")
                yield Label(".prg", id="filename-extension")
            with Horizontal(id="filename-buttons"):
                yield Button("OK", variant="primary", id="ok-btn")
                yield Button("Cancel", variant="default", id="cancel-btn")

    def on_mount(self):
        """Focus input when modal opens"""
        input_field = self.query_one(Input)
        input_field.focus()

    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle button clicks"""
        if event.button.id == "ok-btn":
            input_field = self.query_one(Input)
            filename = input_field.value.strip()

            if filename:
                # Add .prg extension if not present
                if not filename.endswith(".prg"):
                    filename = f"{filename}.prg"
                self.dismiss(filename)
            else:
                # Empty filename, generate default
                self.dismiss("program.prg")
        else:  # cancel-btn
            self.dismiss(None)

    def on_input_submitted(self, event: Input.Submitted) -> None:
        """Handle Enter key in input field"""
        filename = event.value.strip()
        if filename:
            if not filename.endswith(".prg"):
                filename = f"{filename}.prg"
            self.dismiss(filename)
        else:
            self.dismiss("program.prg")
