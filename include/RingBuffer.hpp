//
// Created by moritz on 11/18/20.
//

#ifndef CHANNEL_RINGBUFFER_HPP
#define CHANNEL_RINGBUFFER_HPP

#include <cstddef>
#include <array>

template<typename T, size_t Size>
class RingBuffer
{
private:
    std::array<T, Size> data;
    size_t readHead = 0, writeHead = 0;

public:
    constexpr size_t capacity() const noexcept
    {
        return Size;
    }

    constexpr size_t size() const noexcept
    {
        return writeHead - readHead;
    }

    constexpr bool empty() const noexcept
    {
        return size() == 0;
    }

    constexpr bool full() const noexcept
    {
        return size() == Size;
    }

    constexpr void push(T element) noexcept
    {
        if(writeHead == Size) writeHead = 0;
        data[writeHead++] = element;
    }

    constexpr T pop() noexcept
    {
        if(readHead == Size) readHead = 0;
        return data[readHead++];
    }
};

#endif //CHANNEL_RINGBUFFER_HPP
