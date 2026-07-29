#pragma once

#include <concepts>
#include <cstddef>
#include <stdexcept>

// NOTE: Rule of 5 and Rule of 0. Remember what they are and WHY

template <std::movable T> class RingBuffer
{
public:
  RingBuffer(const size_t bs) : capacity{bs}, ring_buffer{new T[capacity]} {}

  RingBuffer(const RingBuffer& rb) : capacity{rb.capacity}
  { deep_copy(rb); }

  RingBuffer(const RingBuffer&& rb) : capacity{rb.capacity}
  { deep_copy(rb); }

  RingBuffer<T>& operator=(const RingBuffer& rb);

  ~RingBuffer()
  { delete[] ring_buffer; }

  // NOTE: Remember move semantics, l-values, r-values!
  void enqueue(const T& item)
  { add_item(item); }

  void enqueue(const T&& item)
  { add_item(item); }

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

  void add_item(const T& item);
};

template <std::movable T> void RingBuffer<T>::add_item(const T& item)
{
  ring_buffer[write] = item;
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

  for (size_t i{0}; i < source.count; ++i) {
    ring_buffer[i] = source.ring_buffer[i];
    count++;
  }
}

template <std::movable T> RingBuffer<T>& RingBuffer<T>::operator=(const RingBuffer<T>& rb)
{
  capacity = rb.capacity;

  deep_copy(rb);

  return *this;
}
