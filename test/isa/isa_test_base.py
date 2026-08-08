#!/usr/bin/env python3
"""
Base class for all ISA instruction tests

Usage:
    from isa_test_base import ISATestBase

    class TestADDI(ISATestBase):
        def __init__(self):
            super().__init__(
                name="ADDI",
                opcode=0x8,
                format="I-Type A",
                description="Add Immediate"
            )

        def build_test(self):
            # Test 1: Basic
            self.add_instruction(self.encode_itype_a(0x8, 0x1, 5))
            # ... more tests
            self.add_halt()

        def get_expected_results(self):
            return {
                'S1': 0x000F,
                'S2': 0x0005,
                'cycles': 7
            }
"""

import struct
from pathlib import Path
from abc import ABC, abstractmethod

# ============================================================================
# COLOR OUTPUT
# ============================================================================

# Colors removed for simplicity

# ============================================================================
# OPCODE DEFINITIONS
# ============================================================================

class Opcodes:
    """Styx ISA Opcode definitions"""
    NOP   = 0x0
    MOV   = 0x1
    LOAD  = 0x2
    STORE = 0x3
    ADD   = 0x4
    SUB   = 0x5
    MUL   = 0x6
    DIV   = 0x7
    ADDI  = 0x8
    CMP   = 0x9
    JMP   = 0xA
    JZ    = 0xB
    JNZ   = 0xC
    CALL  = 0xD
    RET   = 0xE
    LUI   = 0xF

# ============================================================================
# INSTRUCTION ENCODERS
# ============================================================================

class InstructionEncoder:
    @staticmethod
    def encode_rtype(opcode, rd, rs1, rs2):
        """
        Encode R-Type instruction: [opcode:4][rd:4][rs1:4][rs2:4]
        Used by: ADD, SUB, MUL, DIV, MOV, CMP
        """
        instr = (opcode << 12) | (rd << 8) | (rs1 << 4) | (rs2 & 0xF)
        return struct.pack('<H', instr)

    @staticmethod
    def encode_itype_a(opcode, rd, imm8):
        """
        Encode I-Type A instruction: [opcode:4][rd:4][imm8:8]
        Used by: ADDI, LUI
        """
        instr = (opcode << 12) | (rd << 8) | (imm8 & 0xFF)
        return struct.pack('<H', instr)

    @staticmethod
    def encode_itype_b(opcode, rd, rs, offset4):
        """
        Encode I-Type B instruction: [opcode:4][rd:4][rs:4][offset:4]
        Used by: LOAD, STORE
        """
        instr = (opcode << 12) | (rd << 8) | (rs << 4) | (offset4 & 0xF)
        return struct.pack('<H', instr)

    @staticmethod
    def encode_jtype(opcode, addr12):
        """
        Encode J-Type instruction: [opcode:4][addr12:12]
        Used by: JMP, JZ, JNZ, CALL, RET
        """
        instr = (opcode << 12) | (addr12 & 0xFFF)
        return struct.pack('<H', instr)

# ============================================================================
# BASE TEST CLASS
# ============================================================================

class ISATestBase(ABC, InstructionEncoder):
    """
    Abstract base class for ISA instruction tests
    """

    def __init__(self, name, opcode, format, description):
        """
        Initialize test
        """
        self.name = name
        self.opcode = opcode
        self.format = format
        self.description = description
        self.code = b''
        self.entry_point = 0x0100

    @abstractmethod
    def build_test(self):
        pass

    @abstractmethod
    def get_expected_results(self):
        pass

    def add_instruction(self, encoded_instr):
        self.code += encoded_instr

    def add_halt(self, addr=None):
        if addr is None:
            addr = self.entry_point + len(self.code)
        self.code += self.encode_jtype(Opcodes.JMP, addr)

    def create_prg_header(self):
        """Create .prg file header (16 bytes)"""
        return struct.pack(
            '<4sBBHHHHH',
            b'STYX',              # Magic (4 bytes)
            1,                    # Version (uint8_t)
            0,                    # Flags (uint8_t)
            self.entry_point,     # Entry point (uint16_t)
            len(self.code),       # Code size (uint16_t)
            0,                    # Data size (uint16_t)
            0,                    # BSS size (uint16_t)
            0                     # Reserved (uint16_t)
        )

    def generate_prg_file(self):
        """Generate the .prg file"""
        self.build_test()

        output_file = Path(__file__).parent / f'test_{self.name.lower()}.prg'

        with open(output_file, 'wb') as f:
            f.write(self.create_prg_header() + self.code)

        # Print summary (compact)
        print(f"[OK] {self.name} test generated ({len(self.code)//2} instructions)")

        # Output expected results in parseable format
        expected = self.get_expected_results()
        print("EXPECTED:")
        for reg, value in expected.items():
            print(f"  {reg}={value}")

        return output_file

def run_test(test_class):
    test = test_class()
    test.generate_prg_file()
