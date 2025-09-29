#pragma once

#include <cstddef>
#include <iostream>
#include <list>
#include <unordered_map>

namespace caches {
template <typename KeyT, typename T> class Cache {
  public:
    Cache(size_t capacity = DEFAULT_CAPACITY) noexcept : capacity(capacity) {}

    template <typename F> T get_page(KeyT key, F slow_get_page);

  private:
    static constexpr size_t DEFAULT_CAPACITY = 10;
    size_t p = 0; // ARC cache divider
    size_t capacity;
    std::list<std::pair<KeyT, T>> t1;
    std::list<std::pair<KeyT, T>> t2;
    std::list<KeyT> b1;
    std::list<KeyT> b2;
    enum class Which { T1, T2, B1, B2 };

    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
    std::unordered_map<KeyT, ListIt> hash;
    using GhostListIt = typename std::list<KeyT>::iterator;
    std::unordered_map<KeyT, GhostListIt> ghost_hash;
    std::unordered_map<KeyT, Which> loc_hash;

    void replace(KeyT key);
    void LRU_to_ghost(KeyT key, std::list<std::pair<KeyT, T>> &list,
                      std::list<T> &ghost_list, Which which);
    void remove_from_ghost(KeyT key, std::list<KeyT> &ghost);
    template <typename F> T handle_miss(KeyT key, F slow_get_page);
};
} // namespace caches

template <typename KeyT, typename T>
template <typename F>
T caches::Cache<KeyT, T>::get_page(KeyT key, F slow_get_page) {
    auto cache_hit = hash.find(key);
    if (cache_hit != hash.end()) { // cache-hit
        ListIt hit = cache_hit->second;
        if (loc_hash[key] == Which::T1) {
            t2.splice(t2.begin(), t1, hit);
        } else {
            t2.splice(t2.begin(), t2, hit);
        }
        hash[key] = t2.begin();
        loc_hash[key] = Which::T2;
        return t2.front().second;
    }

    return handle_miss(key, slow_get_page);
}

template <typename KeyT, typename T>
template <typename F>
T caches::Cache<KeyT, T>::handle_miss(KeyT key, F slow_get_page) {
    auto cache_miss = ghost_hash.find(key);
    if (cache_miss != ghost_hash.end()) { // partial miss
        GhostListIt hit = cache_miss->second;
        if (loc_hash[key] == Which::B1) {
            p = std::min(capacity, p + std::max(b2.size() / b1.size(), 1ul));
            replace(key);

            t2.push_front({key, slow_get_page(key)});
            b1.erase(hit);
        } else {
            p = std::max(0ul, p - std::max(b1.size() / b2.size(), 1ul));
            replace(*hit);

            t2.push_front({key, slow_get_page(key)});
            b2.erase(hit);
        }
        ghost_hash.erase(key);
        loc_hash[key] = Which::T2;
        hash[key] = t2.begin();
        return t2.front().second;
    }

    // complete miss
    auto total_size = t1.size() + t2.size() + b1.size() + b2.size();
    if (total_size >= capacity) {
        if (t1.size() + b1.size() == capacity) {
            if (!b1.empty()) {
                remove_from_ghost(b1.back(), b1);
            }
        } else {
            if (!b2.empty()) {
                remove_from_ghost(b2.back(), b2);
            }
        }
    }
    if (t1.size() + t2.size() == capacity) {
        replace(key);
    }

    t1.push_front({key, slow_get_page(key)});
    hash[key] = t1.begin();
    loc_hash[key] = Which::T1;
    return t1.front().second;
}

template <typename KeyT, typename T>
void caches::Cache<KeyT, T>::LRU_to_ghost(KeyT key,
                                          std::list<std::pair<KeyT, T>> &list,
                                          std::list<T> &ghost_list,
                                          Which which) {
    if (list.size() == 0) {
        return;
    }
    auto evicted_key = list.back().first;
    hash.erase(evicted_key);
    list.pop_back();
    ghost_list.push_front(evicted_key);
    loc_hash[evicted_key] = which;
    ghost_hash[evicted_key] = ghost_list.begin();
}

template <typename KeyT, typename T>
void caches::Cache<KeyT, T>::replace(KeyT key) {
    if (!t1.empty() && (t1.size() > p || (p == 0 && t2.size() == capacity))) {
        LRU_to_ghost(key, t1, b1, Which::B1);
    } else {
        LRU_to_ghost(key, t2, b2, Which::B2);
    }
}

template <typename KeyT, typename T>
void caches::Cache<KeyT, T>::remove_from_ghost(KeyT key,
                                               std::list<KeyT> &ghost) {
    auto hit = ghost_hash.find(key);

    if (hit != ghost_hash.end()) {
        loc_hash.erase(key);
        auto elem_it = hit->second;
        ghost.erase(elem_it);
        ghost_hash.erase(key);
    }
}