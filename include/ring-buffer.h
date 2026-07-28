#pragma once

#include <concepts>
#include <cstddef>

template <std::movable T> class RingBuffer
{
public:
  RingBuffer(const size_t bs) : capacity{bs}, ring_buffer{new T[capacity]} {}

  ~RingBuffer()
  { delete[] ring_buffer; }

  void enqueue(const T& item);

  // NOTE: Remember move semantics, l-values, r-values!
  void enqueue(const T&& item);

  T dequeue();

private:
  size_t capacity{};
  T* ring_buffer{nullptr};
  size_t read{0};
  size_t write{0};
  size_t count{0};

  void deep_copy(const RingBuffer& source)
  {
    delete[] ring_buffer;

    capacity = source.capacity;

    ring_buffer = new T[capacity];

    while (source.count > 0) {
      this->enqueue(source.dequeue());
    }
  }
};

template <std::movable T> void RingBuffer<T>::enqueue(const T& item)
{
  ring_buffer[write] = item;
  write = (write + 1) % capacity;

  if (count != capacity) {
    count++;
  }
}

template <std::movable T> T RingBuffer<T>::dequeue()
{
  T item{ring_buffer[read]};
  read = (read + 1) % capacity;

  if (count != 0) {
    count--;
  }

  return item;
}
