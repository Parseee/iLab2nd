#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

#include "ARC/src/arc.hpp"

size_t hits;

static int slow_get_page(int num);

template <typename T>
int count_hits(size_t cache_size, size_t num_calls,
               const std::vector<T> &queries);

void process_files(int test_num);

int main() {
    int cache_size, num_calls;
    std::cin >> cache_size >> num_calls;

    assert((cache_size > 0 && num_calls > 0) &&
           "cache size and num calls must be non-negative");

    std::vector<int> queries(num_calls);
    for (auto &q : queries) {
        std::cin >> q;
    }

    std::cout << count_hits(cache_size, num_calls, queries) << std::endl;
}

template <typename T>
int count_hits(size_t cache_size, size_t num_calls,
               const std::vector<T> &queries) {
    caches::Cache<int, int> cache(cache_size);
    hits = num_calls;
    for (const auto &q : queries) {
        cache.get_page(q, slow_get_page);
    }
    return hits;
}

int slow_get_page(int num) {
    --hits;
    return num * 2;
}