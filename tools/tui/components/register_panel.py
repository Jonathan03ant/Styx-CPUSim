from textual.widgets import Static
class RegisterPanel(Static):
    """Register panel - displays S0-S15, PC, FLAGS with status dots"""

    def __init__(self, **kwargs):
        super().__init__("", **kwargs)
        self.powered = False
        self.markup = True 
        self.render_registers()

    def render_registers(self):
        """Render the register display"""
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

        # Build status dots (grey=off, green=idle, red=occupied/active)
        if self.powered:
            dot_chars = []
            for i in range(16):
                if registers[f'S{i}'] != 0x0000:
                    dot_chars.append("[red]●[/red]")  # Occupied (non-zero)
                else:
                    dot_chars.append("[green]●[/green]")  # Idle (zero)
            dots = " ".join(dot_chars)
            dot_label = "[green](active)[/green]"
        else:
            dots = "[dim]○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○ ○[/dim]"
            dot_label = "[dim](off)[/dim]"

        # Color register values (red if occupied, white if idle)
        def format_reg(name, value):
            if value != 0x0000:
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

{format_reg('S0', registers['S0'])}
{format_reg('S1', registers['S1'])}
{format_reg('S2', registers['S2'])}
{format_reg('S3', registers['S3'])}
{format_reg('S4', registers['S4'])}
{format_reg('S5', registers['S5'])}
{format_reg('S6', registers['S6'])}
{format_reg('S7', registers['S7'])}
{format_reg('S8', registers['S8'])}
{format_reg('S9', registers['S9'])}
{format_reg('S10', registers['S10'])}
{format_reg('S11', registers['S11'])}
{format_reg('S12', registers['S12'])}
{format_reg('S13', registers['S13'])}  ← SP
{format_reg('S14', registers['S14'])}  ← FP
{format_reg('S15', registers['S15'])}  ← RA

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

    def update_registers(self, reg_data):
        """Update register values (for later use with simulator)"""
        # TODO: Will implement when connecting to C simulator
        pass
