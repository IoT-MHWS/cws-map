#include <cws/general.hpp>
#include <cws/concurrent_map.hpp>
#include <cws/map.hpp>

int main() {
  Map map(Dimension{.width = 5, .height = 10});
  std::cout << map.getDimension().width << std::endl;
  map.nextState();

  ConcurrentMap cmap(Dimension{.width = 5, .height = 15});
  std::cout << cmap.getDimension().width << std::endl;
  map.nextState();
}
