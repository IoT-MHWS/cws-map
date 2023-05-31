#pragma once

#include "cws/layer/base.hpp"
#include "cws/network/packet.hpp"
#include <list>
#include <memory>

class LayerNetwork : public Layer {
public:
  using PacketUPTR = std::unique_ptr<Network::Packet>;

private:
  std::list<PacketUPTR> packetList;

public:
  LayerNetwork() = default;

  LayerNetwork(LayerNetwork && obj) noexcept : Layer(std::move(obj)) {
    this->packetList = std::move(obj.packetList);
  }

  LayerNetwork(const LayerNetwork & obj) noexcept : Layer(obj) {
    packetList.clear();
    for (const auto & e : obj.packetList) {
      packetList.push_back(std::unique_ptr<Network::Packet>(e->clone()));
    }
  }

  LayerNetwork & operator=(LayerNetwork && obj) noexcept {
    if (this == &obj) {
      return *this;
    }

    Layer::operator=(std::move(obj));

    this->packetList = std::move(obj.packetList);

    return *this;
  }

  LayerNetwork & operator=(const LayerNetwork & obj) noexcept {
    if (this == &obj) {
      return *this;
    }

    Layer::operator=(obj);

    packetList.clear();
    for (const auto & e : obj.packetList) {
      packetList.push_back(std::unique_ptr<Network::Packet>(e->clone()));
    }

    return *this;
  }

  const std::list<PacketUPTR> & getPacketList() const { return packetList; }

  std::list<PacketUPTR> & getPacketList() { return packetList; }
};
