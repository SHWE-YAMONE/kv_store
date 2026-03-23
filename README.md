# kv_store
## Summary

KV Store is a multi-threaded, asynchronous Key-Value store inspired by Redis, built for extreme performance on Linux. It utilizes a Shared-Nothing architecture combined with Lock-Free/Sharded storage to minimize CPU contention and maximize throughput.

## 🛠 Advanced Features
**Asynchronous I/O:** Powered by epoll in Edge-Triggered (EPOLLET) mode for O(1) event notification.

**Zero-Allocation Path:** Uses custom Circular Ring Buffers and std::string_view to process requests without heap allocations during the hot path.

**Stateful RESP Parsing:** A non-blocking state machine parser that handles fragmented TCP packets and pipelined commands gracefully.

**Sharded Storage:** A sharded LRU cache structure that reduces lock contention across multiple IO threads.

**Memory Efficiency:** Designed with ClientContext isolation to ensure cache locality and prevent cross-thread data corruption.
