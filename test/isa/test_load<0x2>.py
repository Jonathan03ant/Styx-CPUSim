"""
Test: LOAD - I-Type B Instruction
Opcode: 0x2
Format: [opcode:4][rd:4][rs:4][offset:4]

Tests:
1. Load from base address (S10 = 0x2000, offset 0)
2. Load from base + offset (S10 = 0x2000, offset 5)
3. Load from arbitrary address (S11 = 0x28FF, offset 0)
4. Load from arbitrary + offset (S11 = 0x28FF, offset 3)
5. Load with max offset (offset 15)
6. Multiple LOADs (verify idempotent/read-only)
7. Load from different region (HEAP 0x8000)
8. Load after overwrite (verify reads updated value)
"""

from isa_test_base import ISATestBase, Opcodes, run_test

class TestLOAD(ISATestBase):
    def __init__(self):
        super().__init__(
            name="LOAD",
            opcode=Opcodes.LOAD,
            format="I-Type B",
            description="LOAD - Rd = Memory[Rs + offset]"
        )

    def build_test(self):
        """Build the test program"""
        # SETUP: Prepare base addresses
        # Base address 1: 0x2000 (DATA region)
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0xA, 0x20))               # LUI S10, 0x20         → S10 = 0x2000
        # Base address 2: Arbitrary address: 0x28FF (DATA region, custom address)
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0xB, 0x28))               # LUI S11, 0x28         → S11 = 0x2800
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0xB, 0xFF))              # ADDI S11, 0xFF        → S11 = 0x28FF
        # Different region: 0x8000 (HEAP region)
        self.add_instruction(self.encode_itype_a(Opcodes.LUI, 0xC, 0x80))               # LUI S12, 0x80         → S12 = 0x8000

        # SETUP: Prepare memory with STORE (fill with known values)
        # Memory[0x2000] = 100
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x1, 100))               # ADDI S1, 100          → S1 = 100
        self.add_instruction(self.encode_itype_b(Opcodes.STORE, 0x1, 0xA, 0))           # STORE S1, S10, 0      → Memory[0x2000] = 100
        # Memory[0x2005] = 200
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 200))               # ADDI S2, 200          → S2 = 200
        self.add_instruction(self.encode_itype_b(Opcodes.STORE, 0x2, 0xA, 5))           # STORE S2, S10, 5      → Memory[0x2005] = 200
        # Memory[0x200F] = 255 (max offset test)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x3, 255))               # ADDI S3, 255          → S3 = 255
        self.add_instruction(self.encode_itype_b(Opcodes.STORE, 0x3, 0xA, 15))          # STORE S3, S10, 15     → Memory[0x200F] = 255

        # Memory[0x28FF] = 77 (Base Address 2)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x4, 77))                # ADDI S4, 77           → S4 = 77
        self.add_instruction(self.encode_itype_b(Opcodes.STORE, 0x4, 0xB, 0))           # STORE S4, S11, 0      → Memory[0x28FF] = 77

        # Memory[0x2902] = 88 (arbitrary + offset: 0x28FF + 3)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x5, 88))                # ADDI S5, 88           → S5 = 88
        self.add_instruction(self.encode_itype_b(Opcodes.STORE, 0x5, 0xB, 3))           # STORE S5, S11, 3      → Memory[0x2902] = 88

        # Memory[0x8000] = 99 (HEAP region)
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x6, 99))                # ADDI S6, 99           → S6 = 99
        self.add_instruction(self.encode_itype_b(Opcodes.STORE, 0x6, 0xC, 0))           # STORE S6, S12, 0      → Memory[0x8000] = 99

        # ================================================================
        # TEST 1: Load from base address (offset 0)
        # ================================================================
        self.add_instruction(self.encode_itype_b(Opcodes.LOAD, 0x7, 0xA, 0))            # LOAD S7, S10, 0       → S7 = Memory[0x2000] = 100

        # ================================================================
        # TEST 2: Load from base + offset
        # ================================================================
        self.add_instruction(self.encode_itype_b(Opcodes.LOAD, 0x8, 0xA, 5))            # LOAD S8, S10, 5       → S8 = Memory[0x2005] = 200

        # ================================================================
        # TEST 3: Load from arbitrary address
        # ================================================================
        self.add_instruction(self.encode_itype_b(Opcodes.LOAD, 0x9, 0xB, 0))            # LOAD S9, S11, 0       → S9 = Memory[0x28FF] = 77

        # ================================================================
        # TEST 4: Load from arbitrary + offset
        # ================================================================
        self.add_instruction(self.encode_itype_b(Opcodes.LOAD, 0xD, 0xB, 3))            # LOAD S13, S11, 3      → S13 = Memory[0x2902] = 88

        # ================================================================
        # TEST 5: Load with max offset (15)
        # ================================================================
        self.add_instruction(self.encode_itype_b(Opcodes.LOAD, 0xE, 0xA, 15))           # LOAD S14, S10, 15     → S14 = Memory[0x200F] = 255

        # ================================================================
        # TEST 6: Multiple LOADs (verify idempotent - read doesn't modify)
        # ================================================================
        self.add_instruction(self.encode_itype_b(Opcodes.LOAD, 0xF, 0xA, 0))            # LOAD S15, S10, 0      → S15 = Memory[0x2000] = 100
        # Load again to same location we loaded in TEST 1 (S7 also loaded from 0x2000)

        # ================================================================
        # TEST 7: Load from different region (HEAP)
        # ================================================================
        # *Note: Using a temporary register since S0-S15 filling up
        # We'll reuse S1 (original value no longer needed)
        self.add_instruction(self.encode_itype_b(Opcodes.LOAD, 0x1, 0xC, 0))            # LOAD S1, S12, 0        → S1 = Memory[0x8000] = 99

        # ================================================================
        # TEST 8: Load after overwrite (verify reads updated value)
        # ================================================================
        self.add_instruction(self.encode_itype_a(Opcodes.ADDI, 0x2, 50))                # ADDI S2, 50            → S2 = 250 (200+50)
        self.add_instruction(self.encode_itype_b(Opcodes.STORE, 0x2, 0xA, 0))           # STORE S2, S10, 0       → Memory[0x2000] = 250 (overwrite!)
        self.add_instruction(self.encode_itype_b(Opcodes.LOAD, 0x3, 0xA, 0))            # LOAD S3, S10, 0        → S3 = Memory[0x2000] = 250 (new value!)

        # HALT
        self.add_halt()                                                                  # JMP <self>             → HALT

    def get_expected_results(self):
        return {
            'S0': 0x0000,  # Hardwired zero
            'S1': 0x0063,  # 99 (loaded from HEAP 0x8000 in Test 7)
            'S2': 0x00FA,  # 250 (200 + 50, used to overwrite in Test 8)
            'S3': 0x00FA,  # 250 (loaded after overwrite in Test 8)
            'S4': 0x004D,  # 77 (setup, unchanged)
            'S5': 0x0058,  # 88 (setup, unchanged)
            'S6': 0x0063,  # 99 (setup, unchanged)
            'S7': 0x0064,  # 100 (loaded from 0x2000 - Test 1, before overwrite)
            'S8': 0x00C8,  # 200 (loaded from 0x2005 - Test 2)
            'S9': 0x004D,  # 77 (loaded from 0x28FF - Test 3)
            'S10': 0x2000, # Base address DATA
            'S11': 0x28FF, # Arbitrary address
            'S12': 0x8000, # Base address HEAP
            'S13': 0x0058, # 88 (loaded from 0x2902 - Test 4)
            'S14': 0x00FF, # 255 (loaded from 0x200F - Test 5)
            'S15': 0x0064, # 100 (loaded from 0x2000 - Test 6, idempotent)
            'cycles': 27   # 4 addr setup + 14 mem setup (7 ADDI + 7 STORE) + 8 LOADs + 3 overwrite + 1 HALT
        }

if __name__ == '__main__':
    run_test(TestLOAD)