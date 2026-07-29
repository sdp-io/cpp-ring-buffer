#include "ring-buffer.h"
#include <iostream>

int main()
{
  RingBuffer<int> rb1{10};

  rb1.enqueue(1);
  rb1.enqueue(2);
  rb1.enqueue(3);
  rb1.enqueue(4);
  rb1.enqueue(5);

  int v1{rb1.dequeue()};
  int v2{rb1.dequeue()};
  int v3{rb1.dequeue()};

  std::cout << "v1: " << v1 << "\n";
  std::cout << "v2: " << v2 << "\n";
  std::cout << "v3: " << v3 << "\n";

  std::cout << "Testing copy constructor. . .\n";

  RingBuffer<int> rb2{rb1};

  std::cout << "rb2 is_empty(): " << rb2.is_empty() << std::endl;
  rb2.dequeue();
  rb2.dequeue();

  std::cout << "Testing overloaded assignment operator. . .\n";

  rb1 = rb2;

  std::cout << "Testing move constructor. . .\n";

  RingBuffer<int> rb3{std::move(rb1)};

  for (int i{0}; i < 100; ++i) {
    // NOTE: Commented out for testing std::logic_error throw on empty buf removal
    // rb3.dequeue();
    rb2.enqueue(i);
  }

  for (int i{0}; i < 10; ++i) {
    // NOTE: Commented out for testing std::logic_error throw on empty buf removal
    // std::cout << "rb1 item: " << rb1.dequeue() << "\n";
    std::cout << "rb2 item: " << rb2.dequeue() << "\n";
  }
}
