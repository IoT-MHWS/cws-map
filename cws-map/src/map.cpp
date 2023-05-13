#include <cws/map.hpp>

/*
 * 1) Update temperature layer
 * 1.1) Update in-cell temperature
 * 1.2) Update between-cell temperature
 * 2) Update absorption layer == done
 * 3) Update illumination == done
 *
 */
void Map::update(const Map & cur) {
  layers.absorptionLayer.update(layers.subjectLayer);
  layers.illuminationLayer.update(layers.absorptionLayer, layers.subjectLayer);
}

std::ostream & operator<<(std::ostream & out, const Layers * layers) {
  out << "(" << (void *)layers << ")";
  // if (layers) {
  //   out << "{" << std::endl;
  //   out << "  temp:" << &layers->temperatureLayer << "," << std::endl;
  //   out << "  subject:" << &layers->subjectLayer << std::endl;
  //   out << "}";
  // }
  return out;
}

std::ostream & operator<<(std::ostream & out, const Map * map) {
  out << "(" << (void *)map << ")";
  // if (map) {
  //   out << "{" << std::endl;
  //   out << "  dimension:" << &map->dimension << "," << std::endl;
  //   out << "  layers:" << &map->layers << std::endl;
  //   out << "}";
  // }
  return out;
}
