#pragma once

#include "cws/network/container.hpp"
#include "cws/network/type.hpp"
#include <list>
#include <memory>

/*
 * Logic:
 * Put packages in transmitter queue. Then destined 'network' would get all these
 * packages and spread them.
 */
namespace Subject {

class ExtTransmitter {
public:
  virtual void
  transmitPackets(std::list<std::unique_ptr<Network::Packet>> && packetList) = 0;

  virtual std::list<std::unique_ptr<Network::Container>>
  collectNetworkContainers(Network::Type type) const = 0;

  virtual void clearTransmitBuffer() = 0;
};

class ExtReceiver {
public:
  virtual const std::list<std::unique_ptr<Network::Packet>> &
  getReceivedPackets() const = 0;

  virtual void placeNetworkContainers(
      const std::list<std::unique_ptr<Network::Container>> & containerList,
      Network::Type type) = 0;

  virtual void clearReceiveBuffer() = 0;
};

}// namespace Subject
