#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// template <typename T>
// int ideal_caching(size_t cache_size, const std::vector<T> &queries) {
//     std::unordered_set<T> cache;

//     size_t hits = 0;

//     for (auto it = queries.begin(); it != queries.end(); ++it) {
//         // if (std::distance(queries.begin(), it) % 1000 == 0) {
//         //     std::cerr << "been here " << std::distance(queries.begin(),
//         it)
//         //               << std::endl;
//         // }
//         if (cache.contains(*it)) {
//             ++hits;
//         } else {
//             if (std::find(std::next(it), queries.end(), *it) ==
//             queries.end()) {
//                 continue;
//             }

//             if (cache.size() < cache_size) {
//                 cache.insert(*it);
//                 continue;
//             }

//             typename std::unordered_map<T,
//                                         typename
//                                         std::vector<T>::const_iterator>
//                 future_occurance;
//             for (const auto &item : cache) {
//                 auto future_index =
//                     std::find_if(std::next(it), queries.end(),
//                                  [&item](const auto &q) { return q == item;
//                                  });
//                 future_occurance[item] = future_index;
//             }

//             auto victim = std::max_element(future_occurance.begin(),
//                                            future_occurance.end(),
//                                            [](auto const &a, auto const &b) {
//                                                return a.second < b.second;
//                                            });
//             if (victim != future_occurance.end()) {
//                 cache.erase(victim->first); // evict it from cache
//             }
//             cache.insert(*it);
//         }
//     }

//     return hits;
// }

template <typename T>
int ideal_caching(size_t cache_size, const std::vector<T> &queries) {
    std::unordered_set<T> cache;

    size_t hits = 0;

    for (auto it = queries.begin(); it != queries.end(); ++it) {
        if (cache.contains(*it)) {
            ++hits;
        } else {
            auto next_use = std::find(std::next(it), queries.end(), *it);

            if (next_use == queries.end()) {
                continue;
            }

            if (cache.size() < cache_size) {
                cache.insert(*it);
            } else {
                std::unordered_map<T, typename std::vector<T>::const_iterator>
                    future_occurrence;

                for (const auto &item : cache) {
                    auto future_index =
                        std::find(std::next(it), queries.end(), item);
                    future_occurrence[item] = future_index;
                }

                auto victim = std::max_element(
                    future_occurrence.begin(), future_occurrence.end(),
                    [](auto const &a, auto const &b) {
                        return a.second < b.second;
                    });

                if (victim != future_occurrence.end()) {
                    cache.erase(victim->first);
                }

                cache.insert(*it);
            }
        }
    }
    return hits;
}