#include "server.hpp"

int main()
{
    Server server(6379,8);

    server.run();
}