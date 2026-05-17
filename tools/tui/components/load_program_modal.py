from textual.screen import ModalScreen
from textual.containers import Container, Horizontal
from textual.widgets import Input, Button, Label


class LoadProgramModal(ModalScreen):
    """Modal for loading program from file path"""

    CSS = """
    LoadProgramModal {
        align: center middle;
    }

    #load-dialog {
        width: 60;
        height: 12;
        border: thick $primary;
        background: $surface;
        padding: 1;
    }

    #load-label {
        margin-bottom: 1;
    }

    #load-input {
        width: 100%;
        margin-bottom: 1;
    }

    #load-buttons {
        height: 3;
        align: center middle;
    }

    Button {
        margin: 0 2;
    }
    """

    def compose(self):
        with Container(id="load-dialog"):
            yield Label("Enter file path:", id="load-label")
            yield Input(placeholder="/path/to/program.txt", id="load-input")
            with Horizontal(id="load-buttons"):
                yield Button("Load", variant="primary", id="load-btn")
                yield Button("Cancel", variant="default", id="cancel-btn")

    def on_mount(self):
        """Focus input when modal opens"""
        input_field = self.query_one(Input)
        input_field.focus()

    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle button clicks"""
        if event.button.id == "load-btn":
            input_field = self.query_one(Input)
            file_path = input_field.value.strip()

            if file_path:
                self.dismiss(file_path)
            else:
                self.dismiss(None)
        else:  # cancel-btn
            self.dismiss(None)

    def on_input_submitted(self, event: Input.Submitted) -> None:
        """Handle Enter key in input field"""
        file_path = event.value.strip()
        if file_path:
            self.dismiss(file_path)
        else:
            self.dismiss(None)
