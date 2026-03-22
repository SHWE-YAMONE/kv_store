#pragma once
#include <unordered_map>
#include <list>

template<typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(size_t cap) : capacity(cap) {}

    void put(const K& key, const V& value) {
        auto it = map.find(key);

        if (it != map.end()) {
            items.erase(it->second);
        }

        items.push_front({key, value});
        map[key] = items.begin();

        if (items.size() > capacity) {
            auto last = items.back();
            map.erase(last.first);
            items.pop_back();
        }
    }

    bool get(const K& key, V& value) {
        auto it = map.find(key);
        if (it == map.end()) return false;

        items.splice(items.begin(), items, it->second);
        value = it->second->second;
        return true;
    }

    void remove(const K& key) {
        auto it = map.find(key);
        if (it == map.end()) return;

        items.erase(it->second);
        map.erase(it);
    }

private:
    size_t capacity;
    std::list<std::pair<K,V>> items;
    std::unordered_map<K, typename std::list<std::pair<K,V>>::iterator> map;
};