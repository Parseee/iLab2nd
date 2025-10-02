#pragma once

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
int ideal_caching(size_t cache_size, const std::vector<T> &queries) {
    std::unordered_set<T> cache;

    size_t hits = 0;

    std::unordered_map<T, std::queue<typename std::vector<T>::const_iterator>>
        future_positions;
    for (auto it = queries.begin(); it != queries.end(); ++it) {
        future_positions[*it].push(it);
    }

    for (auto it = queries.begin(); it != queries.end(); ++it) {
        future_positions[*it].pop();

        if (cache.contains(*it)) {
            ++hits;
        } else {
            if (future_positions[*it].empty()) {
                continue;
            }

            if (cache.size() < cache_size) {
                cache.insert(*it);
            } else {
                auto farthest_query = it;
                auto victim = cache.end();

                for (auto cache_it = cache.begin(); cache_it != cache.end();
                     ++cache_it) {
                    if (future_positions[*cache_it].empty()) {
                        farthest_query = queries.end();
                        victim = cache_it;
                        break;
                    } else {
                        auto next_use = future_positions[*cache_it].front();
                        farthest_query = std::max(farthest_query, next_use);
                    }
                }
                cache.erase(victim);
                cache.insert(*it);
            }
        }
    }
    return hits;
}