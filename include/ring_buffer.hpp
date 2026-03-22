#pragma once
#include <vector>
#include <string_view>
#include <cstring>

class RingBuffer {
public:
    explicit RingBuffer(size_t capacity = 64 * 1024) 
        : data(capacity), cap(capacity), head(0), tail(0), full(false) {}

    size_t writable_space() const {
        if (full) return 0;
        if (head >= tail) return cap - head;
        return tail - head;
    }

    char* write_ptr() { return data.data() + head; }

    void produce(size_t len) {
        head = (head + len) % cap;
        if (head == tail) full = true;
    }

    std::string_view peek() const {
        if (empty()) return {};
        // Return only the contiguous part to the parser
        size_t end = (head > tail) ? head : cap;
        return {data.data() + tail, end - tail};
    }

    void consume(size_t len) {
        tail = (tail + len) % cap;
        full = false;
    }

    bool empty() const { return !full && (head == tail); }

private:
    std::vector<char> data;
    size_t cap;
    size_t head; 
    size_t tail;
    bool full;
};