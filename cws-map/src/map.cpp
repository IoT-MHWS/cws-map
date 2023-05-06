#include <cws/map.hpp>

void Layers::nextState() {}

void Map::nextState(const Map & cur) { layers.nextState(); }

std::ostream & operator<<(std::ostream & out, const Map * map) {
  if (map) {
    out << "{dimension:" << &map->dimension << "}";
  }
  return out;
}
