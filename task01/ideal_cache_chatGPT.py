from collections import deque, defaultdict
from math import inf
import sys

def belady_optimal(requests, capacity):
    if capacity <= 0:
        return len(requests), []

    positions = defaultdict(deque)
    for idx, page in enumerate(requests):
        positions[page].append(idx)

    cache = set()
    page_faults = 0

    for i, page in enumerate(requests):
        positions[page].popleft()

        if page in cache:
            continue  # hit

        page_faults += 1  # miss

        if len(cache) < capacity:
            cache.add(page)
            continue

        # eviction: choose the one with farthest next use
        victim = None
        victim_next_idx = -1
        for p in cache:
            next_idx = positions[p][0] if positions[p] else inf
            if next_idx > victim_next_idx:
                victim_next_idx = next_idx
                victim = p

        cache.remove(victim)
        cache.add(page)

    return page_faults, list(cache)


if __name__ == "__main__":
    data = sys.stdin.read().strip().split()
    if len(data) < 2:
        print("Invalid input format. Expected: <cache_size> <num_queries> <queries...>")
        sys.exit(1)

    cache_size = int(data[0])
    num_queries = int(data[1])
    queries = list(map(int, data[2:]))

    if len(queries) != num_queries:
        print(f"Expected {num_queries} queries but got {len(queries)}")
        sys.exit(1)

    faults, final_cache = belady_optimal(queries, cache_size)
    print(f"Page Hits: {num_queries - faults}")
