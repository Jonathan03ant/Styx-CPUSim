from textual.widgets import Static
from textual.containers import Container, Vertical
from textual.reactive import reactive


class FileList(Static):
    """Files section - displays saved .prg files"""

    files = reactive([])

    def __init__(self, **kwargs):
        super().__init__("", **kwargs)

    def watch_files(self, files: list):
        """Re-render when files list changes"""
        self.render_files()

    def render_files(self):
        if not self.files:
            content = "FILES: (no files)"
        else:
            content = "FILES:\n"
            content += "\n".join(f"• {filename}" for filename in self.files)
        self.update(content)


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