#!/usr/bin/env python3

import subprocess
import sys

COLUMNS = ["Number of allocations", "Malloc runtime (ns)",  "Free runtime (ns)"]
PRGM = "./target/release/malloc-rs"
ARGS = [
        ["-n", "10", "-r", "10000"],
        ["-n", "100", "-r", "10000"],
        ["-n", "1000", "-r", "1000"],
        ["-n", "10000", "-r", "500"],
    ]

print("\t".join(COLUMNS))
sys.stdout.flush()

for argset in ARGS:
    cmd = [PRGM] + argset
    
    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error executing: {' '.join(cmd)}")
        print(f"Return code: {e.returncode}")
