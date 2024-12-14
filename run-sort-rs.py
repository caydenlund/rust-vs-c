#!/usr/bin/env python3

import subprocess
import sys

COLUMNS = ["Array length", "Insertion sort runtime (ns)",  "Merge sort runtime (ns)",  "Radix sort runtime (ns)", "Validation runtime (ns)"]
PRGM = "./target/release/sort-rs"
ARGS = [
        ["-n", "10", "-r", "1000"],
        ["-n", "100", "-r", "1000"],
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
