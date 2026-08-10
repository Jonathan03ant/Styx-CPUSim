"""
Test: MOV - R-Type Instruction
Opcode: 0x1
Format: [opcode:4][rd:4][rs1:4][rs2:4]  (rs2 unused)

Tests:
1. Basic move: Copy S1 to S3 (both should have same value)
2. Move from S0: Copy S0 to S4 (S4 should be 0)
3. Chain move: Copy S3 to S5 (verify S3 still has value)
4. Overwrite: Copy S2 to S1 (S1 changes, S2 unchanged)
5. S0 as destination: MOV S0, S2 ignored (S0 stays 0)
"""

from isa_test_base import ISATestBase, Opcodes, run_test

class TestMOV(ISATestBase):
    def __init__(self):
        super().__init__(
                    name="MOV",
                    opcode=Opcodes.MOV,
                    format="R-TYPE",
                    description="MOV source register to dest register"
                )

    def build_test(self):
        """"BUILD the test program"""
        # Setup: Initialize S1 = 100, S2 = 200
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x1, 100))       # ADDI S1, 100
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 200))       # ADDI S2, 200

        # Test 1: Basic move, s1 = s3
        self.add_instruction(self.encode_rtype(Opcodes.MOV, 0x3, 0x1, 0x0))     # MOV, S3, S1

        # Test 2: Move zero, s4 = s0 = 0
        self.add_instruction(self.encode_rtype(Opcodes.MOV, 0x4, 0x0, 0x0))     # MOV, S4, S0

        # Test 3: Chain move (S5 = S3 = 100)
        self.add_instruction(self.encode_rtype(Opcodes.MOV, 0x5, 0x3, 0x0))     # MOV, S5, S3

        # Test 4: Overwrite (S1 = S2 = 200)
        self.add_instruction(self.encode_rtype(Opcodes.MOV, 0x1, 0x2, 0x0))     # MOV S1, S2

        # Test 5: S0 as destination (should be ignored)
        self.add_instruction(self.encode_rtype(Opcodes.MOV, 0x0, 0x2, 0x0))     # MOV S0, S2

        # HALT
        self.add_halt()

    def get_expected_results(self):
        return {
            'S0': 0x0000,   # Hardwired to zero
            'S1': 0x00C8,   # 100
            'S2': 0x00C8,   # 200
            'S3': 0x0064,   # 100
            'S4': 0x0000,   # 0
            'S5': 0x0064,   #100
            'cycles': 8
        }

if __name__ == '__main__':
    run_test(TestMOV)
