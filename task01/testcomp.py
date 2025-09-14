from pathlib import Path
import sys
import subprocess
import difflib

REF_PATH = Path("./reference_tests/")
BIN_PATH = Path("./build/arc-cache")
TIMEOUT = 1.0

def run_test(test_path: Path) -> (bool, str):
    ref_path = REF_PATH / (test_path.stem + ".out")
    if not ref_path.exists():
        return False, f"Reference file missing: {ref_path}"

    try:
        proc = subprocess.run([str(BIN_PATH)], stdin=test_path.open("rb"),
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                              timeout=TIMEOUT)
    except subprocess.TimeoutExpired:
        return False, "Timed out"

    if proc.returncode != 0:
        return False, f"Non-zero exit ({proc.returncode}). Stderr:\n{proc.stderr.decode(errors='replace')}"

    actual = proc.stdout.decode(errors='replace').replace("\r\n", "\n")
    expected = ref_path.read_text(encoding="utf-8").replace("\r\n", "\n")

    if actual == expected:
        return True, "OK"

    # produce unified diff for failed test
    diff = "\n".join(difflib.unified_diff(
        expected.splitlines(), actual.splitlines(),
        fromfile=str(ref_path), tofile=f"{test_path.stem}.actual", lineterm=""
    ))
    return False, diff

def main():
    if not BIN_PATH.exists() or not BIN_PATH.is_file():
        print(f"Error: application not found at {BIN_PATH}", file=sys.stderr)
        sys.exit(2)
    if not REF_PATH.exists():
        print(f"Error: reference tests not found at {REF_PATH}", file=sys.stderr)
        sys.exit(2)

    input_files = sorted(REF_PATH.glob("test*.in"))
    if not input_files:
        print("No input files found in", REF_PATH)
        sys.exit(2)

    failures = 0
    
    for t in input_files:
        ok, info = run_test(t)
        if ok:
            print(f"[PASS] {t.name}")
        else:
            failures += 1
            print(f"[FAIL] {t.name}")
            print(info)
            print("-" * 60)
    
    total = len(input_files)
    print(f"\n{total - failures}/{total} passed, {failures} failed.")
    sys.exit(0 if failures == 0 else 1)

if __name__ == "__main__":
    main()
