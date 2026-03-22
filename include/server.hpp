#include <vector>
#include <memory>
#include <thread>
#include "io_thread.hpp"

class Server {

public:

    Server(int port,int threads);

    void run();

private:

    int listen_fd;

    std::vector<std::unique_ptr<IOThread>> workers;

    std::vector<std::thread> worker_threads;
};