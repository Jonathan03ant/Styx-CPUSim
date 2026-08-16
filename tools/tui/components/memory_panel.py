from textual.widgets import Static, Button
from textual.containers import Container, Vertical, Horizontal, VerticalScroll
from textual.app import ComposeResult


class MemoryPanel(Container):
    """Memory panel - displays memory regions"""

    BINDINGS = [
        ("1", "zoom('code')", "CODE"),
        ("2", "zoom('data')", "DATA"),
        ("3", "zoom('heap')", "HEAP"),
        ("4", "zoom('stack')", "STACK"),
        ("escape", "back", "Back"),
    ]

    can_focus = True

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.powered = False
        self.view_mode = "overview"
        self.zoomed_region = None
        self.stats = None
        self.sim = None
        self.mem_scroll_offset = 0
        self.highlight_addr = None  # Address to flash (temporary)
        self.current_pc = None  # Current PC (persistent marker)

    def compose(self) -> ComposeResult:
        yield Static("", id="memory-header")
        with VerticalScroll(id="memory-scroll"):
            yield Static("", id="memory-content")

    def on_mount(self):
        self.render_memory()

    def set_simulator(self, sim):
        self.sim = sim

    def update_stats(self, stats):
        self.stats = stats
        self.render_memory()

    def render_memory(self):
        if not self.powered:
            header = self.query_one("#memory-header", Static)
            content = self.query_one("#memory-content", Static)

            header.update("[bold cyan]MEMORY REGIONS[/bold cyan]")
            content.update("""[yellow]⚡[/yellow] SYSTEM OFFLINE [yellow]⚡[/yellow]

[dim]Code (0x0100-1FFF): [░░░░░░░░░░] ---
Data (0x2000-7FFF): [░░░░░░░░░░] ---
Heap (0x8000-DFFF): [░░░░░░░░░░] ---
Stack (0xE000-FFFF): [░░░░░░░░░░] ---[/dim]

[dim italic]Memory uninitialized[/dim italic]""")
        elif self.view_mode == "overview":
            self._render_overview()
        else:
            self._render_zoomed()

    def _render_overview(self):
        header = self.query_one("#memory-header", Static)
        content = self.query_one("#memory-content", Static)

        if not self.stats:
            code_used = 0
            data_used = 0
            heap_used = 0
            stack_used = 0
        else:
            code_used = self.stats.code_used
            data_used = self.stats.data_used
            heap_used = self.stats.heap_used
            stack_used = self.stats.stack_used

        code_max = 7936
        data_max = 24576
        heap_max = 24576
        stack_max = 8192

        code_pct = (code_used / code_max * 100) if code_max > 0 else 0
        data_pct = (data_used / data_max * 100) if data_max > 0 else 0
        heap_pct = (heap_used / heap_max * 100) if heap_max > 0 else 0
        stack_pct = (stack_used / stack_max * 100) if stack_max > 0 else 0

        code_bar = self._make_bar(code_pct)
        data_bar = self._make_bar(data_pct)
        heap_bar = self._make_bar(heap_pct)
        stack_bar = self._make_bar(stack_pct)

        header.update("[bold cyan]MEMORY REGIONS[/bold cyan]\n[bold green]✓[/bold green] 64KB UNIFIED")

        content.update(f"""[yellow]1[/yellow] CODE (0x0100)
{code_bar} [cyan]{code_pct:.1f}%[/cyan] [dim]{code_used}B[/dim]

[yellow]2[/yellow] DATA (0x2000)
{data_bar} [cyan]{data_pct:.1f}%[/cyan] [dim]{data_used}B[/dim]

[yellow]3[/yellow] HEAP (0x8000)
{heap_bar} [cyan]{heap_pct:.1f}%[/cyan] [dim]{heap_used}B[/dim]

[yellow]4[/yellow] STACK (0xE000)
{stack_bar} [cyan]{stack_pct:.1f}%[/cyan] [dim]{stack_used}B[/dim]

[dim italic]Press 1-4 to inspect[/dim italic]""")

    def _make_bar(self, percentage):
        bar_length = 10
        filled = int(percentage / 10)
        bar = "█" * filled + "░" * (bar_length - filled)
        return f"[dim][{bar}][/dim]"

    def _update_app_footer(self, msg):
        from components.footer import Footer
        try:
            footer = self.app.query_one(Footer)
            footer.set_mem_info(msg)
        except:
            pass

    def _render_zoomed(self):
        if not self.zoomed_region or not self.sim:
            return self._render_overview()

        header = self.query_one("#memory-header", Static)
        content = self.query_one("#memory-content", Static)

        region_map = {
            "code": (0x0100, 0x1FFF, "CODE"),
            "data": (0x2000, 0x7FFF, "DATA"),
            "heap": (0x8000, 0xDFFF, "HEAP"),
            "stack": (0xE000, 0xFFFF, "STACK"),
        }

        if self.zoomed_region not in region_map:
            return self._render_overview()

        start_addr, end_addr, region_name = region_map[self.zoomed_region]

        used_bytes = 0
        if self.stats:
            if self.zoomed_region == "code":
                used_bytes = self.stats.code_used
            elif self.zoomed_region == "data":
                used_bytes = self.stats.data_used
            elif self.zoomed_region == "heap":
                used_bytes = self.stats.heap_used
            elif self.zoomed_region == "stack":
                used_bytes = self.stats.stack_used

        header.update(f"[bold cyan]{region_name}[/bold cyan] [dim]0x{start_addr:04X}-0x{end_addr:04X} | {used_bytes}B used | ESC=back[/dim]")

        words_per_row = 10
        bytes_per_row = words_per_row * 2

        lines = []
        current_addr = start_addr

        while current_addr <= end_addr:
            hex_bytes = []
            for i in range(0, bytes_per_row, 2):
                if current_addr + i > end_addr:
                    break
                addr_at_position = current_addr + i
                err, value = self.sim.read_memory(addr_at_position)

                # Check if this address should be highlighted
                is_flash = (self.highlight_addr is not None and addr_at_position == self.highlight_addr)
                is_current_pc = (self.current_pc is not None and addr_at_position == self.current_pc)

                if err == 0:
                    if is_flash:
                        # Flash highlight (temporary, red background)
                        hex_bytes.append(f"[black on red]{value:04X}[/black on red]")
                    elif is_current_pc:
                        # Current PC marker (persistent, yellow background)
                        hex_bytes.append(f"[black on yellow]{value:04X}[/black on yellow]")
                    elif value == 0:
                        hex_bytes.append(f"[dim]{value:04X}[/dim]")
                    else:
                        hex_bytes.append(f"[green]{value:04X}[/green]")
                else:
                    hex_bytes.append("[red]????[/red]")

            if hex_bytes:
                line = f"[cyan]0x{current_addr:04X}:[/cyan] {' '.join(hex_bytes)}"
                lines.append(line)

            current_addr += bytes_per_row

        content.update("\n".join(lines))

        region_size = end_addr - start_addr + 1
        self._update_app_footer(f"{region_name}: {region_size}B")

    def power_on(self):
        self.powered = True
        self.view_mode = "overview"
        self.zoomed_region = None
        self.mem_scroll_offset = 0
        self.render_memory()

    def power_off(self):
        self.powered = False
        self.view_mode = "overview"
        self.zoomed_region = None
        self.stats = None
        self.mem_scroll_offset = 0
        self.render_memory()

    def action_zoom(self, region: str):
        if not self.powered or self.view_mode != "overview":
            return
        self.view_mode = "zoomed"
        self.zoomed_region = region
        self.mem_scroll_offset = 0
        self.render_memory()

        try:
            scroll = self.query_one("#memory-scroll", VerticalScroll)
            scroll.scroll_home(animate=False)
        except:
            pass

    def action_back(self):
        if self.view_mode != "zoomed":
            return
        self.view_mode = "overview"
        self.zoomed_region = None
        self.mem_scroll_offset = 0
        self._update_app_footer("Ready")
        self.render_memory()

        try:
            scroll = self.query_one("#memory-scroll", VerticalScroll)
            scroll.scroll_home(animate=False)
        except:
            pass

    def flash_address(self, address: int | None):
        """Highlight a specific address (or clear if None)"""
        self.highlight_addr = address
        if self.view_mode == "zoomed":
            self.render_memory()

    def auto_switch_to_address(self, address: int):
        """Auto-switch memory view to show the region containing this address"""
        if not self.powered:
            return

        # Determine which region this address belongs to
        region = None
        if 0x0100 <= address <= 0x1FFF:
            region = "code"
        elif 0x2000 <= address <= 0x7FFF:
            region = "data"
        elif 0x8000 <= address <= 0xDFFF:
            region = "heap"
        elif 0xE000 <= address <= 0xFFFF:
            region = "stack"

        if not region:
            return

        # If already in the right zoomed view, do nothing
        if self.view_mode == "zoomed" and self.zoomed_region == region:
            return

        # Switch to the correct region
        self.view_mode = "zoomed"
        self.zoomed_region = region
        self.mem_scroll_offset = 0
        self.render_memory()

        try:
            scroll = self.query_one("#memory-scroll", VerticalScroll)
            scroll.scroll_home(animate=False)
        except:
            pass

    def action_scroll_down(self):
        if self.view_mode != "zoomed":
            return

        region_map = {
            "code": (0x0100, 0x1FFF),
            "data": (0x2000, 0x7FFF),
            "heap": (0x8000, 0xDFFF),
            "stack": (0xE000, 0xFFFF),
        }

        if self.zoomed_region in region_map:
            start_addr, end_addr = region_map[self.zoomed_region]
            region_size = end_addr - start_addr + 1
            words_per_row = 10
            rows_to_show = 39
            bytes_per_view = rows_to_show * words_per_row * 2

            max_offset = max(0, (region_size - bytes_per_view) // (words_per_row * 2))
            self.mem_scroll_offset = min(self.mem_scroll_offset + 5, max_offset)

        self.render_memory()

    def action_scroll_up(self):
        if self.view_mode != "zoomed":
            return
        self.mem_scroll_offset = max(0, self.mem_scroll_offset - 5)
        self.render_memory()
