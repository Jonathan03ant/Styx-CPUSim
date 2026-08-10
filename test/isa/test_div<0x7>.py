#!/usr/bin/env python3
"""
DIV Instruction Test
Opcode: 0x7
Format: R-Type
Encoding: [opcode:4][rd:4][rs1:4][rs2:4]
Operation: Rd = rs1 / rs2 (integer division)
"""

from pathlib import Path
import sys
sys.path.append(str(Path(__file__).parent.parent))

from isa_test_base import ISATestBase, Opcodes


class TestDIV(ISATestBase):
    def __init__(self):
        super().__init__(
            name="DIV",
            opcode=Opcodes.DIV,
            format="R-Type",
            description="DIV - Rd = rs1 / rs2 (quotient only)"
        )

    def build_test(self):
        # Test 1: Basic division (20 / 4 = 5)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x1, 20))   # ADDI S1, 20
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 4))    # ADDI S2, 4
        self.add_instruction(self.encode_rtype(Opcodes.DIV, 0x3, 0x1, 0x2)) # DIV S3, S1, S2

        # Test 2: Division with remainder (17 / 5 = 3, remainder discarded)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x4, 17))   # ADDI S4, 17
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x5, 5))    # ADDI S5, 5
        self.add_instruction(self.encode_rtype(Opcodes.DIV, 0x6, 0x4, 0x5)) # DIV S6, S4, S5

        # Test 3: Divide by one (15 / 1 = 15)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x7, 15))   # ADDI S7, 15
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x8, 1))    # ADDI S8, 1
        self.add_instruction(self.encode_rtype(Opcodes.DIV, 0x9, 0x7, 0x8)) # DIV S9, S7, S8

        # Test 4: Divide zero (0 / 5 = 0)
        self.add_instruction(self.encode_rtype(Opcodes.DIV, 0xA, 0x0, 0x5)) # DIV S10, S0, S5

        # Test 5: Division by zero (10 / 0 = 0, no crash)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0xB, 10))   # ADDI S11, 10
        self.add_instruction(self.encode_rtype(Opcodes.DIV, 0xC, 0xB, 0x0)) # DIV S12, S11, S0

        # Test 6: S0 hardwired to zero (attempt to write result to S0)
        self.add_instruction(self.encode_rtype(Opcodes.DIV, 0x0, 0x1, 0x2)) # DIV S0, S1, S2

        self.add_halt()

    def get_expected_results(self):
        return {
            'S0': 0x0000,  # Always zero (hardwired)
            'S1': 0x0014,  # 20
            'S2': 0x0004,  # 4
            'S3': 0x0005,  # 20 / 4 = 5
            'S4': 0x0011,  # 17
            'S5': 0x0005,  # 5
            'S6': 0x0003,  # 17 / 5 = 3 (remainder discarded)
            'S7': 0x000F,  # 15
            'S8': 0x0001,  # 1
            'S9': 0x000F,  # 15 / 1 = 15
            'S10': 0x0000, # 0 / 5 = 0
            'S11': 0x000A, # 10
            'S12': 0x0000, # 10 / 0 = 0 (division by zero returns 0)
            'cycles': 14
        }


if __name__ == '__main__':
    test = TestDIV()
    test.generate_prg_file()
