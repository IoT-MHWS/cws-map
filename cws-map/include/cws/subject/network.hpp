#pragma once

#include "cws/subject/extension/network.hpp"
#include "cws/subject/plain.hpp"

namespace Subject {

class NetworkDevice : public Plain, public ExtTransmitter, public ExtReceiver {
public:
  using PacketUPTR = std::unique_ptr<Network::Packet>;

protected:
  std::list<PacketUPTR> transmitPackets_;
  std::list<PacketUPTR> receivedPackets_;

public:
  NetworkDevice(Plain && plain, Network::Type networkType) : Plain(std::move(plain)) {}

  virtual void transmitPackets(
      std::list<std::unique_ptr<Network::Packet>> && containerList) override;

  void clearTransmitBuffer() final override { transmitPackets_.clear(); }

  const std::list<std::unique_ptr<Network::Packet>> & getTransmitPackets() const {
    return transmitPackets_;
  }

  const std::list<std::unique_ptr<Network::Packet>> &
  getReceivedPackets() const final override {
    return receivedPackets_;
  }

  void clearReceiveBuffer() final override { receivedPackets_.clear(); }
};

class WirelessNetworkDevice : public NetworkDevice {
private:
  int transmitPower_;
  int receiveThresh_;

public:
  WirelessNetworkDevice(Plain && plain, int transmitPower, int receiveThresh)
      : NetworkDevice(std::move(plain), Network::Type::WIRELESS),
        transmitPower_(transmitPower), receiveThresh_(receiveThresh) {
    setType(Type::WIRELESS_NETWORK_DEVICE);
  }

  int getTransmitPower() const { return transmitPower_; }

  int getReceiveThresh() const { return receiveThresh_; }

  virtual std::list<std::unique_ptr<Network::Container>>
  collectNetworkContainers(Network::Type type) const override;

  virtual void placeNetworkContainers(
      const std::list<std::unique_ptr<Network::Container>> & containerList,
      Network::Type type) override;
};

}// namespace Subject
