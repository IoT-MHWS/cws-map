#include "cws/general.hpp"
#include <cws/map.hpp>

int main() {
  Map map(Dimension{.width = 5, .height = 10});
  std::cout << map.getDimension().width << std::endl;
  map.buildNextState();
}
