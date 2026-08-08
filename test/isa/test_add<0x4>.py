"""
Test: ADD  - I-Type R Instruction
Opcode: 0x8
Format: [opcode:4][rd:4][rs1:4][rs2:4]

Tests:
1. Basic addition: 5 + 10 = 15
2. Addition with zero: 7 + 0 = 7
3. Three-way addition: (5 + 10) + 3 = 18 (chain multiple ADDs)
4. S0 as source: S0 + 5 = 5 (S0 is always 0)
5. S0 as destination: ADD S0, S1, S2 should be ignored (S0 stays 0)
"""

from isa_test_base import ISATestBase, Opcodes, run_test

class TestADD(ISATestBase):
    def __init__(self):
        super().__init__(
            name="ADD",
            opcode=Opcodes.ADD,
            format="R-Type",
            description="ADD - Rd = rs1 + rs2"
        )

    def build_test(self):
        # Setup: S1 = 5, S2 = 10
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x1, 5))             # ADDI S1, 5
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 10))            # ADDI S1, 10

        # Test 1: Basic addition (S3 = S1 + S2 = 5 + 10)
        self.add_instruction(self.encode_rtype(Opcodes.ADD, 0x3, 0x1, 0x2))         # ADD S3 S1 S2

        # Test 2: Addition with zero (S4 = S1 + S0 = 5 + 0 = 5)
        self.add_instruction(self.encode_rtype(Opcodes.ADD, 0x4, 0x1, 0x0))         # ADD S4, S1, S0

        # Test 3: Chain addition (S5 = S3 + S2 = 15 + 10 = 25)
        self.add_instruction(self.encode_rtype(Opcodes.ADD, 0x5, 0x3, 0x2))         # ADD S5, S3, S2

        # Test 4: S0 as destination (S0 should stay 0 - hardwired!)
        self.add_instruction(self.encode_rtype(Opcodes.ADD, 0x0, 0x1, 0x2))         # ADD S0, S1, S2 (ignored!)

        # HALT
        self.add_halt()                                                             # JMP <self>

    def get_expected_results(self):
        return {
            'S0': 0x0000,  # Hardwired to zero (ADD S0, S1, S2 ignored)
            'S1': 0x0005,  # 5
            'S2': 0x000A,  # 10
            'S3': 0x000F,  # 15 (5 + 10)
            'S4': 0x0005,  # 5 (5 + 0)
            'S5': 0x0019,  # 25 (15 + 10)
            'cycles': 7    # Total instructions executed
        }
if __name__ == '__main__':
    run_test(TestADD)