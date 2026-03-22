#pragma once
#include <string>
#include <vector>
#include "resp_parser.hpp"
#include "ring_buffer.hpp"

struct ClientContext {
    int fd;
    RingBuffer rb;
    RESPParser parser;
    Command current_cmd;

    explicit ClientContext(int client_fd) : fd(client_fd) {}
};