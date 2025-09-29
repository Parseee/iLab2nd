#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
int ideal_caching(size_t cache_size, const std::vector<T> &queries) {
    std::unordered_set<T> cache;

    size_t hits = 0;

    for (auto it = queries.begin(); it != queries.end(); ++it) {
        if (cache.contains(*it)) {
            ++hits;
        } else {
            if (cache.size() < cache_size) {
                cache.insert(*it);
                continue;
            }

            typename std::unordered_map<T,
                                        typename std::vector<T>::const_iterator>
                future_occurance;
            for (const auto &item : cache) {
                typename std::vector<T>::const_iterator future_index =
                    queries.end();
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