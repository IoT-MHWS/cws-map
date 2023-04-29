#include <iostream>

#include <hello.pb.h>
#include <cws/main.hpp>

int main() {
  cws::Hello msg;
  msg.set_content("hello");
  std::cout << msg.content() << std::endl;
  hello();
}
