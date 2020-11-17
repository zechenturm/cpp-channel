//
// Created by moritz on 11/17/20.
//

#ifndef CHANNEL_CHANNEL_HPP
#define CHANNEL_CHANNEL_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

template <typename T>
class Channel
{
    using lock = std::unique_lock<std::mutex>;
    std::queue<T> queue;
    std::mutex m;
    std::condition_variable cv;
    const size_t maxSize;
    bool isClosed;

public:
    ~Channel() = default;
    Channel(const Channel<T>&) = delete;
    Channel& operator=(const Channel<T>&) = delete;
    Channel(const Channel<T>&&) = delete;
    Channel& operator=(const Channel<T>&&) = delete;

    explicit Channel(size_t size): maxSize(size), isClosed(false) {}

    struct ClosedException: public std::exception
    {
        const char* what() const noexcept override
        {
            return "channel closed";
        }
    };

    void send(T data)
    {
        lock l(m);
        cv.wait(l, [this](){return queue.size() < maxSize || isClosed;});
        if (isClosed) throw ClosedException{};
        queue.push(data);
        cv.notify_one();
    }

    T receive()
    {
        lock l(m);
        cv.wait(l, [this](){return queue.size() > 0 || isClosed;});
        if (isClosed && queue.empty()) throw ClosedException{};
        auto front = queue.front();
        queue.pop();
        cv.notify_one();
        return  front;
    }

    void close()
    {
        lock l(m);
        isClosed = true;
        cv.notify_all();
    }

    bool closed()
    {
        lock l(m);
        return isClosed;
    }
};

#endif //CHANNEL_CHANNEL_HPP
