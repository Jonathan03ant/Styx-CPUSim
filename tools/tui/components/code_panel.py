from textual.widgets import Static
from textual.containers import Container, Vertical
from textual.reactive import reactive


class CPUStatusView(Static):
    """CPU status display - shows state, PC, cycle, internal registers, FLAGS"""

    def __init__(self, **kwargs):
        super().__init__("", **kwargs)
        self.powered = False
        self.snapshot = None
        self.highlight_pc = False
        self.highlight_ir = False
        self.highlight_cycle = False
        self.render_status()

    def render_status(self):
        if not self.powered:
            content = """[bold cyan]CPU STATUS[/bold cyan]
─────────────────────
[dim italic]System Offline[/dim italic]
"""
        elif not self.snapshot:
            content = """[bold cyan]═══ CPU STATUS ═══[/bold cyan]              [dim]Status:[/dim] [yellow]IDLE[/yellow]

       [dim]PC:[/dim] 0x0100 [yellow]◄[/yellow]
       [dim]Cycle:[/dim] 0
       [dim]IR:[/dim] 0x0000

[dim]─── Internal ───[/dim]
       [dim]MAR:[/dim] 0x0000
       [dim]MDR:[/dim] 0x0000

[dim]──── FLAGS ────[/dim]
    [cyan]Z N C V[/cyan]
    ○ ○ ○ ○
"""
        else:
            # Render with actual snapshot data
            state_colors = {
                0: "[yellow]IDLE[/yellow]",
                1: "[green]LOADED[/green]",
                2: "[bold green]RUNNING[/bold green]",
                3: "[red]HALTED[/red]",
                4: "[bold red]ERROR[/bold red]"
            }
            state_str = state_colors.get(self.snapshot.state, "[dim]UNKNOWN[/dim]")

            # FLAGS indicators
            z = "●" if self.snapshot.flags & 0x01 else "○"
            n = "●" if self.snapshot.flags & 0x02 else "○"
            c = "●" if self.snapshot.flags & 0x04 else "○"
            v = "●" if self.snapshot.flags & 0x08 else "○"

            # Highlight animations
            pc_str = f"[black on red]0x{self.snapshot.pc:04X}[/black on red]" if self.highlight_pc else f"0x{self.snapshot.pc:04X}"
            ir_str = f"[black on yellow]0x{self.snapshot.ir:04X}[/black on yellow]" if self.highlight_ir else f"0x{self.snapshot.ir:04X}"
            cycle_str = f"[black on green]{self.snapshot.cycle_count}[/black on green]" if self.highlight_cycle else f"{self.snapshot.cycle_count}"

            content = f"""[bold cyan]═══ CPU STATUS ═══[/bold cyan]              [dim]Status:[/dim] {state_str}

       [dim]PC:[/dim] {pc_str} [yellow]◄[/yellow]
       [dim]Cycle:[/dim] {cycle_str}
       [dim]IR:[/dim] {ir_str}

[dim]─── Internal ───[/dim]
       [dim]MAR:[/dim] 0x{self.snapshot.mar:04X}
       [dim]MDR:[/dim] 0x{self.snapshot.mdr:04X}

[dim]──── FLAGS ────[/dim]
    [cyan]Z N C V[/cyan]
    {z} {n} {c} {v}
"""
        self.update(content)

    def power_on(self):
        self.powered = True
        self.snapshot = None
        self.render_status()

    def power_off(self):
        self.powered = False
        self.snapshot = None
        self.render_status()

    def update_snapshot(self, snapshot):
        """Update with new CPU snapshot"""
        self.snapshot = snapshot
        self.render_status()

    def flash_pc(self, enable=True):
        """Flash PC highlight"""
        self.highlight_pc = enable
        self.render_status()

    def flash_ir(self, enable=True):
        """Flash IR highlight"""
        self.highlight_ir = enable
        self.render_status()

    def flash_cycle(self, enable=True):
        """Flash cycle highlight"""
        self.highlight_cycle = enable
        self.render_status()


class DisassemblyView(Static):
    """Disassembly view - shows instructions around PC with animations"""

    def __init__(self, **kwargs):
        super().__init__("", **kwargs)
        self.powered = False
        self.sim = None
        self.pc = 0x0100
        self.executed_addrs = set()  # Track executed instructions
        self.last_instruction = ""
        self.render_disassembly()

    def set_simulator(self, sim):
        """Set the simulator instance"""
        self.sim = sim

    def render_disassembly(self):
        if not self.powered:
            content = """[bold cyan]DISASSEMBLY[/bold cyan]
═════════════════════

[dim italic]Awaiting program load...[/dim italic]
"""
        elif not self.sim:
            content = """[bold cyan]DISASSEMBLY[/bold cyan]
═════════════════════

[yellow]→[/yellow] 0x0100: [dim](awaiting program)[/dim]
  0x0102:
  0x0104:
  0x0106:

─────────────────────
[dim]Ready to execute[/dim]
"""
        else:
            # Show instructions around PC
            lines = []
            lines.append("[bold cyan]DISASSEMBLY[/bold cyan]")
            lines.append("═════════════════════")
            lines.append("")

            # Show 5 instructions around PC (2 before, current, 2 after)
            start_addr = max(0x0100, self.pc - 4)

            for i in range(7):  # Show 7 instructions
                addr = start_addr + (i * 2)

                if addr >= 0x10000:  # Beyond memory
                    break

                # Get disassembly
                try:
                    err, asm_text = self.sim.disassemble(addr)
                    if err != 0:
                        asm_text = "???"
                except:
                    asm_text = "???"

                # Format line
                if addr == self.pc:
                    # Current instruction - highlighted
                    line = f"[bold yellow]▶[/bold yellow] [bold green on #1a1a1a]0x{addr:04X}: {asm_text:<20}[/bold green on #1a1a1a]"
                elif addr in self.executed_addrs:
                    # Already executed - dim with checkmark
                    line = f"  [dim]0x{addr:04X}: {asm_text:<20}[/dim] [green]✓[/green]"
                else:
                    # Not executed yet
                    line = f"  0x{addr:04X}: {asm_text:<20}"

                lines.append(line)

            lines.append("")
            lines.append("─────────────────────")
            if self.last_instruction:
                lines.append(f"[dim]Last:[/dim] {self.last_instruction}")
            else:
                lines.append("[dim]No instructions executed[/dim]")

            content = "\n".join(lines)

        self.update(content)

    def power_on(self):
        self.powered = True
        self.pc = 0x0100
        self.executed_addrs.clear()
        self.last_instruction = ""
        self.render_disassembly()

    def power_off(self):
        self.powered = False
        self.sim = None
        self.pc = 0x0100
        self.executed_addrs.clear()
        self.last_instruction = ""
        self.render_disassembly()

    def update_from_snapshot(self, snapshot):
        """Update display from CPU snapshot"""
        if snapshot:
            # Mark current PC as executed (before it moves)
            if self.pc != snapshot.pc and self.powered:
                self.executed_addrs.add(self.pc)

            # Update PC
            self.pc = snapshot.pc

            # Try to get last instruction
            if self.sim:
                try:
                    # Get instruction at previous PC (the one just executed)
                    prev_pc = snapshot.pc - 2 if snapshot.pc > 0x0100 else 0x0100
                    err, asm = self.sim.disassemble(prev_pc)
                    if err == 0:
                        self.last_instruction = asm
                except:
                    pass

            self.render_disassembly()


class CodePanel(Vertical):
    """Code panel - CPU status + disassembly view"""

    def compose(self):
        yield CPUStatusView(id="cpu-status")
        yield DisassemblyView(id="disassembly-view")
