#pragma once

#include <unordered_map>
#include <vector>
#include <sys/epoll.h>

#include "client_context.hpp"
#include "kv_store.hpp"

class IOThread {
public:
    explicit IOThread(int id);
    void run();
    void add_client(int fd);

private:
    void handle_read(int fd);
    void process_requests(ClientContext& ctx);
    void execute_command(ClientContext& ctx);
    void close_connection(int fd);

    int epoll_fd;
    KVStore store;
    std::unordered_map<int, ClientContext> clients;
};