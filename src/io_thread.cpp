#include "io_thread.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <iostream>

IOThread::IOThread(int id) : epoll_fd(-1) {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
    }
}

void IOThread::add_client(int fd) {
    // Set non-blocking for Edge Triggered mode
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

    clients.emplace(std::piecewise_construct,
                    std::forward_as_tuple(fd),
                    std::forward_as_tuple(fd));

    epoll_event ev{};
    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    ev.data.fd = fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl: add");
        clients.erase(fd);
        close(fd);
    }
}

void IOThread::run() {
    std::vector<epoll_event> events(1024);

    while (true) {
        int n = epoll_wait(epoll_fd, events.data(), events.size(), -1);
        if (n == -1) {
            if (errno == EINTR) continue;
            break;
        }

        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;

            if (events[i].events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) {
                close_connection(fd);
            } 
            else if (events[i].events & EPOLLIN) {
                handle_read(fd);
            }
        }
    }
}

void IOThread::handle_read(int fd) {
    auto it = clients.find(fd);
    if (it == clients.end()) return;

    ClientContext& ctx = it->second;

    while (true) {
        size_t space = ctx.rb.writable_space();
        if (space == 0) break; 

        ssize_t n = recv(fd, ctx.rb.write_ptr(), space, 0);

        if (n > 0) {
            ctx.rb.produce(n);
        } else if (n == 0) {
            close_connection(fd);
            return;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            close_connection(fd);
            return;
        }
    }
    process_requests(ctx);
}

void IOThread::process_requests(ClientContext& ctx) {
    while (!ctx.rb.empty()) {
        std::string_view view = ctx.rb.peek();
        size_t initial_size = view.size();
        
        ParseStatus status = ctx.parser.parse(view, ctx.current_cmd);

        if (status == ParseStatus::SUCCESS) {
            size_t consumed = initial_size - view.size();
            ctx.rb.consume(consumed);

            execute_command(ctx);
            ctx.current_cmd.clear(); 
        } 
        else if (status == ParseStatus::INCOMPLETE) {
            break; 
        } 
        else {
            close_connection(ctx.fd);
            break;
        }
    }
}

void IOThread::execute_command(ClientContext& ctx) {
    const auto& cmd = ctx.current_cmd;
    std::string response;

    if (cmd.name == "GET" && !cmd.args.empty()) {
        auto key = std::string(cmd.args[0]);
        auto val = store.shard(key).get(key);
        response = val ? ("$" + std::to_string(val->size()) + "\r\n" + *val + "\r\n") : "$-1\r\n";
    } 
    else if (cmd.name == "SET" && cmd.args.size() >= 2) {
        auto key = std::string(cmd.args[0]);
        auto val = std::string(cmd.args[1]);
        store.shard(key).set(key, val);
        response = "+OK\r\n";
    } 
    else {
        response = "-ERR unknown command\r\n";
    }

    send(ctx.fd, response.data(), response.size(), MSG_NOSIGNAL);
}

void IOThread::close_connection(int fd) {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
    close(fd);
    clients.erase(fd);
}