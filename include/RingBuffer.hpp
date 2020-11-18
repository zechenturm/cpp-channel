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
    bool overflow = false;

public:
    constexpr size_t capacity() const noexcept
    {
        return Size;
    }

    constexpr size_t size() const noexcept
    {
        if(overflow) return (Size - readHead) + writeHead;
        else return writeHead - readHead;
    }

    constexpr bool empty() const noexcept
    {
        return size() == 0;
    }

    constexpr bool full() const noexcept
    {
        return size() == Size;
    }

    constexpr void push(T& element) noexcept
    {
        if(writeHead == Size)
        {
            writeHead = 0;
            overflow = true;
        }
        data[writeHead++] = element;
    }

    constexpr void push(T&& element) noexcept
    {
        if(writeHead == Size)
        {
            writeHead = 0;
            overflow = true;
        }
        data[writeHead++] = std::move(element);
    }

    constexpr T pop() noexcept
    {
        if(readHead == Size)
        {
            readHead = 0;
            overflow = false;
        }
        return std::move(data[readHead++]);
    }
};

#endif //CHANNEL_RINGBUFFER_HPP
