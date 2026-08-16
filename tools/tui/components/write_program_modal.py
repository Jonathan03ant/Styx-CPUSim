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

    #code-text-area {
        height: 1fr;
        margin-bottom: 1;
    }

    #data-text-area {
        height: 1fr;
        margin-bottom: 1;
    }

    #code-label, #data-label {
        margin-top: 1;
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
            yield Label("CODE (hex, one per line):", id="code-label")
            yield TextArea("", id="code-text-area")
            yield Label("DATA (hex, one per line, optional):", id="data-label")
            yield TextArea("", id="data-text-area")
            with Horizontal(id="write-buttons"):
                yield Button("Save", variant="primary", id="save-btn")
                yield Button("Cancel", variant="default", id="cancel-btn")

    def on_mount(self):
        """Focus CODE text area when modal opens"""
        code_area = self.query_one("#code-text-area", TextArea)
        code_area.focus()

    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle button clicks"""
        if event.button.id == "save-btn":
            code_area = self.query_one("#code-text-area", TextArea)
            data_area = self.query_one("#data-text-area", TextArea)

            code_text = code_area.text.strip()
            data_text = data_area.text.strip()

            if code_text:
                # Return both CODE and DATA text
                self.dismiss((code_text, data_text))
            else:
                # Empty CODE section
                self.dismiss(None)
        else:  # cancel-btn
            self.dismiss(None)
