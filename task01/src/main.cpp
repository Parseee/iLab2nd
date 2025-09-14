#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "ARC/src/arc.hpp"

size_t hits;

int slow_get_page(int num);
void count_hits();
void count_hits_from_file(std::ifstream &in, std::ofstream &out);
void process_files(int test_num);

int main() { count_hits(); }

int slow_get_page(int num) {
    // std::cerr << "slow : " << num << std::endl;
    --hits;
    return num * 2;
}

void count_hits() {
    int cache_size, num_calls;
    std::cin >> cache_size >> num_calls;

    caches::Cache<int, int> cache(cache_size);

    hits = num_calls;
    for (int i = 0; i < num_calls; ++i) {
        int x;
        std::cin >> x;
        std::cerr << x << " | " << cache.get_page(x, slow_get_page)
                  << std::endl;
    }

    std::cout << hits << std::endl;
}

void count_hits_from_file(std::ifstream &in, std::ofstream &out) {
    int cache_size, num_calls;
    in >> cache_size >> num_calls;

    caches::Cache<int, int> cache(cache_size);

    hits = num_calls;

    for (int i = 0; i < num_calls; ++i) {
        int q;
        in >> q;
        cache.get_page(q, slow_get_page);
    }

    out << hits << std::endl;
}

void process_files(int test_num) {
    std::ifstream in;
    std::ofstream out;

    for (int i = 0; i < test_num; ++i) {
        in.open("../tests/test" + std::to_string(i + 1) + ".in",
                std::fstream::in);
        out.open("../tests/test" + std::to_string(i + 1) + ".out",
                 std::fstream::out);

        if (in.fail() || out.fail()) {
            std::cerr << "Can't open " << i + 1 << "'th testcase!" << std::endl;
            std::abort();
        }

        count_hits_from_file(in, out);

        in.close();
        out.close();
    }
}