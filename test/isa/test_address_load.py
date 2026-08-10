"""
Test: Address Loading - LUI + ADDI combination
Tests loading arbitrary 16-bit addresses into registers

LUI Rd, imm8  → Rd = imm8 << 8
ADDI Rd, imm8 → Rd = Rd + imm8

Tests:
1. Simple base address (0x2000) - LUI only
2. Address with lower byte (0x2008) - LUI + ADDI
3. Arbitrary address (0x28FF) - LUI + ADDI
4. Max DATA region (0x7FFF) - LUI + ADDI
5. Different region (0x3A5C) - LUI + ADDI
"""
from isa_test_base import ISATestBase, Opcodes, run_test

class TestAddressLoad(ISATestBase):
    def __init__(self):
        super().__init__(
            name="address_load",
            opcode=None,  # Tests combination of LUI + ADDI
            format="I-Type A",
            description="Load arbitrary 16-bit addresses using LUI + ADDI"
        )

    def build_test(self):
        """ Build the test program"""
        # TEST 1: Load simple base address  (0x2000 MEM Data base address)
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0x1, 0x20))       # S1 = 0x20 << 8 = 0x2000

        # TEST 2: Address 0x2008  S2 = 0x2000 + 0x08 = 0x2008
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0x2, 0x20))       # LUI, S2, 0x20
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 0x08))      # ADDI, S2, 0x08

        # TEST 3: Arbitrary address 0x28FF
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0x3, 0x28))       # LUI S3, 0x28
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x3, 0xFF))      # ADDI, S3, 0xFF

        # TEST 4: Max DATA region (0x7FFF) S4 = 0x7F00 + 0xFF = 0x7FFF
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0x4, 0x7F))       # LUI, S4, 0x7F
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x4, 0xFF))      # ADDI, S4, 0xFF

        # TEST 5: Different region (0x3A5C) S5 = 0x3A + 0x5C = 0x3A5C
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0x5, 0x3A))       # LUI, S5, 0x3A
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x5, 0x5C))      # ADDI, S5, 0x5C

        # TEST 6: Heap region start (0x8000)
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0x6, 0x80))       # LUI, S6, 0x80

        # TEST 7: Stack region (0xE000)
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0x7, 0xE0))       # LUI, S7, 0xE0

        # HALT
        self.add_halt()

    def get_expected_results(self):
            return {
                'S0': 0x0000,  # Hardwired zero
                'S1': 0x2000,  # DATA base (LUI only)
                'S2': 0x2008,  # DATA + 8 (LUI + ADDI)
                'S3': 0x28FF,  # Arbitrary address (LUI + ADDI)
                'S4': 0x7FFF,  # Max DATA region (LUI + ADDI)
                'S5': 0x3A5C,  # Random address (LUI + ADDI)
                'S6': 0x8000,  # HEAP base (LUI only)
                'S7': 0xE000,  # STACK base (LUI only)
                'cycles': 12   # 1 + 2 + 2 + 2 + 2 + 1 + 1 + 1 HALT
            }

if __name__ == '__main__':
    run_test(TestAddressLoad)