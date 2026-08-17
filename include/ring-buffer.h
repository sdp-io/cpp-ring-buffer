#pragma once

#include <concepts>
#include <cstddef>
#include <stdexcept>

// NOTE: Rule of 5 and Rule of 0. Remember what they are and WHY

// NOTE: Remember move semantics, l-values, r-values!

template <std::movable T> class RingBuffer
{
public:
  RingBuffer(const size_t bs) : capacity{bs}, ring_buffer{new T[capacity]} {}

  RingBuffer(const RingBuffer& rb) : capacity{rb.capacity}
  { deep_copy(rb); }

  RingBuffer(RingBuffer&& rb) noexcept
      : capacity{rb.capacity}, ring_buffer{rb.ring_buffer}, read{rb.read}, write{rb.write},
        count{rb.count}
  {
    rb.capacity = 0;
    rb.ring_buffer = nullptr;
    rb.read = 0;
    rb.write = 0;
    rb.count = 0;
  }

  RingBuffer<T>& operator=(const RingBuffer& rb);

  RingBuffer<T>& operator=(RingBuffer&& rb);

  ~RingBuffer()
  { delete[] ring_buffer; }

  // NOTE: This implementation silently overwrites when full. Other implementations
  // May throw or notify the user.
  void enqueue(const T& item);

  void enqueue(T&& item);

  bool is_empty()
  { return count == 0; }

  T dequeue();

private:
  size_t capacity{};
  T* ring_buffer{nullptr};
  size_t read{0};
  size_t write{0};
  size_t count{0};

  void deep_copy(const RingBuffer& source);
};

template <std::movable T> void RingBuffer<T>::enqueue(const T& item)
{
  ring_buffer[write] = item;
  write = (write + 1) % capacity;

  if (count != capacity) {
    count++;
  }
}

template <std::movable T> void RingBuffer<T>::enqueue(T&& item)
{
  ring_buffer[write] = std::move(item);
  write = (write + 1) % capacity;

  if (count != capacity) {
    count++;
  }
}

template <std::movable T> T RingBuffer<T>::dequeue()
{
  // NOTE: Alternatively could have used an assert() for speed, or C++17 std::optional return value.
  // All approaches are valid with their own tradeoffs.
  if (is_empty()) {
    throw std::logic_error("RingBuffer is empty! Did you try calling is_empty() first?\n");
  }

  T item{ring_buffer[read]};
  read = (read + 1) % capacity;

  if (count != 0) {
    count--;
  }

  return item;
}

template <std::movable T> void RingBuffer<T>::deep_copy(const RingBuffer& source)
{
  delete[] ring_buffer;

  ring_buffer = new T[capacity];
  read = 0;
  write = 0;
  count = 0;
  for (size_t i{0}; i < source.count; ++i) {
    size_t idx{(source.read + i) % source.capacity}; // Start from read pointer and wrap around
    ring_buffer[i] = source.ring_buffer[idx];
    count++;
  }
  write = count % capacity;
}

template <std::movable T> RingBuffer<T>& RingBuffer<T>::operator=(const RingBuffer<T>& rb)
{
  if (this == &rb) {
    return *this;
  }

  capacity = rb.capacity;

  deep_copy(rb);

  return *this;
}

template <std::movable T> RingBuffer<T>& RingBuffer<T>::operator=(RingBuffer<T>&& rb)
{
  if (this == &rb) {
    return *this;
  }

  delete[] ring_buffer;

  capacity = rb.capacity;
  ring_buffer = rb.ring_buffer;
  read = rb.read;
  write = rb.write;
  count = rb.count;

  rb.capacity = 0;
  rb.ring_buffer = nullptr;
  rb.read = 0;
  rb.write = 0;
  rb.count = 0;

  return *this;
}
