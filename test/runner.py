#!/usr/bin/env python3
"""
Styx ISA Test Runner

A cinematic test framework for validating Styx CPU instructions.

Features:
  - Interactive step-by-step execution
  - Color-coded output with timing delays
  - Automated verification (expected vs actual)
  - Support for auto-run mode (skip prompts)

Usage:
  python3 runner.py isa/test_add.py              # Interactive mode
  python3 runner.py isa/test_add.py --auto       # Skip all prompts
  python3 runner.py isa/test_add.py --no-delay   # Remove delays
  python3 runner.py isa/test_add.py --verbose    # Show full output
"""

import sys
import subprocess
import re
import time
import argparse
from pathlib import Path

# ANSI Colors
C_RESET = '\033[0m'
C_BOLD = '\033[1m'
C_GREEN = '\033[92m'
C_CYAN = '\033[96m'
C_YELLOW = '\033[93m'
C_RED = '\033[91m'
C_DIM = '\033[2m'

def strip_ansi(text):
    """Remove ANSI color codes"""
    ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    return ansi_escape.sub('', text)

def parse_register_value(output, reg_name):
    """Extract register value from isa_test output"""
    clean_output = strip_ansi(output)
    final_state_section = clean_output.split('FINAL STATE')[-1]
    pattern = rf'│\s+{reg_name}\s+│\s+(0x[0-9A-F]+)\s+│'
    match = re.search(pattern, final_state_section)
    if match:
        return int(match.group(1), 16)
    return None

def parse_cycles(output):
    """Extract cycle count from HALT message or FINAL STATE header"""
    clean_output = strip_ansi(output)
    match = re.search(r'\[HALT\] CPU halted at cycle (\d+)', clean_output)
    if match:
        return int(match.group(1))
    match = re.search(r'FINAL STATE \((\d+) cycles', clean_output)
    if match:
        return int(match.group(1))
    return None

def parse_timing(output):
    """Extract timing from FINAL STATE header"""
    clean_output = strip_ansi(output)
    match = re.search(r'FINAL STATE \(\d+ cycles, ([\d.]+) ms\)', clean_output)
    if match:
        return match.group(1)
    return None

def prompt_continue(message, auto=False):
    """Ask user to continue (or auto-continue if --auto flag)"""
    if auto:
        return True
    response = input(f"{C_CYAN}➤ {message}{C_RESET} ").strip().lower()
    return response in ['y', 'yes', '']

def extract_section(output, start_marker, end_marker=None):
    """Extract a section of output between markers (preserving colors)"""
    lines = output.split('\n')
    clean_lines = [strip_ansi(line) for line in lines]

    # Find start
    start_idx = -1
    for i, line in enumerate(clean_lines):
        if start_marker in line:
            start_idx = i
            break

    if start_idx == -1:
        return ""

    # Find end
    end_idx = len(lines)
    if end_marker:
        for i in range(start_idx + 1, len(clean_lines)):
            if end_marker in clean_lines[i]:
                end_idx = i
                break

    return '\n'.join(lines[start_idx:end_idx])

def run_single_test(test_file, args):
    """Run a single test with optional interactive prompts"""
    test_path = Path(test_file)

    if not test_path.exists():
        print(f"{C_RED}[ERROR]{C_RESET} Test file not found: {test_file}")
        return False

    # Timing function (respects --no-delay flag)
    def delay(seconds):
        if not args.no_delay:
            time.sleep(seconds)

    # Header
    print(f"\n{C_BOLD}{'='*64}{C_RESET}")
    print(f"{C_BOLD}Styx ISA Test Runner{C_RESET}")
    print(f"{C_BOLD}{'='*64}{C_RESET}\n")

    # ========================================================================
    # STEP 1: Generate .prg file
    # ========================================================================
    print(f"{C_CYAN}[Step 1]{C_RESET} Generating test program...")
    delay(0.3)

    result = subprocess.run(
        ["python3", test_path.name],
        capture_output=True,
        text=True,
        cwd=test_path.parent
    )

    if result.returncode != 0:
        print(f"{C_RED}✗ Generation failed{C_RESET}")
        print(result.stderr)
        return False

    # Extract test name and prg file
    test_name = test_path.stem.replace('test_', '').split('<')[0]
    prg_file = test_path.parent / f"test_{test_name}.prg"

    delay(0.2)

    # Show generation output (hide EXPECTED: in non-verbose mode)
    if args.verbose:
        print(result.stdout.strip())
    else:
        for line in result.stdout.split('\n'):
            if not line.strip().startswith(('EXPECTED:', 'S', 'cycles')):
                print(line)

    print(f"{C_DIM}Generated: {prg_file}{C_RESET}\n")
    delay(0.5)

    # Extract expected results from EXPECTED: section
    expected = {}
    for line in result.stdout.split('\n'):
        # Match:   S1=0x0005 or   cycles=7
        match = re.match(r'\s+([A-Za-z0-9]+)=(.+)', line)
        if match:
            reg = match.group(1)
            value = match.group(2).strip()
            if reg == 'cycles':
                expected[reg] = int(value)
            else:
                expected[reg] = int(value, 16) if value.startswith('0x') else int(value)

    # ========================================================================
    # STEP 2: Run simulation
    # ========================================================================
    if not prompt_continue(f"Run simulation? [Y/n]:", args.auto):
        print(f"{C_YELLOW}Aborted{C_RESET}")
        return False

    print(f"\n{C_CYAN}[Step 2]{C_RESET} Running simulation...\n")
    delay(0.3)

    result = subprocess.run(
        ["./isa_test", str(prg_file)],
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        print(f"{C_RED}✗ Simulation failed{C_RESET}")
        print(result.stderr)
        return False

    # Parse output
    output = result.stdout

    # Show loading info
    loading_section = extract_section(output, 'Loading:', 'INITIAL STATE')
    print(loading_section.strip())
    delay(0.8)

    # Show initial state
    print()
    initial_section = extract_section(output, 'INITIAL STATE', 'EXECUTION')
    print(initial_section.strip())
    delay(1.0)

    # Ask for execution
    print()
    if not prompt_continue(f"Execute program? [Y/n]:", args.auto):
        print(f"{C_YELLOW}Aborted{C_RESET}")
        return False

    print()
    # Show execution
    execution_section = extract_section(output, 'EXECUTION', '[HALT]')
    print(execution_section.strip())
    delay(0.5)

    # Show HALT with timing
    cycles = parse_cycles(output)
    timing_ms = parse_timing(output)
    print(f"\n{C_RED}[HALT]{C_RESET} CPU halted at cycle {cycles} ({timing_ms} ms)\n")
    delay(0.8)

    # Show final register state
    lines = output.split('\n')
    register_file_count = 0
    final_start = -1
    for i, line in enumerate(lines):
        if 'Register File' in strip_ansi(line):
            register_file_count += 1
            if register_file_count == 2:  # Second occurrence
                final_start = i
                break

    if final_start != -1:
        final_end = -1
        for i in range(final_start, len(lines)):
            if 'Test completed' in strip_ansi(lines[i]):
                final_end = i
                break
        if final_end == -1:
            final_end = len(lines)
        print('\n'.join(lines[final_start:final_end]).strip())

    delay(0.5)

    # Show test completed
    print(f"\n{C_GREEN}{'='*64}")
    print("Test completed")
    print(f"{'='*64}{C_RESET}\n")
    delay(0.5)

    # ========================================================================
    # STEP 3: Verify results
    # ========================================================================
    if not prompt_continue(f"Verify results? [Y/n]:", args.auto):
        print(f"{C_YELLOW}Skipped verification{C_RESET}")
        return True

    print(f"\n{C_CYAN}[Step 3]{C_RESET} Verifying results...\n")
    delay(0.3)

    # Parse actual results
    actual = {}
    for reg in expected.keys():
        if reg == 'cycles':
            actual[reg] = parse_cycles(output)
        else:
            actual[reg] = parse_register_value(output, reg)

    # Compare results
    print(f"{C_BOLD}{'='*64}{C_RESET}")
    print(f"{C_BOLD}VERIFICATION{C_RESET}")
    print(f"{C_BOLD}{'='*64}{C_RESET}")

    all_passed = True
    for reg, expected_val in expected.items():
        delay(0.15)  # Delay between each test
        actual_val = actual.get(reg)

        if actual_val is None:
            print(f"{C_YELLOW}[WARN]{C_RESET} {reg}: Could not parse")
            continue

        if actual_val == expected_val:
            if reg == 'cycles':
                print(f"{C_GREEN}[PASS]{C_RESET} {reg}: {expected_val}")
            else:
                print(f"{C_GREEN}[PASS]{C_RESET} {reg}: 0x{expected_val:04X}")
        else:
            if reg == 'cycles':
                print(f"{C_RED}[FAIL]{C_RESET} {reg}: expected {expected_val}, got {actual_val}")
            else:
                print(f"{C_RED}[FAIL]{C_RESET} {reg}: expected 0x{expected_val:04X}, got 0x{actual_val:04X}")
            all_passed = False

    delay(0.3)  # Pause before final result
    print(f"{C_BOLD}{'='*64}{C_RESET}")
    if all_passed:
        print(f"{C_GREEN}{C_BOLD}✓ ALL TESTS PASSED{C_RESET}")
    else:
        print(f"{C_RED}{C_BOLD}✗ SOME TESTS FAILED{C_RESET}")
    print(f"{C_BOLD}{'='*64}{C_RESET}\n")

    return all_passed

def main():
    parser = argparse.ArgumentParser(
        description='Styx ISA Test Runner - Automated instruction verification',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s isa/test_add.py              Interactive mode with prompts
  %(prog)s isa/test_add.py --auto       Auto-run (skip all prompts)
  %(prog)s isa/test_add.py --no-delay   Fast mode (no timing delays)
  %(prog)s isa/test_add.py --verbose    Show full output
  %(prog)s isa/test_add.py -a -n        Auto + fast mode

Workflow:
  1. Generate .prg file from test
  2. Run simulation with isa_test harness
  3. Verify expected vs actual register values
        """
    )

    parser.add_argument(
        'test_file',
        help='Test file to run (e.g., isa/test_add.py)'
    )

    parser.add_argument(
        '-a', '--auto',
        action='store_true',
        help='Auto mode: skip all prompts (answer yes to everything)'
    )

    parser.add_argument(
        '-n', '--no-delay',
        action='store_true',
        help='Fast mode: remove timing delays between steps'
    )

    parser.add_argument(
        '-v', '--verbose',
        action='store_true',
        help='Verbose mode: show full output including expected results'
    )

    args = parser.parse_args()

    # Run the test
    passed = run_single_test(args.test_file, args)

    sys.exit(0 if passed else 1)

if __name__ == '__main__':
    main()
