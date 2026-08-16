"""
Styx Simulator - Python ctypes wrapper

Wraps the C Styx API (libstyx.so) for use in Python TUI.
Provides clean Python interface to simulator functions.
"""

import ctypes
from pathlib import Path

# Load the shared library
LIB_PATH = Path(__file__).parent.parent.parent / "lib" / "libstyx.so"
lib = ctypes.CDLL(str(LIB_PATH))

# Basic types (from types.h)
addr_t = ctypes.c_uint16
word_t = ctypes.c_uint16
regval_t = ctypes.c_uint16
flags_t = ctypes.c_uint16
error_t = ctypes.c_int

class Simulator_t(ctypes.Structure):
    pass
SimulatorPtr = ctypes.POINTER(Simulator_t)

# CPUSnapshot_t struct (from types.h)
class CPUSnapshot(ctypes.Structure):
    _fields_ = [
        ("regs", regval_t * 16),      # 16 registers
        ("pc", addr_t),               # Program Counter
        ("flags", flags_t),           # FLAGS register
        ("mar", addr_t),              # Memory Address Register
        ("mdr", word_t),              # Memory Data Register
        ("ir", word_t),               # Instruction Register
        ("state", ctypes.c_int),      # CPU state
        ("cycle_count", ctypes.c_uint32),  # Cycle count
        ("last_error", ctypes.c_int), # Last error
    ]

# MemoryStats_t struct (from types.h)
class MemoryStats(ctypes.Structure):
    _fields_ = [
        ("code_used", ctypes.c_uint16),
        ("data_used", ctypes.c_uint16),
        ("heap_used", ctypes.c_uint16),
        ("stack_used", ctypes.c_uint16),
    ]

# ============================================================================
# C FUNCTION SIGNATURES
# ============================================================================

# Lifecycle
lib.styx_create.argtypes = []
lib.styx_create.restype = SimulatorPtr

lib.styx_destroy.argtypes = [SimulatorPtr]
lib.styx_destroy.restype = None

# Program management
lib.styx_load_program.argtypes = [SimulatorPtr, ctypes.c_char_p]
lib.styx_load_program.restype = error_t
# styx_load_from_bytes(sim, code_bytes, code_size, data_bytes, data_size)
lib.styx_load_from_bytes.argtypes = [
    SimulatorPtr,
    ctypes.POINTER(ctypes.c_uint8),
    ctypes.c_size_t,
    ctypes.POINTER(ctypes.c_uint8),
    ctypes.c_size_t
]
lib.styx_load_from_bytes.restype = error_t

lib.styx_reset.argtypes = [SimulatorPtr]
lib.styx_reset.restype = error_t

# Execution
lib.styx_step.argtypes = [SimulatorPtr]
lib.styx_step.restype = error_t

lib.styx_run.argtypes = [SimulatorPtr]
lib.styx_run.restype = error_t

# State inspection
lib.styx_get_snapshot.argtypes = [SimulatorPtr]
lib.styx_get_snapshot.restype = CPUSnapshot

lib.styx_read_memory.argtypes = [SimulatorPtr, addr_t, ctypes.POINTER(word_t)]
lib.styx_read_memory.restype = error_t

lib.styx_get_memory_stats.argtypes = [SimulatorPtr, ctypes.POINTER(MemoryStats)]
lib.styx_get_memory_stats.restype = error_t

lib.styx_disassemble.argtypes = [SimulatorPtr, addr_t, ctypes.c_char_p, ctypes.c_size_t]
lib.styx_disassemble.restype = error_t

# ============================================================================
# PYTHON WRAPPER CLASS
# ============================================================================

class StyxSimulator:
    """High-level Python wrapper for Styx simulator"""

    def __init__(self):
        """Create a new simulator instance"""
        self.sim = lib.styx_create()
        if not self.sim:
            raise RuntimeError("Failed to create simulator")

    def __del__(self):
        """Cleanup simulator on object destruction"""
        if hasattr(self, 'sim') and self.sim:
            lib.styx_destroy(self.sim)

    def load_program(self, filepath: str) -> int:
        """Load a .prg file into memory"""
        return lib.styx_load_program(self.sim, filepath.encode('utf-8'))

    def load_from_bytes(self, code_bytes: bytes, data_bytes: bytes = None) -> int:
        """
        Load program from memory buffers (no file I/O).
        """
        if not self.sim:
            return -1

        # Convert CODE bytes to C array
        code_array = (ctypes.c_uint8 * len(code_bytes))(*code_bytes)

        # Convert DATA bytes if provided
        if data_bytes:
            data_array = (ctypes.c_uint8 * len(data_bytes))(*data_bytes)
            data_size = len(data_bytes)
        else:
            data_array = None
            data_size = 0

        return lib.styx_load_from_bytes(
            self.sim,
            code_array, len(code_bytes),
            data_array, data_size
        )

    def reset(self) -> int:
        """Reset CPU to initial state"""
        return lib.styx_reset(self.sim)

    def step(self) -> int:
        """Execute one instruction"""
        return lib.styx_step(self.sim)

    def run(self) -> int:
        """Run until HALT or error"""
        return lib.styx_run(self.sim)

    def get_snapshot(self) -> CPUSnapshot:
        """Get current CPU state snapshot"""
        return lib.styx_get_snapshot(self.sim)

    def read_memory(self, address: int) -> tuple[int, int]:
        """Read word from memory at address. Returns (error_code, value)"""
        value = word_t()
        err = lib.styx_read_memory(self.sim, address, ctypes.byref(value))
        return err, value.value

    def get_memory_stats(self) -> tuple[int, MemoryStats]:
        """Get memory usage statistics. Returns (error_code, stats)"""
        stats = MemoryStats()
        err = lib.styx_get_memory_stats(self.sim, ctypes.byref(stats))
        return err, stats

    def disassemble(self, address: int, buf_size: int = 64) -> tuple[int, str]:
        """Disassemble instruction at address. Returns (error_code, asm_text)"""
        buffer = ctypes.create_string_buffer(buf_size)
        err = lib.styx_disassemble(self.sim, address, buffer, buf_size)
        return err, buffer.value.decode('utf-8')