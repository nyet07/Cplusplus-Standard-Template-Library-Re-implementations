Non-intrusive, thread-safe, reference-counting smart pointer.
It has really similar properties to std::shared_ptr.

It allows different smart pointers in different threads to be safely used on the same shared objects. It uses a type that allows atomic 
increment and decrement operations to accomplish thread-safe usage.