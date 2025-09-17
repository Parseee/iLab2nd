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

#include "ARC/src/arc.hpp"

size_t hits;

int slow_get_page(int num);

template <typename T>
int count_hits(size_t cache_size, size_t num_calls,
               const std::vector<T> &queries);
template <typename T>
int ideal_caching(size_t cache_size, size_t num_calls, std::vector<T> &queries);
void process_files(int test_num);

int main() {
    int cache_size, num_calls;
    std::cin >> cache_size >> num_calls;

    std::vector<int> queries(num_calls);
    for (auto &q : queries) {
        std::cin >> q;
    }

    std::cout << count_hits(cache_size, num_calls, queries) << " | "
              << ideal_caching(cache_size, num_calls, queries);
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

template <typename T>
int ideal_caching(size_t cache_size, size_t num_calls,
                  std::vector<T> &queries) {
    std::unordered_set<T> cache;

    hits = 0;

    for (auto it = queries.begin(); it != queries.end(); ++it) {
        if (cache.contains(*it)) {
            ++hits;
        } else {
            if (cache.size() < cache_size) {
                cache.insert(*it);
                continue;
            }

            std::unordered_map<T, typename std::vector<T>::iterator>
                future_occurance;
            for (const auto &item : cache) {
                typename std::vector<T>::iterator future_index = queries.end();
                for (auto qit = it + 1; qit != queries.end(); ++qit) {
                    if (*qit == item) {
                        future_index = qit;
                        break;
                    }
                }
                future_occurance[item] = future_index;
            }

            std::vector<T> never_used;

            for (const auto &[key, n_it] : future_occurance) {
                if (n_it == queries.end()) {
                    never_used.push_back(key);
                }
            }

            if (!never_used.empty()) {
                cache.erase(never_used.front());
            } else {
                auto victim = queries.begin();
                for (const auto &[key, occ] : future_occurance) {
                    victim = (victim < occ) ? occ : victim;
                }
                cache.erase(*victim);
            }
            cache.insert(*it);
        }
    }

    return hits;
}

int slow_get_page(int num) {
    // std::cerr << "slow : " << num << std::endl;
    --hits;
    return num * 2;
}

#ifdef NOT_DEF
int count_hits_from_file(std::ifstream &in, std::ofstream &out) {
    int cache_size, num_calls;
    in >> cache_size >> num_calls;

    caches::Cache<int, int> cache(cache_size);

    hits = num_calls;

    std::vector<int> queries(num_calls);
    for (auto &q : queries) {
        in >> q;
    }

    for (const auto &q : queries) {
        cache.get_page(q, slow_get_page);
    }

    out << hits << std::endl;
    std::cout << hits << "  |  ";
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
#endif /* NOT_DEF */