#pragma once

#include <cstddef>
#include <iostream>
#include <list>
#include <unordered_map>

namespace caches {
template <typename KeyT, typename T> class Cache {
  public:
    Cache() noexcept : cap(10) {}
    explicit Cache(size_t capacity) noexcept : cap(capacity) {}

    template <typename F> T get_page(KeyT key, F slow_get_page);

  private:
    size_t sz;
    int p = 0; // ARC cache divider
    size_t cap;
    std::list<std::pair<KeyT, T>> t1;
    std::list<std::pair<KeyT, T>> t2;
    std::list<KeyT> b1;
    std::list<KeyT> b2;

    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
    std::unordered_map<KeyT, ListIt> hash;
    using GhostListIt = typename std::list<KeyT>::iterator;
    std::unordered_map<KeyT, GhostListIt> ghost_hash;
    enum Which { T1, T2, B1, B2, NIL };
    std::unordered_map<KeyT, Which> loc_hash;

    void replace(KeyT key);
    void LRU_evict(KeyT key, std::list<std::pair<KeyT, T>> &list,
                   std::list<T> &ghost_list, Which which);
};
} // namespace caches

template <typename KeyT, typename T>
template <typename F>
T caches::Cache<KeyT, T>::get_page(KeyT key, F slow_get_page) {
    auto cache_hit = hash.find(key);
    if (cache_hit != hash.end()) { // cache-hit
        ListIt hit = cache_hit->second;
        if (loc_hash[key] == T1) {
            t2.splice(t2.begin(), t1, hit);
        } else {
            t2.splice(t2.begin(), t2, hit);
        }
        hash[key] = t2.begin();
        loc_hash[key] = T2;
        return t2.front().second;
    }

    auto cache_miss = ghost_hash.find(key);
    if (cache_miss != ghost_hash.end()) { // partial miss
        GhostListIt hit = cache_miss->second;
        if (loc_hash[key] == B1) {
            p = std::min(cap, p + std::max(b2.size() / b1.size(), 1ul));
            replace(key);

            t2.push_front({key, slow_get_page(key)});
            b1.erase(hit);
        } else {
            p = std::max(0ul, p - std::max(b1.size() / b2.size(), 1ul));
            replace(*hit);

            t2.push_front({key, slow_get_page(key)});
            b2.erase(hit);
        }
        loc_hash[key] = T2;
        hash[key] = t2.begin();
        return t2.front().second;
    }

    // complete miss
    if (t1.size() + b1.size() == cap) {
        if (t1.size() < cap) {
            loc_hash[b1.back()] = NIL;
            ghost_hash.erase(b1.back());
            b1.pop_back();
            replace(key);
        } else {
            LRU_evict(key, t1, b1, B1);
        }
    } else if (t1.size() + t2.size() + b1.size() + b2.size() >= cap) {
        if (t1.size() + t2.size() + b1.size() + b2.size() >= 2 * cap) {
            loc_hash[b2.back()] = NIL;
            ghost_hash.erase(b2.back());
            b2.pop_back();
        }
        replace(key);
    }
    t1.push_front({key, slow_get_page(key)});
    hash[key] = t1.begin();
    loc_hash[key] = T1;
    return t1.front().second;
}

template <typename KeyT, typename T>
void caches::Cache<KeyT, T>::LRU_evict(KeyT key,
                                       std::list<std::pair<KeyT, T>> &list,
                                       std::list<T> &ghost_list, Which which) {
    if (list.size() == 0) {
        return;
    }
    auto evicted_key = list.back().first;
    hash.erase(list.back().first);
    list.pop_back();
    ghost_list.push_front(evicted_key);
    loc_hash[evicted_key] = which;
    ghost_hash[evicted_key] = ghost_list.begin();
}

template <typename KeyT, typename T>
void caches::Cache<KeyT, T>::replace(KeyT key) {
    if (!t1.empty() &&
        (t1.size() > p || (loc_hash[key] == B2 && t1.size() == p))) {
        LRU_evict(key, t1, b1, B1);
    } else {
        LRU_evict(key, t2, b2, B2);
    }
}