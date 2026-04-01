from textual.widgets import Static, Button
from textual.containers import Container, Vertical, Horizontal
from textual.reactive import reactive
from textual.message import Message


class FileItem(Static):
    """Individual file item (just the name, clickable)"""

    class Selected(Message):
        """Posted when file is selected (single-click)"""
        def __init__(self, filename: str):
            super().__init__()
            self.filename = filename

    class DoubleClicked(Message):
        """Posted when file is double-clicked for editing"""
        def __init__(self, filename: str):
            super().__init__()
            self.filename = filename

    def __init__(self, filename: str, is_selected: bool = False, **kwargs):
        super().__init__(**kwargs)
        self.filename = filename
        self.is_selected = is_selected
        marker = "►" if is_selected else "•"
        self.update(f"{marker} {self.filename}")

    def on_click(self, event) -> None:
        """Handle single/double click on file name"""
        if event.button != 1:  # Only left mouse button
            return

        # Detect double-click
        import time
        current_time = time.time()
        if not hasattr(self, 'last_click_time'):
            self.last_click_time = 0

        if current_time - self.last_click_time < 0.5:  # 500ms double-click threshold
            self.post_message(self.DoubleClicked(self.filename))
        else:
            self.post_message(self.Selected(self.filename))

        self.last_click_time = current_time


class FileList(Vertical):
    """Files section - displays saved .prg files"""

    files = reactive([])
    selected_file = reactive(None)

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

    def compose(self):
        # Simple header
        yield Static("FILES:", classes="file-list-header")

    def watch_files(self, files: list):
        """Re-render when files list changes"""
        self.render_files()

    def watch_selected_file(self, selected: str | None):
        """Re-render when selection changes"""
        self.render_files()

    def render_files(self):
        # Remove all existing file items
        for child in list(self.query(FileItem)):
            child.remove()

        # Add file items
        if not self.files:
            if not self.query(".no-files-msg"):
                self.mount(Static("(no files)", classes="no-files-msg"))
        else:
            # Remove "no files" message if it exists
            for msg in self.query(".no-files-msg"):
                msg.remove()

            # Mount new file items
            for filename in self.files:
                is_selected = (filename == self.selected_file)
                self.mount(FileItem(filename, is_selected, classes="file-item"))


class CodeView(Static):
    """Code view - displays disassembly"""

    def __init__(self, **kwargs):
        super().__init__("", **kwargs)
        self.render_code()

    def render_code(self):
        content = """CODE VIEW
─────────────────────

0x0100: (empty)
0x0102: (empty)
0x0104: (empty)
0x0106: (empty)

─────────────────────
Status: POWERED OFF
Cycle: 0

─────────────────────
Internal Registers:
MAR: 0x0000
MDR: 0x0000
IR:  0x0000
"""
        self.update(content)


class CodePanel(Vertical):
    """Code panel - file list + code view"""

    def compose(self):
        yield FileList(id="file-list")
        yield CodeView(id="code-view")