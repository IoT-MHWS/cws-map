#pragma once

#include "cws/layer/network.hpp"
#include "cws/map_layer/base.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/network/type.hpp"

class MapLayerNetwork {
  Network::Type networkType_;

  MapLayerBase<LayerNetwork> layerTransmittable_;// stores containers to be transmitted
  MapLayerBase<LayerNetwork> layerReceivable_;   // stores containers to be received

public:
  MapLayerNetwork(Dimension dimension, Network::Type type)
      : layerTransmittable_(dimension), layerReceivable_(dimension),
        networkType_(type) {}

  MapLayerNetwork(Dimension dimension)
      : MapLayerNetwork(dimension, Network::Type::UNSPECIFIED) {}

  const std::list<std::unique_ptr<Network::Container>> &
  getTransmittableContainers(Coordinates c) const {
    return layerTransmittable_.getCell(c).getElement().getContainerList();
  }

  const std::list<std::unique_ptr<Network::Container>> &
  getReceivableContainers(Coordinates c) const {
    return layerReceivable_.getCell(c).getElement().getContainerList();
  }

  Network::Type getNetworkType() const { return networkType_; }
  Dimension getDimension() const;

  void clearNetwork();
  virtual void collectTransmittableContainers(const MapLayerSubject & layerSubject);
  virtual void updateNetwork(const MapLayerObstruction & obstruction) = 0;

private:
  std::list<std::unique_ptr<Network::Container>> &
  getTransmittableContainers(Coordinates c) {
    return layerTransmittable_.accessCell(c).accessElement().getContainerList();
  }

  std::list<std::unique_ptr<Network::Container>> &
  getReceivableContainers(Coordinates c) {
    return layerReceivable_.accessCell(c).accessElement().getContainerList();
  }
};

class MapLayerNetworkWireless : public MapLayerNetwork {
public:
  MapLayerNetworkWireless(Dimension dimension)
      : MapLayerNetwork(dimension, Network::Type::WIRELESS) {}

  void updateNetwork(const MapLayerObstruction & obstruction) override;
};
