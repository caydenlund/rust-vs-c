#!/usr/bin/env python3

import subprocess
import sys

COLUMNS = ["Matrix dim.", "Runtime (ns)"]
PRGM = "./cmake-build-release/matrix-c"
ARGS = [
        ["-n", "40",   "-r", "10000"],
        ["-n", "80",   "-r", "10000"],
        ["-n", "160",  "-r", "1000"],
        ["-n", "320",  "-r", "500"],
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
