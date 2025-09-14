#!/usr/bin/env python3
import random
import os

# Configuration
OUTPUT_DIR = "tests"      # directory to write test files
NUM_FILES = 10            # how many test files to generate
MAX_CACHE_SIZE = 10       # maximum cache size (actual size will be 1..MAX_CACHE_SIZE)
MAX_NUM_CALLS = 25        # maximum number of calls (actual will be 1..MAX_NUM_CALLS)
MAX_KEY = 50              # keys are integers in range [0, MAX_KEY]

random.seed()  # system time

os.makedirs(OUTPUT_DIR, exist_ok=True)

for i in range(1, NUM_FILES + 1):
    cache_size = random.randint(1, MAX_CACHE_SIZE)
    num_calls = random.randint(1, MAX_NUM_CALLS)
    calls = [str(random.randint(0, MAX_KEY)) for _ in range(num_calls // 2)]

    content = f"{cache_size} {num_calls} \n"
    for _ in range(num_calls):
        content += str(calls[random.randint(0, len(calls) - 1)]) + " "

    path = os.path.join(OUTPUT_DIR, f"test{i}.in")
    with open(path, "w") as f:
        f.write(content)

print(f"Generated {NUM_FILES} test files in '{OUTPUT_DIR}'")
