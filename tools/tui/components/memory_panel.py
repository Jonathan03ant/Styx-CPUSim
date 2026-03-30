from textual.widgets import Static


class MemoryPanel(Static):
    """Memory panel - displays memory regions"""

    def __init__(self, **kwargs):
        super().__init__("", **kwargs)
        self.render_memory()

    def render_memory(self):
        content = """MEMORY REGIONS
─────────────────────

Code (0x0100-1FFF):
[░░░░░░░░░░] 0%

Data (0x2000-7FFF):
[░░░░░░░░░░] 0%

Heap (0x8000-DFFF):
[░░░░░░░░░░] 0%

Stack (0xE000-FFFF):
[░░░░░░░░░█] SP

─────────────────────
Search Address:
[0x____] [Go]

Result: --
"""
        self.update(content)