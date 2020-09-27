# Flow

This is an experimental project for implementing sequences, also known as pull iterators from Rust or streams from Java, in C++ 17.

It aims to be as extensible as possible, meaning that sequences defined in this project such as `map` are implemented using the exact same toolset as custom sequences would use. In its current implementation, as sequence composition would look like this:

```c++

auto s = successors(1)
    | filter ([] (int n) { return n % 2 == 0; })   // Only even numbers.
    | map ([] (int n) { return std::pow(2, n); })  // Exponentiation to base 2.
    | enumerate();                                 // Index each element.

for (auto [i, n] : s | take(10)) {
    std::cout << "Element no. " << i << ": " << n << std::endl;
    
    // Element no. 0: 4  (2^2)
    // Element no. 1: 16 (2^4)
    // Element no. 2: 64 (2^6)
    // ...
}

```
