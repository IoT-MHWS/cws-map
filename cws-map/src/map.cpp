#include <cws/map.hpp>

/*
 * Current state (this) of map is equal to passed by argument
 *
 * Update subject temperature == done
 * Update convection (in-cell) temperature == done
 * Update air obstruction == done
 * Update circulation (between-cell) temperature == done
 * Update light obstruction == done
 * Update illumination == done
 *
 * Clear wireless network from previous frame == done
 * Collect packages from subjects == done
 * Update network (spread containers where packets are stored)
 * Place packages in subjects == done
 */
void Map::next(const Map & curMap) {
  layers.subjectLayer.nextTemperature();
  layers.airLayer.nextConvection(layers.subjectLayer);
  layers.obstructionLayer.updateAirObstruction(layers.subjectLayer);
  layers.airLayer.nextCirculation(curMap.layers.airLayer, layers.obstructionLayer);
  layers.obstructionLayer.updateLightObstruction(layers.subjectLayer);
  layers.illuminationLayer.updateIllumination(layers.obstructionLayer,
                                              layers.subjectLayer);
  // clear network from previous state
  layers.networkWireless.clearNetwork();
  layers.networkWireless.collectTransmittableContainers(layers.subjectLayer);
  layers.networkWireless.updateNetwork(layers.obstructionLayer);
  layers.subjectLayer.receiveContainers(layers.networkWireless);
  // like cameras and so on
  layers.subjectLayer.setupSubjects(layers.obstructionLayer, layers.illuminationLayer);
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
