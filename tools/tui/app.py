from textual.app import App
from textual.containers import Horizontal
from components.footer import Footer
from components.header_bar import HeaderBar
from components.control_bar import ControlBar
from components.register_panel import RegisterPanel
from components.code_panel import CodePanel, FileList, FileItem
from components.memory_panel import MemoryPanel
from components.write_program_modal import WriteProgramModal
from components.filename_input_modal import FilenameInputModal

class StyxTUI(App):
    """Styx CPU Simulator TUI"""

    CSS_PATH = "styx.tcss"

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        # Store programs: {filename: program_text}
        self.programs = {}
        self.current_editing_filename = None
        self.powered = False

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
        file_list = self.query_one(FileList)

        self.powered = event.powered

        if event.powered:
            footer.log("DEBUG: Power turned ON")
            register_panel.power_on()
        else:
            footer.log("DEBUG: Power turned OFF")
            register_panel.power_off()

            # Clear all files on power off (fresh start next time)
            self.programs.clear()
            file_list.files = []
            file_list.selected_file = None

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
        elif event.action == "delete":
            # Delete selected file
            self.delete_selected_file()
        else:
            # Placeholder for other buttons
            footer.log(f"DEBUG: {event.action} clicked (not implemented)")

    def on_file_item_selected(self, event: FileItem.Selected):
        """Handle file selection (single-click)"""
        if not self.powered:
            return

        file_list = self.query_one(FileList)
        file_list.selected_file = event.filename

        footer = self.query_one(Footer)
        footer.log(f"Selected: {event.filename}")

    def on_file_item_double_clicked(self, event: FileItem.DoubleClicked):
        """Handle file double-click (edit)"""
        if not self.powered:
            return

        if event.filename in self.programs:
            # Open editor with existing program text
            self.current_editing_filename = event.filename
            existing_text = self.programs[event.filename]
            self.push_screen(WriteProgramModal(initial_text=existing_text), self.handle_write_program_result)

    def delete_selected_file(self):
        """Delete the currently selected file"""
        file_list = self.query_one(FileList)
        selected = file_list.selected_file
        footer = self.query_one(Footer)

        if selected and selected in self.programs:
            # Remove from programs dict
            # Update file list
            # Log to footer
            del self.programs[selected]
            file_list.files = list(self.programs.keys())
            file_list.selected_file = None
            footer.log(f"Deleted: {selected}")
        else:
            footer.log("No file selected to delete")

    async def handle_write_program_result(self, program_text: str | None):
        """Handle result from Write Program modal"""
        if program_text:
            if self.current_editing_filename:
                # Editing existing file - save directly
                self.save_program(program_text, self.current_editing_filename)
            else:
                # New file - ask for filename
                self.push_screen(FilenameInputModal(), lambda filename: self.save_program(program_text, filename))

    def save_program(self, program_text: str, filename: str | None):
        """Save program to file list"""
        if filename:
            # Save program
            # Update file list in Code Panel
            # Log to footer
            # Reset editing state
            self.programs[filename] = program_text
            file_list = self.query_one(FileList)
            file_list.files = list(self.programs.keys())
            footer = self.query_one(Footer)
            action = "updated" if self.current_editing_filename else "saved"
            footer.log(f"Program {action}: {filename}")
            self.current_editing_filename = None


if __name__ == "__main__":
    app = StyxTUI()
    app.run()