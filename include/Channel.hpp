//
// Created by moritz on 11/17/20.
//

#ifndef CHANNEL_CHANNEL_HPP
#define CHANNEL_CHANNEL_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include "RingBuffer.hpp"

template <typename T, size_t Size>
class Channel
{
    using lock = std::unique_lock<std::mutex>;
    RingBuffer<T, Size> queue;
    std::mutex m;
    std::condition_variable cv;
    bool isClosed = false;

public:
    ~Channel() = default;
    Channel() = default;
    Channel(const Channel<T, Size>&) = delete;
    Channel& operator=(const Channel<T, Size>&) = delete;
    Channel(const Channel<T, Size>&&) = delete;
    Channel& operator=(const Channel<T, Size>&&) = delete;

    struct ClosedException: public std::exception
    {
        const char* what() const noexcept override
        {
            return "channel closed";
        }
    };

    template<typename D>
    void send(D&& data)
    {
        lock l(m);
        cv.wait(l, [this](){return !queue.full() || isClosed;});
        if (isClosed) throw ClosedException{};
        queue.push(std::forward<D>(data));
        cv.notify_one();
    }

    T receive()
    {
        lock l(m);
        cv.wait(l, [this](){return !queue.empty() || isClosed;});
        if (isClosed && queue.empty()) throw ClosedException{};
        auto front = queue.pop();
        cv.notify_one();
        return std::move(front);
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

    template<typename D>
    Channel& operator<<(D&& data)
    {
        send(std::forward<D>(data));
        return *this;
    }

    Channel& operator>>(T& data)
    {
        data = std::move(receive());
        return *this;
    }
};

#endif //CHANNEL_CHANNEL_HPP
