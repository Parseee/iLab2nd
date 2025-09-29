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
                auto future_index =
                    std::find_if(std::next(it), queries.end(),
                                 [&item](const auto &q) { return q == item; });
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
                    victim = std::max(victim, occ);
                }
                cache.erase(*victim);
            }
            cache.insert(*it);
        }
    }

    return hits;
}