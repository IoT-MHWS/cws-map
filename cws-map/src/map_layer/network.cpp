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
      layerTransmittable_.accessCell(c).accessElement().getPacketList().clear();
      layerReceivable_.accessCell(c).accessElement().getPacketList().clear();
    }
  }
}

void MapLayerNetwork::collectTransmittablePackages(
    const MapLayerSubject & layerSubject) {
  Dimension dim = getDimension();
  assert(layerSubject.getDimension() == dim);

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {

      const auto & transmitters =
          layerSubject.getCell(c).getElement().getNetworkTransmitters();

      std::list<std::unique_ptr<Network::Packet>> transPackets;

      for (auto transmitter : transmitters) {
        transPackets.splice(transPackets.end(),
                            transmitter->collectNetworkPackets(networkType_));
      }

      auto & layerPackets = getTransmittablePackages(c);
      layerPackets.splice(layerPackets.end(), transPackets);
    }
  }
}

// algorithm to spread packets
void MapLayerNetworkWireless::updateNetwork(const MapLayerObstruction & obstruction) {}
