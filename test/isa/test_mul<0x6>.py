#!/usr/bin/env python3
"""
MUL Instruction Test
Opcode: 0x6
Format: R-Type
Encoding: [opcode:4][rd:4][rs1:4][rs2:4]
Operation: Rd = rs1 * rs2
"""

from pathlib import Path
import sys
sys.path.append(str(Path(__file__).parent.parent))

from isa_test_base import ISATestBase, Opcodes


class TestMUL(ISATestBase):
    def __init__(self):
        super().__init__(
            name="MUL",
            opcode=Opcodes.MUL,
            format="R-Type",
            description="MUL - Rd = rs1 * rs2"
        )

    def build_test(self):
        # Test 1: Basic multiplication (3 × 5 = 15)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x1, 3))    # ADDI S1, 3
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 5))    # ADDI S2, 5
        self.add_instruction(self.encode_rtype(Opcodes.MUL, 0x3, 0x1, 0x2)) # MUL S3, S1, S2

        # Test 2: Multiply by zero (7 × 0 = 0)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x4, 7))    # ADDI S4, 7
        self.add_instruction(self.encode_rtype(Opcodes.MUL, 0x5, 0x4, 0x0)) # MUL S5, S4, S0

        # Test 3: Multiply by one (9 × 1 = 9)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x6, 9))    # ADDI S6, 9
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x7, 1))    # ADDI S7, 1
        self.add_instruction(self.encode_rtype(Opcodes.MUL, 0x8, 0x6, 0x7)) # MUL S8, S6, S7

        # Test 4: Chain multiplication (2 × 3 × 4 = 24)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x9, 2))    # ADDI S9, 2
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0xA, 3))    # ADDI S10, 3
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0xB, 4))    # ADDI S11, 4
        self.add_instruction(self.encode_rtype(Opcodes.MUL, 0xC, 0x9, 0xA)) # MUL S12, S9, S10
        self.add_instruction(self.encode_rtype(Opcodes.MUL, 0xC, 0xC, 0xB)) # MUL S12, S12, S11

        # Test 5: S0 hardwired to zero (attempt to write result to S0)
        self.add_instruction(self.encode_rtype(Opcodes.MUL, 0x0, 0x1, 0x2)) # MUL S0, S1, S2

        self.add_halt()

    def get_expected_results(self):
        return {
            'S0': 0x0000,  # Always zero (hardwired)
            'S1': 0x0003,  # 3
            'S2': 0x0005,  # 5
            'S3': 0x000F,  # 3 × 5 = 15
            'S4': 0x0007,  # 7
            'S5': 0x0000,  # 7 × 0 = 0
            'S6': 0x0009,  # 9
            'S7': 0x0001,  # 1
            'S8': 0x0009,  # 9 × 1 = 9
            'S9': 0x0002,  # 2
            'S10': 0x0003, # 3
            'S11': 0x0004, # 4
            'S12': 0x0018, # 2 × 3 × 4 = 24
            'cycles': 15
        }


if __name__ == '__main__':
    test = TestMUL()
    test.generate_prg_file()
