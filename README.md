# Sequences

This library allows for iterating chaining, similar to **Rust's iterators** or **Java's Streams API**.

```c++
std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8,
                   9, 10, 11, 12, 13, 14, 15, 16};

// Square all numbers, then take only the even one
// and put them into a string.
auto sequence = makeSequence(v)
        .map<int>([](int n) { return n * n; })
        .filter([](int n) { return n % 2 == 0; })
        .map<std::string>([](int n) {
            std::stringstream ss;
            ss << "Number: " << n;
            return ss.str();
        });

// Take first entry and move sequence the next element.
assert(sequence.next() == "Number: 0");
assert(sequence.next() == "Number: 4");
assert(sequence.next() == "Number: 16");
assert(sequence.next() == "Number: 36");
assert(sequence.next() == "Number: 64");
assert(sequence.next() == "Number: 100");

// Skip 12^2 and 14^2 and move to next element.
assert(sequence.skip(2).next() == "Number: 256");

// After all elements have been consumed, the sequence is empty.
assert(sequence.empty());
```

## Combinators

Currently, the following combinator operations are available and can be chained together:

- `.map(T → R)`: Map a single input argument to an output of any type.
- `.filter(T → Bool)`: Take only elements for which a predicate function returns true.
- `.inspect(T → Void)`: Runs a function on each element, without returning new elements. Useful for printing values.
- `.zip({T})`: Zips to sequences of possibly different types together to sequence of pairs.
- `.map2((T, E) → R)`: Similar to mapping, but the function takes two arguments.
  Works for sequences of pairs, like the ones created by zipping.
  You could use the normal map, but the function would have to accept pairs, and first defining
  local references to the first and second entry, so map2 simply saves boilerplate code.
- `.chain({T})`: Chains a sequence, so that the resulting sequence covers both sequences' elements.
- `.skip(n)`: Skips the first n elements.
- `.skipped(n)`: Like skip, but returns a new sequence instead of modifying the original one.
- `.range(n)`: Creates a new sequence over the first n elements.

`T`, `E` and `R` are arbitrary type parameters, `{T}` denotes a sequence over elements of the type `T` and n ∈ ℕ.

Furthermore, the following operations are available.

- `.forEach(T → Void)`: Runs a function on each element, without returning new elements.
  Unlike `inspect()`, this forces the sequence to immediately iterate over all elements.
- `.count()`: Counts the elements of the sequence.
- `.empty()`: Checks whether a sequences has zero elements.
- `.next()`: Returns the current element, and moves the sequences start to the next element.

The range-based for loop can be used with any sequence to iterate over all elements.

There is one additional, interesting iterator type, `Mutation`.
This is an iterator designed for infinite sequences of values.
Each mutation has a function generating some value based on the value of the previous function output.
The predefined linear mutation generates values of a linear function.
For example, this sequence yields all numbers from 0 to 999, without actually putting 1000 values into memory:

```c++
auto s = makeMutationLinear().range(1000); // 0, 1, 2, ..., 999
```

Of course, the attempt to iterate over all elements of a mutation leads inherently to an infinite loop:

```c++
for(auto n : makeMutationLinear())
{
    printf("%d\n", n); // Prints 0, 1, 2, 3, .... unfinitely
}
```

You can provide own mutations by using `makeMutation()`.
The `makeMutation()` and `makeMutationLinear()` function actually not only creates a mutation
but also wraps it in a sequence.

## Integrity

Sequences can be built upon any type providing a begin and end iterator.
This means all STL containers are supported out of the box.

## Behind the scenes

Each sequence contains two iterators, the current iteration element and the end iterator,
while the end iterator - just as in STL traditions - works like a beyond-the-end marker.
When combinating new sequences, special iterator classes such as `MapIterator` wrap the
sequence. The iterator type is not exposed directly, but rather wrapped in a sequence
to provide a consistent interface.

A sequence does not make any heap allocations. Everything is handled on the stack.
Sequences are lazily evaluated. This means that a map does not run immediately,
but rather only when requested, such as when using `next()` on a sequence.
Data is processed horizontally instead of vertically.
When a sequence combinates two mappings `f` and `g` and than iterates over each element,
then for each element `f` is ran, than `g` and then the result is passed to you.
Afterwards the next iteration starts. This gives cache locality, which would be lost if `f` is firstly ran
on all elements, then `g` runs for each element and only after that, you get each element iteratively.

Sequences do not enforce immutability.
It is possible for mapping to take non-const references instead of copies,
so the underlying elements can be mutated.
This gives additional flexibility.

```c++
int a = 1;
int b = 2;

std::vector<int *> pointers{&a, &b};

auto sequence = makeSequence(pointers).inspect([](int *n) {
    *n = 10;
});

// Original values.
assert(a == 1);
assert(b == 2);

// Run inspection function.
assert(sequence.next() == &a);
assert(sequence.next() == &b);
assert(sequence.empty());

// Now the values are mutated.
assert(a == 10);
assert(b == 10);
```

## Hints

- The iterator type is expected to be lightweight, as it is often copied.
  Usually, iterators are nothing but a pointer anyway.
- Functions given to combinators must be copyable.
  This is important for lambdas which capture variables by value instead of by reference.
