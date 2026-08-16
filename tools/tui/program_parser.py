"""Program parser for Styx TUI - converts text to bytecode"""

def parse_program(text: str) -> tuple[bytes, bytes]:
    """Parse CODE/DATA sections from text into bytes"""

    lines = text.split('\n')
    current_section = None
    code_hex = []
    data_hex = []

    for line_num, line in enumerate(lines, 1):
        original_line = line
        line = line.strip()

        if not line:
            continue

        # Section markers
        if line.upper().startswith('CODE:'):
            current_section = 'CODE'
            continue
        elif line.upper().startswith('DATA:'):
            current_section = 'DATA'
            continue

        # Remove comments
        if '#' in line:
            line = line[:line.index('#')].strip()

        if not line:
            continue

        # Must be in a section
        if current_section is None:
            raise ValueError(f"Line {line_num}: Hex data outside CODE/DATA section")

        # Remove whitespace
        hex_value = line.replace(' ', '').replace('\t', '')

        # Validate: must be 4 hex digits
        if len(hex_value) != 4:
            raise ValueError(f"Line {line_num}: '{hex_value}' must be 4 hex digits")

        try:
            int(hex_value, 16)
        except ValueError:
            raise ValueError(f"Line {line_num}: Invalid hex '{hex_value}'")

        # Add to section
        if current_section == 'CODE':
            code_hex.append(hex_value)
        elif current_section == 'DATA':
            data_hex.append(hex_value)

    def to_little_endian_bytes(hex_words):
        """Convert list of 4-digit hex words to little-endian bytes"""
        result = []
        for word in hex_words:
            value = int(word, 16)
            low_byte = value & 0xFF
            high_byte = (value >> 8) & 0xFF
            result.extend([low_byte, high_byte])
        return bytes(result)

    code_bytes = to_little_endian_bytes(code_hex) if code_hex else b''
    data_bytes = to_little_endian_bytes(data_hex) if data_hex else b''

    return code_bytes, data_bytes
