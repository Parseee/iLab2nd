#include <array>
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

#include "Belady/src/belady.hpp"

int main() {
    int cache_size, num_calls;
    std::cin >> cache_size >> num_calls;

    std::vector<int> queries(num_calls);
    for (auto &q : queries) {
        std::cin >> q;
    }

    std::cout << ideal_caching(cache_size, queries) << std::endl;
}