#include <iostream>
#include <thread>
#include "Channel.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    constexpr auto channelSize = 10;

    Channel<int, channelSize> c{};
    Channel<std::unique_ptr<int>, channelSize> c2{};

    c2.send(std::make_unique<int>(10));
    auto p = c2.receive();

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
