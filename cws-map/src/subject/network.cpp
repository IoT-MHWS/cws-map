#include "cws/subject/network.hpp"

namespace Subject {

using namespace Network;

void NetworkDevice::transmitPackets(
    std::list<std::unique_ptr<Packet>> && containerList) {

  for (auto && container : containerList) {
    transmitPackets_.push_back(std::move(container));
  }
}

void NetworkDevice::clearTransmitBuffer() { transmitPackets_.clear(); }

const std::list<std::unique_ptr<Packet>> & NetworkDevice::getReceivedPackets() const {
  return receivedPackets_;
}

void NetworkDevice::clearReceiveBuffer() { receivedPackets_.clear(); }

std::list<std::unique_ptr<Container>>
WirelessNetworkDevice::collectNetworkContainers(Network::Type type) const {
  std::list<std::unique_ptr<Container>> result;

  if (type == Network::Type::WIRELESS) {
    for (const auto & packet : transmitPackets_) {
      result.push_back(std::make_unique<WirelessContainer>(
          std::unique_ptr<Packet>(packet->clone()), getTransmitPower()));
    }
  }

  return result;
}

void WirelessNetworkDevice::placeNetworkContainers(
    const std::list<std::unique_ptr<Container>> & containerList, Network::Type type) {

  if (type == Network::Type::WIRELESS) {
    for (const auto & container : containerList) {
      receivedPackets_.push_back(
          std::unique_ptr<Packet>(container->getPacket()->clone()));
    }
  }
}

};// namespace Subject
