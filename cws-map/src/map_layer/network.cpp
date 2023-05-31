#include "cws/map_layer/network.hpp"
#include <cassert>

Dimension MapLayerNetwork::getDimension() const {
  return layerTransmittable_.getDimension();
}

void MapLayerNetwork::clearNetwork() {
  Dimension dim = getDimension();
  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      layerTransmittable_.accessCell(c).accessElement().getContainerList().clear();
      layerReceivable_.accessCell(c).accessElement().getContainerList().clear();
    }
  }
}

void MapLayerNetwork::collectTransmittableContainers(
    const MapLayerSubject & layerSubject) {
  Dimension dim = getDimension();
  assert(layerSubject.getDimension() == dim);

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {

      const auto & transmitters =
          layerSubject.getCell(c).getElement().getNetworkTransmitters();

      std::list<std::unique_ptr<Network::Container>> transContainers;

      for (auto transmitter : transmitters) {
        transContainers.splice(transContainers.end(),
                               transmitter->collectNetworkContainers(networkType_));
      }

      auto & layerContainers = getTransmittableContainers(c);
      layerContainers.splice(layerContainers.end(), transContainers);
    }
  }
}

// algorithm to spread containers
// Here containers are only of type Network::WirelessContainers on it's derived
// static_cast them!
void MapLayerNetworkWireless::updateNetwork(const MapLayerObstruction & obstruction) {}
