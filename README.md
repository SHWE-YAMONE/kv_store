# kv_store
## Summary

KV Store is a multi-threaded, asynchronous Key-Value store inspired by Redis, built for extreme performance on Linux. It utilizes a Shared-Nothing architecture combined with Lock-Free/Sharded storage to minimize CPU contention and maximize throughput.

## Advanced Features
**Asynchronous I/O:** Powered by epoll in Edge-Triggered (EPOLLET) mode for O(1) event notification.

**Zero-Allocation Path:** Uses custom Circular Ring Buffers and std::string_view to process requests without heap allocations during the hot path.

**Stateful RESP Parsing:** A non-blocking state machine parser that handles fragmented TCP packets and pipelined commands gracefully.

**Sharded Storage:** A sharded LRU cache structure that reduces lock contention across multiple IO threads.

**Memory Efficiency:** Designed with ClientContext isolation to ensure cache locality and prevent cross-thread data corruption.


## Project Structure
**project_root/**
&nbsp;&nbsp;&nbsp; - **include/**    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *client_context.hpp*        
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *io_thread.hpp*
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *kv_store.hpp*  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *lru.hpp*  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *resp_parser.hpp*  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *ring_buffer.hpp*  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *server.hpp*  

&nbsp;&nbsp;&nbsp; - **src/**            
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *io_thread.cpp*
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *main.hpp*   
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *resp_parser.hpp* 
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - *server.hpp*  

&nbsp;&nbsp;&nbsp; - **CMakeLists.txt**  
&nbsp;&nbsp;&nbsp; - **Dockerfile** 

## Getting Started
### Build(Docker)
```
docker build -t kv_store .
docker run -p 6379:6379 kv_store
```

### Usage
```
redis-cli -p 6379
```
### Supported Command
```
SET key value
GET key
```

### Example
```
SET foo bar
GET foo
```

### Response
```
bar
```

## Future Improvements
- Add DELETE/EXPIRE commands
- Integrate LRU cache into KVStore
- Cluster support
- Pipelining optomization
- Metrics & observability