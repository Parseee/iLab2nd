#include <cstdlib>
#include <iostream>
#include <random>

#include "ARC/src/arc.hpp"

size_t hits;

int slow_get_page(int num) {
    std::cout << "slow : " << num << std::endl;
    --hits;
    return num * 2;
}

int main() {
    int cache_size, num_calls;
    std::cin >> cache_size >> num_calls;
    // cache_size = 2, num_calls = 5;

    caches::Cache<int, int> cache(cache_size);

    std::vector<int> test = {1, 2, 1, 2, 1, 2, 3, 2, 3, 3, 4, 4, 4, 4, 1, 1, 1};

    test.clear();
    for (int i = 0; i < num_calls; ++i) {
        int x;
        std::cin >> x;
        test.push_back(x);
    }
    num_calls = test.size();

    hits = test.size();

    for (int i = 0; i < num_calls; ++i) {
        const int t = test[i];
        std::cout << t << " | " << cache.get_page(t, slow_get_page)
                  << std::endl;
    }

    std::cout << hits << std::endl;
}