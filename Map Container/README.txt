A container template class Map that is very similar to std::map from the C++ Standard Library. It is implemented with a non-recursive but iterative threaded AVL Search tree as the data structure. This container implements key-value pairs, where key and value may be 
any types, including class types.  

This one satisfies following performance requirements:
O(log(N)) for key lookup, insertion and deletion
O(1) for iterator increments and decrements
O(N log(N)) for copy constructor and assginment operator.

Note: I am still working on making copy time from O(N log(N)) to 
O(N). That way I did copy constructor was to insert each element of a binary tree to the other one. However, a recursive function that 
traverses an entire may be the solution. 