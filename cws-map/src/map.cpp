#include <cws/map.hpp>

/*
 * Update subject temperature == done
 * Update in-cell temperature == done
 * Update obstruction layer == done
 * Update between-cell temperature
 * Update illumination == done
 *
 */
void Map::update(const Map & cur) {
  layers.subjectLayer.nextTemperature();
  layers.airLayer.nextConvection(layers.subjectLayer);
  layers.airLayer.nextMixing(layers.subjectLayer);

  layers.obstructionLayer.updateObstruction(layers.subjectLayer);
  layers.illuminationLayer.updateIllumination(layers.obstructionLayer,
                                              layers.subjectLayer);
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
