#include "cws/map_layer/network.hpp"
#include <cassert>
#include <queue>

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

void MapLayerNetworkWireless::updateNetwork(const MapLayerObstruction & obstruction) {
  Dimension dim = getDimension();
  assert(dim == obstruction.getDimension());

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      updateNetworkCell(obstruction, c);
    }
  }
}

Network::WirelessContainer *
cloneApplyingObstruction(const Network::WirelessContainer * source, Coordinates c,
                         const MapLayerObstruction & obsLayer) {
  return source->cloneWithSignal(
      source->getSignalPower() *
      std::max(1 - obsLayer.getWirelessObstruction(c).get(), 0.));
}

void MapLayerNetworkWireless::updateNetworkCell(const MapLayerObstruction & obsLayer,
                                                Coordinates root) {
  Dimension dim = getDimension();
  const auto & transList = getTransmittableContainers(root);
  if (transList.empty())
    return;

  enum Status { UNPROCESSED = 0, IN_QUEUE = 1, PROCESSED = 2 };

  std::vector<std::vector<Status>> infoV(
      dim.width, std::vector<Status>(dim.height, Status::UNPROCESSED));

  std::queue<Coordinates> queue;

  auto addNeighboursQV = [&](Coordinates c) -> void {
    for (const auto & neigh : getNeighbours(dim, c)) {
      auto & infoVN = infoV[neigh.x][neigh.y];
      if (infoVN == UNPROCESSED) {
        queue.push(neigh);
        infoVN = IN_QUEUE;
      }
    }
  };

  // put all wireless containers in receivable and apply obstruction
  {
    auto & receivList = getReceivableContainers(root);
    for (const auto & trans : transList) {
      auto transWireless = static_cast<Network::WirelessContainer *>(trans.get());
      receivList.emplace_back(cloneApplyingObstruction(transWireless, root, obsLayer));
    }
    infoV[root.x][root.y] = PROCESSED;
    addNeighboursQV(root);
  }

  while (!queue.empty()) {
    Coordinates u = queue.front();
    queue.pop();

    auto uNeighs = getNeighbours(dim, u);
    auto v_ur = getVector(u, root);

    Coordinates best_n;
    auto best_sm = -1;
    auto best_d_un = 0;

    for (const auto & n : uNeighs) {
      auto v_un = getVector(u, n);
      auto sm_n = getScalarMultiplication(v_ur, v_un);
      auto d_un = getDistanceSquare(v_un);

      if (sm_n > best_sm || (sm_n == best_sm && d_un < best_d_un)) {
        best_n = n;
        best_sm = sm_n;
        best_d_un = d_un;
      }
    }

    const auto & bestNList = getReceivableContainers(best_n);
    auto & receiUList = getReceivableContainers(u);

    for (const auto & bestN : bestNList) {
      auto bestNWireless = static_cast<Network::WirelessContainer *>(bestN.get());
      receiUList.emplace_back(cloneApplyingObstruction(bestNWireless, u, obsLayer));
    }

    addNeighboursQV(u);
  }
}
