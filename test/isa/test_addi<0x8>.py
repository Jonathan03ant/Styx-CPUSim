#!/usr/bin/env python3
"""
Test: ADDI (Add Immediate) - I-Type A Instruction
Opcode: 0x8
Format: [opcode:4][rd:4][imm8:8]

Tests:
1. Basic addition (5 + 10)
2. Zero immediate (S1 + 0)
3. Max immediate (S1 + 255)
4. S0 hardwired to zero (ADDI S0, 99 should do nothing)
"""

from isa_test_base import ISATestBase, Opcodes, run_test

class TestADDI(ISATestBase):
    def __init__(self):
        super().__init__(
            name="ADDI",
            opcode=Opcodes.ADDI,
            format="I-Type A",
            description="Add Immediate - Rd = Rd + imm8"
        )

    def build_test(self):
        """Build the test program"""
        # Test 1: Basic addition (S1 = 5, then S1 = S1 + 10 = 15)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x1, 5))
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x1, 10))

        # Test 2: Zero immediate (S2 = 5 + 0 = 5)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 5))
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 0))

        # Test 3: Max immediate (S3 = 0 + 255)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x3, 255))

        # Test 4: S0 hardwired (S0 should stay 0 - write ignored!)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x0, 99))

        # HALT
        self.add_halt()

    def get_expected_results(self):
        """Expected register values after execution"""
        return {
            'S0': 0x0000,  # Hardwired to zero
            'S1': 0x000F,  # 15 (5 + 10)
            'S2': 0x0005,  # 5 (5 + 0)
            'S3': 0x00FF,  # 255 (max imm8)
            'cycles': 7
        }

if __name__ == '__main__':
    run_test(TestADDI)
