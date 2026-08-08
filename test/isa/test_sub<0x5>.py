#!/usr/bin/env python3
"""
SUB Instruction Test
Opcode: 0x5
Format: R-Type
Encoding: [opcode:4][rd:4][rs1:4][rs2:4]
Operation: Rd = rs1 - rs2
"""

from pathlib import Path
import sys
sys.path.append(str(Path(__file__).parent.parent))

from isa_test_base import ISATestBase, Opcodes


class TestSUB(ISATestBase):
    def __init__(self):
        super().__init__(
            name="SUB",
            opcode=Opcodes.SUB,
            format="R-Type",
            description="SUB - Rd = rs1 - rs2"
        )

    def build_test(self):
        # Test 1: Basic subtraction (10 - 3 = 7)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x1, 10))            # ADDI S1, 10
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 3))             # ADDI S2, 3
        self.add_instruction(self.encode_rtype(Opcodes.SUB, 0x3, 0x1, 0x2))         # SUB S3, S1, S2

        # Test 2: Subtraction with zero (5 - 0 = 5)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x4, 5))             # ADDI S4, 5
        self.add_instruction(self.encode_rtype(Opcodes.SUB, 0x5, 0x4, 0x0))         # SUB S5, S4, S0

        # Test 3: Subtraction resulting in zero (8 - 8 = 0)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x6, 8))             # ADDI S6, 8
        self.add_instruction(self.encode_rtype(Opcodes.SUB, 0x7, 0x6, 0x6))         # SUB S7, S6, S6

        # Test 4: Chain subtraction (15 - 5 - 3 = 7)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x8, 15))            # ADDI S8, 15
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x9, 5))             # ADDI S9, 5
        self.add_instruction(self.encode_rtype(Opcodes.SUB, 0xA, 0x8, 0x9))         # SUB S10, S8, S9
        self.add_instruction(self.encode_rtype(Opcodes.SUB, 0xB, 0xA, 0x2))         # SUB S11, S10, S2

        # Test 5: S0 hardwired to zero (attempt to write result to S0)
        self.add_instruction(self.encode_rtype(Opcodes.SUB, 0x0, 0x1, 0x2)) # SUB S0, S1, S2

        self.add_halt()

    def get_expected_results(self):
        return {
            'S0': 0x0000,  # Always zero (hardwired)
            'S1': 0x000A,  # 10
            'S2': 0x0003,  # 3
            'S3': 0x0007,  # 10 - 3 = 7
            'S4': 0x0005,  # 5
            'S5': 0x0005,  # 5 - 0 = 5
            'S6': 0x0008,  # 8
            'S7': 0x0000,  # 8 - 8 = 0
            'S8': 0x000F,  # 15
            'S9': 0x0005,  # 5
            'S10': 0x000A, # 15 - 5 = 10
            'S11': 0x0007, # 10 - 3 = 7
            'cycles': 13
        }


if __name__ == '__main__':
    test = TestSUB()
    test.generate_prg_file()
