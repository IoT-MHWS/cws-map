#include <iostream>

#include <hello.pb.h>

int main() {
  cws::Hello hello;
  hello.set_content("hello");
  std::cout << hello.content() << std::endl;
}
