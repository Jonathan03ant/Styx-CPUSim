from textual.widgets import Static


class CodePanel(Static):
    """Code panel - displays disassembly view"""

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