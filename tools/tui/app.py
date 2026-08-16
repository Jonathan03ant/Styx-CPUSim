from textual.app import App
from textual.containers import Horizontal
from components.footer import Footer
from components.header_bar import HeaderBar
from components.control_bar import ControlBar
from components.register_panel import RegisterPanel
from components.code_panel import CodePanel, CPUStatusView, DisassemblyView
from components.memory_panel import MemoryPanel
from components.write_program_modal import WriteProgramModal
from components.filename_input_modal import FilenameInputModal
from components.load_program_modal import LoadProgramModal
from program_parser import parse_program
from styx_wrapper import StyxSimulator
import os
import asyncio

class StyxTUI(App):
    """Styx CPU Simulator TUI"""

    CSS_PATH = "styx.tcss"
    BINDINGS = [
        ("q", "quit", "Quit"),
        ("m", "focus('memory')", "Memory"),
    ]

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        # Store programs: {filename: program_text}
        self.programs = {}
        self.current_editing_filename = None
        self.powered = False
        self.sim = None  # C simulator instance

    def compose(self):
        yield HeaderBar()
        yield ControlBar()
        # Main 3-panel content
        with Horizontal(id="main-panels"):
            yield RegisterPanel(id="registers")
            yield CodePanel(id="code")
            yield MemoryPanel(id="memory")
        yield Footer()

    def on_header_bar_power_toggled(self, event):
        "Handle power button clicks"
        footer = self.query_one(Footer)
        register_panel = self.query_one(RegisterPanel)
        cpu_status = self.query_one(CPUStatusView)
        disassembly_view = self.query_one(DisassemblyView)
        memory_panel = self.query_one(MemoryPanel)

        self.powered = event.powered

        if event.powered:
            footer.log("⚡ BOOT SEQUENCE: Initializing STYX-16 CPU...")
            self.sim = StyxSimulator()
            memory_panel.set_simulator(self.sim)
            disassembly_view.set_simulator(self.sim)
            register_panel.power_on()
            cpu_status.power_on()
            disassembly_view.power_on()
            memory_panel.power_on()

            # Fetch initial state from simulator and update panels
            snapshot = self.sim.get_snapshot()
            register_panel.snapshot = snapshot
            register_panel.highlight_regs.clear()
            register_panel.render_registers()
            cpu_status.snapshot = snapshot
            cpu_status.render_status()

            footer.log("✓ SYSTEM READY | CPU: IDLE | Memory: 64KB initialized")
        else:
            footer.log("⚠ SHUTDOWN: Halting CPU...")
            # Destroy C simulator
            self.sim = None
            register_panel.power_off()
            cpu_status.power_off()
            disassembly_view.power_off()
            memory_panel.power_off()
            footer.log("✓ SYSTEM HALTED | All state cleared")

            # Clear programs on power off
            self.programs.clear()

    def on_control_bar_button_clicked(self, event: ControlBar.ButtonClicked):
        """Handle control button clicks"""
        footer = self.query_one(Footer)

        # Check if powered on
        if not self.powered:
            footer.log("Power must be ON to use controls")
            return

        if event.action == "write-prg":
            # Open Write Program modal for new file
            self.current_editing_filename = None
            self.push_screen(WriteProgramModal(), self.handle_write_program_result)
        elif event.action == "load-prg":
            # Open Load Program modal
            self.push_screen(LoadProgramModal(), self.handle_load_program_result)
        elif event.action == "step":
            # Execute one instruction
            self.run_worker(self.handle_step())
        elif event.action == "execute":
            # Run until HALT
            self.handle_execute()
        elif event.action == "reset":
            # Reset CPU
            self.handle_reset()
        else:
            # Placeholder for other buttons
            footer.log(f"{event.action} clicked (not implemented)")


    async def handle_write_program_result(self, result: tuple | None):
        """Handle result from Write Program modal"""
        if result:
            code_text, data_text = result
            footer = self.query_one(Footer)

            # Build full program text
            full_text = f"CODE:\n{code_text}\n\nDATA:\n{data_text}" if data_text else f"CODE:\n{code_text}"

            try:
                code_bytes, data_bytes = parse_program(full_text)

                if self.sim:
                    error = self.sim.load_from_bytes(code_bytes, data_bytes)

                    if error == 0:
                        footer.log(f"✓ Loaded {len(code_bytes)}B code, {len(data_bytes)}B data → memory")

                        err, stats = self.sim.get_memory_stats()
                        if err == 0:
                            memory_panel = self.query_one(MemoryPanel)
                            memory_panel.update_stats(stats)
                    else:
                        footer.log(f"✗ Load error: {error}")

            except ValueError as e:
                footer.log(f"✗ Parse error: {e}")

    def handle_load_program_result(self, file_path: str | None):
        """Handle result from Load Program modal"""
        footer = self.query_one(Footer)
        if not file_path:
            return

        try:
            # Load binary .prg file directly into simulator
            if self.sim:
                error = self.sim.load_program(file_path)
                if error == 0:
                    filename = os.path.basename(file_path)
                    footer.log(f"✓ Loaded binary: {filename} → Memory")

                    # Update memory panel stats
                    memory_panel = self.query_one(MemoryPanel)
                    err, stats = self.sim.get_memory_stats()
                    if err == 0:
                        memory_panel.update_stats(stats)

                    # Update CPU status and disassembly
                    cpu_status = self.query_one(CPUStatusView)
                    disassembly_view = self.query_one(DisassemblyView)
                    snapshot = self.sim.get_snapshot()
                    cpu_status.update_snapshot(snapshot)
                    disassembly_view.update_from_snapshot(snapshot)
                else:
                    footer.log(f"✗ Failed to load {file_path} (error {error})")
            else:
                footer.log("Error: Simulator not initialized (power on first)")
        except FileNotFoundError:
            footer.log(f"Error: File not found - {file_path}")
        except PermissionError:
            footer.log(f"Error: Permission denied - {file_path}")
        except Exception as e:
            footer.log(f"Error loading file: {str(e)}")

    async def handle_step(self):
        """Execute one instruction with animations"""
        if not self.sim:
            footer = self.query_one(Footer)
            footer.log("Error: No simulator (power on first)")
            return

        footer = self.query_one(Footer)
        cpu_status = self.query_one(CPUStatusView)
        disassembly_view = self.query_one(DisassemblyView)
        memory_panel = self.query_one(MemoryPanel)

        # Get current state BEFORE step
        snapshot_before = self.sim.get_snapshot()
        pc_before = snapshot_before.pc

        # Set persistent PC marker in memory panel
        memory_panel.current_pc = pc_before
        memory_panel.render_memory()

        # 1. FETCH PHASE - Flash PC (reading address)
        footer.log(f"→ FETCH: Reading PC=0x{pc_before:04X}")
        cpu_status.flash_pc(True)
        await asyncio.sleep(0.6)
        cpu_status.flash_pc(False)

        # 2. MEMORY ACCESS - Auto-switch view and flash address
        footer.log(f"→ FETCH: Accessing memory at 0x{pc_before:04X}")
        memory_panel.auto_switch_to_address(pc_before)
        memory_panel.flash_address(pc_before)
        await asyncio.sleep(0.6)
        memory_panel.flash_address(None)

        # 3. EXECUTE - Run the instruction
        error = self.sim.step()

        if error != 0:
            footer.log(f"✗ Step failed (error {error})")
            cpu_status.flash_pc(False)
            memory_panel.flash_address(None)
            return

        # 4. Get updated CPU state AFTER step
        snapshot_after = self.sim.get_snapshot()

        # 5. DECODE PHASE - Flash IR
        footer.log(f"→ DECODE: IR=0x{snapshot_after.ir:04X}")
        cpu_status.update_snapshot(snapshot_after)
        cpu_status.flash_ir(True)
        await asyncio.sleep(0.6)
        cpu_status.flash_ir(False)

        # 6. Update disassembly (show execution)
        disassembly_view.update_from_snapshot(snapshot_after)
        await asyncio.sleep(0.3)

        # 7. MEMORY WRITE/READ - If this is LOAD/STORE, jump to MAR address
        # Extract opcode from IR (top 4 bits)
        opcode = (snapshot_after.ir >> 12) & 0xF
        is_load_store = (opcode == 0x2 or opcode == 0x3)  # LOAD=0x2, STORE=0x3

        if is_load_store and snapshot_after.mar != 0x0000:
            memory_panel.auto_switch_to_address(snapshot_after.mar)
            memory_panel.flash_address(snapshot_after.mar)
            await asyncio.sleep(0.6)
            memory_panel.flash_address(None)

        # 8. EXECUTE - Update registers (flash changed ones)
        footer.log(f"→ EXECUTE: Updating registers...")
        register_panel = self.query_one(RegisterPanel)
        register_panel.update_from_snapshot(snapshot_before, snapshot_after)
        await asyncio.sleep(0.6)

        # 9. COMPLETE - Flash cycle counter
        cpu_status.flash_cycle(True)
        footer.log(f"✓ COMPLETE: PC=0x{snapshot_after.pc:04X} | Cycle={snapshot_after.cycle_count}")
        await asyncio.sleep(0.6)
        cpu_status.flash_cycle(False)

    def handle_execute(self):
        """Run until HALT"""
        if not self.sim:
            footer = self.query_one(Footer)
            footer.log("Error: No simulator (power on first)")
            return

        footer = self.query_one(Footer)
        error = self.sim.run()

        if error != 0:
            footer.log(f"✗ Execute failed (error {error})")
        else:
            footer.log("✓ Program execution complete")

        # Update displays
        self.handle_step()  # Reuse step logic to update UI

    def handle_reset(self):
        """Reset CPU to initial state"""
        if not self.sim:
            footer = self.query_one(Footer)
            footer.log("Error: No simulator (power on first)")
            return

        footer = self.query_one(Footer)
        error = self.sim.reset()

        if error != 0:
            footer.log(f"✗ Reset failed (error {error})")
        else:
            # Update all panels
            cpu_status = self.query_one(CPUStatusView)
            disassembly_view = self.query_one(DisassemblyView)
            register_panel = self.query_one(RegisterPanel)
            memory_panel = self.query_one(MemoryPanel)

            # Clear executed instructions
            disassembly_view.executed_addrs.clear()
            disassembly_view.pc = 0x0100
            disassembly_view.last_instruction = ""

            # Get fresh snapshot
            snapshot = self.sim.get_snapshot()
            cpu_status.update_snapshot(snapshot)
            disassembly_view.update_from_snapshot(snapshot)

            # Reset register panel (clear highlights, update values)
            register_panel.snapshot = snapshot
            register_panel.highlight_regs.clear()
            register_panel.render_registers()

            # Reset memory panel (clear highlights, reset PC marker)
            memory_panel.highlight_addr = None
            memory_panel.current_pc = 0x0100
            memory_panel.render_memory()

            footer.log("✓ CPU Reset | PC=0x0100 | State=LOADED")


if __name__ == "__main__":
    app = StyxTUI()
    app.run()