from textual.widgets import Static
class RegisterPanel(Static):
    """Register panel - displays S0-S15, PC, FLAGS with status dots"""

    def __init__(self, **kwargs):
        super().__init__("", **kwargs)
        self.powered = False
        self.markup = True
        self.snapshot = None
        self.highlight_regs = set()  # Set of register indices to highlight
        self.render_registers()

    def render_registers(self):
        """Render the register display"""
        # Get values from snapshot if available, otherwise defaults
        if self.powered and self.snapshot:
            registers = {f'S{i}': self.snapshot.regs[i] for i in range(16)}
            pc = self.snapshot.pc
            # Format FLAGS as binary string
            flags_val = self.snapshot.flags
            flags = f"{(flags_val & 0x01):01b}{(flags_val & 0x02) >> 1:01b}{(flags_val & 0x04) >> 2:01b}{(flags_val & 0x08) >> 3:01b}"
        elif self.powered:
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

        # Build status dots (grey=off, green=idle, yellow=highlighted, red=occupied/active)
        if self.powered:
            dot_chars = []
            for i in range(16):
                if i in self.highlight_regs:
                    dot_chars.append("[black on yellow]●[/black on yellow]")  # Highlighted (changed)
                elif registers[f'S{i}'] != 0x0000:
                    dot_chars.append("[red]●[/red]")  # Occupied (non-zero)
                else:
                    dot_chars.append("[green]●[/green]")  # Idle (zero)
            dots = " ".join(dot_chars)
            dot_label = "[green](active)[/green]"
        else:
            dots = "[dim]○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○[/dim]"
            dot_label = "[dim](off)[/dim]"

        # Color register values (red if occupied, white if idle, highlight if changed)
        def format_reg(name, value, idx):
            if idx in self.highlight_regs:
                return f"[black on yellow]{name}: 0x{value:04X}[/black on yellow]"
            elif value != 0x0000:
                return f"[red]{name}: 0x{value:04X}[/red]"
            else:
                return f"{name}: 0x{value:04X}"

        # Color PC (red if non-zero)
        if pc != 0x0000:
            pc_line = f"[red]PC:    0x{pc:04X}[/red]"
        else:
            pc_line = f"PC:    0x{pc:04X}"

        content = f"""REGISTERS
─────────────────────
{dots}
{dot_label}
─────────────────────

{format_reg('S0', registers['S0'], 0)}
{format_reg('S1', registers['S1'], 1)}
{format_reg('S2', registers['S2'], 2)}
{format_reg('S3', registers['S3'], 3)}
{format_reg('S4', registers['S4'], 4)}
{format_reg('S5', registers['S5'], 5)}
{format_reg('S6', registers['S6'], 6)}
{format_reg('S7', registers['S7'], 7)}
{format_reg('S8', registers['S8'], 8)}
{format_reg('S9', registers['S9'], 9)}
{format_reg('S10', registers['S10'], 10)}
{format_reg('S11', registers['S11'], 11)}
{format_reg('S12', registers['S12'], 12)}
{format_reg('S13', registers['S13'], 13)}  ← SP
{format_reg('S14', registers['S14'], 14)}  ← FP
{format_reg('S15', registers['S15'], 15)}  ← RA

─────────────────────
{pc_line}
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

    def update_from_snapshot(self, snapshot_before, snapshot_after):
        """Update registers from CPU snapshot, highlighting changed ones"""
        self.snapshot = snapshot_after

        # Find which registers changed
        self.highlight_regs.clear()
        for i in range(16):
            if snapshot_before.regs[i] != snapshot_after.regs[i]:
                self.highlight_regs.add(i)

        # Render with highlights
        self.render_registers()

    def clear_highlights(self):
        """Clear all register highlights"""
        self.highlight_regs.clear()
        self.render_registers()
