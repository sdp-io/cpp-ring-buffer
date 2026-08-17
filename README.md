# RingBuffer

A generic, header-only circular buffer in C++20, built as an educational template and 
move semantics exercise before starting a larger C++ performance library project.

## Features
- `std::movable` concept constraint on the template parameter
- Rule of five implemented explicitly with correct move semantics
- Manual heap management via `new[]` and `delete[]` to implement rule of five from scratch
- Circular indexing via modulo arithmetic
- Exception-based bounds checking on empty dequeue usage

## Build and Run

```bash
cmake -B build

cmake --build build

./build/main
```

## Why raw pointers instead of std::vector?
This project was built specifically to practice manual resource management 
and the rule of five. Using std::vector would utilize its preexisting RAII,
hindering the overarching educational goal for this project.
