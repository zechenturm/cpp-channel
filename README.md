# C++ Channel

This is a simple implementation for GO-like channels for communication between threads.

## How it Works

A Channel works a lot like a thread safe queue of fixed (maximum) size.
Things can be sent (pushed) to the channel and received (popped) from the channel.
Notably, sending to a channel will block if the queue is full until there is space again (i.e. someone else has received from the channel).
Likewise, if a channel is empty, receiving from it will block until new data is available (i.e someone sent something).
To efficiently block `Channel` use `std::condition_variable` internally.

The size of channel determines how many elements the channel can buffer internally, i.e. how many elements can be sent before blocking until space is freed up by receiving an element.

### Writing

Data can be sent to a channel using the `send()` method or by means of the `<<` operrator.

```
// create a channel of size 1
auto c = Channel<int, 2>{};
//send data
c.send(1);
// this will block until someone receives 1
c << 2;
```

### Reading

Likewise, data can be received using `receive()` or the `>>` operator.
```
int a;
c >> a; // a = 1
const int b = c.reveive(); // b = 2
```

Since either will block until new data is available, it can easily be used in a loop:

```
while (true)
    {
        c >> received;
        m.lock();
        std::cout << "received: " << received << '\n';
        m.unlock();
    }
```

### Closing a channel

To indicate that a channel can no longer transfer data it can be closed.
Sending to a closed channel will result in a `Channel::ClosedException` exception.
Receiving from a closed channel works as long as there is still data queued in it.
Once all available data is received, receiving from a closed channel will also yield a `Channel::ClosedException` exception.
This can be used in conjunction with an infinite loop to receive all data until the channel is closed:

```
try
{
    while (true)
    {
        c >> received;
        m.lock();
        std::cout << "received: " << received << '\n';
        m.unlock();
    }
}
catch (Channel<int, 2>::ClosedException& ce)
{
    std::cout << "channel closed\n";
}
```