#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include <optional>
#include <vector>

class Shard {
private:
    std::unordered_map<std::string, std::string> data;
    std::mutex mtx;
public:
    void set(const std::string& key, const std::string& val) {
        std::lock_guard<std::mutex> lock(mtx);
        data[key] = val;
    }
    std::optional<std::string> get(const std::string& key) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = data.find(key);
        if (it != data.end()) return it->second;
        return std::nullopt;
    }
};

class KVStore {
private:
    std::vector<Shard> shards;
public:
    KVStore(size_t num_shards = 16) : shards(num_shards) {}
    Shard& shard(const std::string& key) {
        return shards[std::hash<std::string>{}(key) % shards.size()];
    }
};