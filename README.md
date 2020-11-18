# C++ Channel

This is a simple implementation for GO-like channels for communication between threads.

## How to Use

A Channel works a lot like a thread safe queue of fixed (maximum) size.
Things can be sent (pushed) to the channel and received (popped) from the channel.
Notably, sending to a channel will block if the queue is full until there is space again (i.e. someone else has received from the channel).
Likewise, if a channel is empty, receiving from it will block until new data is availble (i.e someone sent something).
To efficiently block `Channel` use `std::condition_variable` internally.

Data can be sent to a channel using the `send()` method or by means of the `<<` operrator.

```
// create a channel of size 1
auto c = Channel<int, 2>{};
//send data
c.send(1);
// this will block until someone receives 1
c << 2;
```

Likewise, data can be received using `receive()` or the `>>` operator.
```
int a;
c >> a; // a = 1
const int b = c.reveive(); // b = 2
```

