#include "server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

Server::Server(int port, int threads) {
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(listen_fd, SOMAXCONN) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < threads; i++) {
        workers.push_back(std::make_unique<IOThread>(i));
    }

    std::cout << "Server started on port " << port << " with " << threads << " threads." << std::endl;
}

void Server::run() {
    for (auto& worker : workers) {
        worker_threads.emplace_back(&IOThread::run, worker.get());
    }

    int current_worker = 0;
    while (true) {
        int client_fd = accept(listen_fd, nullptr, nullptr);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }

        workers[current_worker]->add_client(client_fd);
        current_worker = (current_worker + 1) % workers.size();
    }
}