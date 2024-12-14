#!/usr/bin/env python3

import subprocess
import sys

COLUMNS = ["Number of threads", "Matrix size", "Runtime (ns)"]
PRGM = "./cmake-build-release/concurrency-c"
ARGS = [
        ["-t", "1", "-n", "10",  "-r", "5000"],
        ["-t", "2", "-n", "10",  "-r", "5000"],
        ["-t", "4", "-n", "10",  "-r", "5000"],
        ["-t", "6", "-n", "10",  "-r", "5000"],
        ["-t", "8", "-n", "10",  "-r", "5000"],
        ["-t", "1", "-n", "100", "-r", "2000"],
        ["-t", "2", "-n", "100", "-r", "2000"],
        ["-t", "4", "-n", "100", "-r", "2000"],
        ["-t", "6", "-n", "100", "-r", "2000"],
        ["-t", "8", "-n", "100", "-r", "2000"],
        ["-t", "1", "-n", "400", "-r", "100"],
        ["-t", "2", "-n", "400", "-r", "100"],
        ["-t", "4", "-n", "400", "-r", "100"],
        ["-t", "6", "-n", "400", "-r", "100"],
        ["-t", "8", "-n", "400", "-r", "100"],
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
