#pragma once

#include "cws/subject/extension/network.hpp"
#include "cws/subject/plain.hpp"

namespace Subject {

class NetworkDevice : public Plain, public ExtTransmitter, public ExtReceiver {
public:
  using PacketUPTR = std::unique_ptr<Network::Packet>;

private:
  Network::Type networkType_;
  std::list<PacketUPTR> transmitBuffer_;
  std::list<PacketUPTR> receiveBuffer_;

public:
  NetworkDevice(Plain && plain, Network::Type networkType)
      : Plain(std::move(plain)), networkType_(networkType) {

    setType(Type::NETWORK_DEVICE);
  }

  virtual void
  transmitPackets(std::list<std::unique_ptr<Network::Packet>> && packetList) override;

  virtual std::list<std::unique_ptr<Network::Packet>>
  networkCollectPackets(Network::Type type) override;

  virtual void clearTransmitBuffer() override;

  virtual const std::list<std::unique_ptr<Network::Packet>> &
  getReceivedPackets() override;

  virtual void
  networkPlacePackets(const std::list<std::unique_ptr<Network::Packet>> & packetList,
                      Network::Type type) override;

  virtual void clearReceiveBuffer() override;
};

}// namespace Subject
