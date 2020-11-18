//
// Created by moritz on 11/18/20.
//

#ifndef CHANNEL_RINGBUFFER_HPP
#define CHANNEL_RINGBUFFER_HPP

#include <cstddef>

template<typename T, size_t Size>
class RingBuffer
{
private:
    std::array<T, Size> data;
    size_t readHead = 0, writeHead = 0;

public:
    size_t capacity()
    {
        return Size;
    }

    size_t size()
    {
        return writeHead - readHead;
    }

    bool empty()
    {
        return size() == 0;
    }

    bool full()
    {
        return size() == Size;
    }

    void push(T element)
    {
        if(writeHead == Size) writeHead = 0;
        data[writeHead++] = element;
    }

    T pop()
    {
        if(readHead == Size) readHead = 0;
        return data[readHead++];
    }
};

#endif //CHANNEL_RINGBUFFER_HPP
