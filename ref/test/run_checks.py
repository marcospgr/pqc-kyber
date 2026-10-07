#!/usr/bin/env python3
"""Run every check and retain a failing exit code if any test fails."""
import json
from pathlib import Path
import subprocess
import sys

test_dir = Path(__file__).resolve().parent
names = ["test_hashing"]
names += [f"test_rejection{k}" for k in (512, 768, 1024)]
names += [f"test_kyber{k}" for k in (512, 768, 1024)]
names += [f"test_vectors{k}" for k in (512, 768, 1024)]
results = []
for name in names:
    result = subprocess.run([str(test_dir / name)], capture_output=True, text=True)
    print(f"{'PASS' if result.returncode == 0 else 'FAIL'} {name}", flush=True)
    if result.returncode:
        print(result.stdout[-2000:] + result.stderr[-2000:], end="", flush=True)
    results.append({"test": name, "exit_code": result.returncode,
                    "output": result.stdout if not name.startswith("test_vectors")
                    else result.stdout[-500:], "stderr": result.stderr})
if len(sys.argv) == 2:
    Path(sys.argv[1]).write_text(json.dumps(results, indent=2) + "\n")
sys.exit(any(result["exit_code"] != 0 for result in results))
