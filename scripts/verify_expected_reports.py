from __future__ import annotations

import difflib
import re
import subprocess
import sys
import tempfile
from pathlib import Path


TIMESTAMP_PATTERN = re.compile(r"\d{8}-\d{6}\.\d{3}")
DEFAULT_SAMPLE_NUMBERS = ["2", "3", "4"]


def normalize_timestamps(text: str) -> str:
    normalized_text = text.replace("\r\n", "\n").lstrip("\ufeff")
    normalized = TIMESTAMP_PATTERN.sub("<TIMESTAMP>", normalized_text)
    return normalized.strip() + "\n"


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def compare_text(expected: str, actual: str, label: str) -> None:
    if expected == actual:
        return

    diff = "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{label}",
            tofile=f"actual/{label}",
        )
    )
    raise AssertionError(diff or f"Mismatch detected for {label}")


def run_sample(repo_root: Path, sample_number: str) -> None:
    engine_path = repo_root / "exchange.exe"
    sample_path = repo_root / "tests" / f"sample_orders_{sample_number}.csv"
    expected_exec_path = repo_root / "tests" / f"expected_exec_rep_{sample_number}.csv"
    expected_reject_path = repo_root / "tests" / f"expected_rejected_exec_rep_{sample_number}.csv"

    if not sample_path.exists():
        raise FileNotFoundError(f"Missing sample file: {sample_path}")
    if not expected_exec_path.exists():
        raise FileNotFoundError(f"Missing expected execution file: {expected_exec_path}")
    if not expected_reject_path.exists():
        raise FileNotFoundError(f"Missing expected rejected file: {expected_reject_path}")

    with tempfile.TemporaryDirectory(prefix=f"verify_sample_{sample_number}_") as temp_dir:
        temp_path = Path(temp_dir)
        result = subprocess.run(
            [str(engine_path), str(sample_path.resolve())],
            cwd=temp_path,
            capture_output=True,
            text=True,
        )
        if result.returncode != 0:
            raise RuntimeError(
                f"Engine failed for sample {sample_number}.\nSTDOUT:\n{result.stdout}\nSTDERR:\n{result.stderr}"
            )

        actual_exec = normalize_timestamps(read_text(temp_path / "execution_rep.csv"))
        actual_reject = normalize_timestamps(read_text(temp_path / "rejected_execution_rep.csv"))
        expected_exec = normalize_timestamps(read_text(expected_exec_path))
        expected_reject = normalize_timestamps(read_text(expected_reject_path))

        compare_text(expected_exec, actual_exec, f"execution sample {sample_number}")
        compare_text(expected_reject, actual_reject, f"rejected sample {sample_number}")

    print(f"Verified sample {sample_number}")


def main(argv: list[str]) -> int:
    repo_root = Path(__file__).resolve().parents[1]
    # Explicit sample arguments such as `8` are supported without changing the default fast path.
    sample_numbers = argv[1:] or DEFAULT_SAMPLE_NUMBERS

    try:
        for sample_number in sample_numbers:
            run_sample(repo_root, sample_number)
    except Exception as exc:
        print(str(exc), file=sys.stderr)
        return 1

    print("All expected reports verified.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
