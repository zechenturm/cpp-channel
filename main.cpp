#include <iostream>
#include <thread>
#include "Channel.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    constexpr auto channelSize = 10;

    Channel<int, channelSize> c{};

    std::mutex m;

    auto t = std::thread{[&](){
        int received = 0;
        try {
            while (true)
            {
                received = c.receive();
                m.lock();
                std::cout << "received: " << received << '\n';
                m.unlock();
            }
        }
        catch (Channel<int, channelSize>::ClosedException& ce) {
            std::cout << "channel closed\n";
        }
    }};

    for (auto i = 0; i < 11; ++i)
    {
        c.send(i);
        m.lock();
        std::cout << "sent " << i << '\n';
        m.unlock();
    }

    m.lock();
    std::cout << "closed channel\n";
    m.unlock();
    c.close();

    t.join();

    return 0;
}
