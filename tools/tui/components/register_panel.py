from textual.widgets import Static
class RegisterPanel(Static):
    """Register panel - displays S0-S15, PC, FLAGS with status dots"""

    def __init__(self, **kwargs):
        super().__init__("", **kwargs)
        self.powered = False
        self.render_registers()

    def render_registers(self):
        """Render the register display"""
        # Status dots (grey when off, green when on)
        if self.powered:
            dots = "● ● ● ● ● ● ● ● ● ● ● ● ● ● ● ●"  # 16 green dots (powered on)
            dot_label = "(active)"
        else:
            dots = "○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○"  # 16 grey dots (powered off)
            dot_label = "(off)"

        # Default values (powered off or initial state)
        if self.powered:
            registers = {
                'S0': 0x0000, 'S1': 0x0000, 'S2': 0x0000, 'S3': 0x0000,
                'S4': 0x0000, 'S5': 0x0000, 'S6': 0x0000, 'S7': 0x0000,
                'S8': 0x0000, 'S9': 0x0000, 'S10': 0x0000, 'S11': 0x0000,
                'S12': 0x0000, 'S13': 0xFFFF, 'S14': 0x0000, 'S15': 0x0000
            }
            pc = 0x0100
            flags = "0000"
        else:
            registers = {f'S{i}': 0x0000 for i in range(16)}
            pc = 0x0000
            flags = "----"

        content = f"""REGISTERS
─────────────────────
{dots}
{dot_label}
─────────────────────

S0:  0x{registers['S0']:04X}
S1:  0x{registers['S1']:04X}
S2:  0x{registers['S2']:04X}
S3:  0x{registers['S3']:04X}
S4:  0x{registers['S4']:04X}
S5:  0x{registers['S5']:04X}
S6:  0x{registers['S6']:04X}
S7:  0x{registers['S7']:04X}
S8:  0x{registers['S8']:04X}
S9:  0x{registers['S9']:04X}
S10: 0x{registers['S10']:04X}
S11: 0x{registers['S11']:04X}
S12: 0x{registers['S12']:04X}
S13: 0x{registers['S13']:04X}  ← SP
S14: 0x{registers['S14']:04X}  ← FP
S15: 0x{registers['S15']:04X}  ← RA

─────────────────────
PC:    0x{pc:04X}
FLAGS: {flags}
"""
        self.update(content)

    def power_on(self):
        """Power on the CPU - initialize registers"""
        self.powered = True
        self.render_registers()

    def power_off(self):
        """Power off the CPU - clear all registers"""
        self.powered = False
        self.render_registers()

    def update_registers(self, reg_data):
        """Update register values (for later use with simulator)"""
        # TODO: Will implement when connecting to C simulator
        pass
