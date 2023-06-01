#include "cws/subject/network.hpp"
#include "cws/common.hpp"
#include "cws/map_layer/network.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "gtest/gtest.h"

TEST(MapLayersNetworkWireless, updateNetworkUSE) {
  using namespace Subject;

  Dimension dim{4, 4};

  std::vector<std::vector<double>> obs_layer{
      {1., 0.2, 0.4, 0.1},
      {0.2, 0.2, 0.0, 0.1},
      {0.2, 0.8, 0.0, 0.1},
      {0.2, 0.0, 0.4, 0.5},
  };
  MapLayerObstruction obstruction(dim);

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      Obstruction obs;
      Illumination ill;
      if (c.x < 4 && c.y < 4) {
        obs = Obstruction{obs_layer[c.x][c.y]};
      } else {
        obs = {0};
        ill = {0};
      }
      obstruction.setWirelessObstruction(c, obs);
    }
  }

  MapLayerSubject layerSubject(dim);

  {
    layerSubject.accessSubjectList({1, 1}).push_back(
        std::make_unique<WirelessNetworkDevice>(Plain({}, 1, 10, {}), 100, 20));
  }

  auto networkDevice = static_cast<Subject::NetworkDevice *>(
      layerSubject.getSubjectList({1, 1}).begin()->get());

  MapLayerNetworkWireless wirelessNetwork(dim);

  {
    std::string s1 = "hello world!@";

    const std::byte * byteA = reinterpret_cast<const std::byte *>(s1.data());
    std::vector<std::byte> byteV(byteA, byteA + s1.size());

    Network::Packet packet1(std::move(byteV));

    std::list<std::unique_ptr<Network::Packet>> packetList;
    packetList.emplace_back(packet1.clone());
    packetList.emplace_back(packet1.clone());
    packetList.emplace_back(packet1.clone());
    packetList.emplace_back(packet1.clone());

    networkDevice->transmitPackets(std::move(packetList));
  }

  wirelessNetwork.clearNetwork();
  wirelessNetwork.collectTransmittableContainers(layerSubject);
  wirelessNetwork.updateNetwork(obstruction);
  layerSubject.receiveContainers(wirelessNetwork);

  for (const auto & packet : networkDevice->getReceivedPackets()) {
    std::string s(reinterpret_cast<const char *>(packet->getContent().data()),
                  packet->getContent().size());
    std::cout << s << std::endl;
  }
}
