#pragma once

#include "cws/network/packet.hpp"
#include <memory>
#include <vector>

namespace Network {

// container of message, packet, whatever protocols?
class Container {
  std::unique_ptr<Packet> packet_;

public:
  Container(std::unique_ptr<Packet> && packet) : packet_(std::move(packet)) {}

  Container(const Container & obj) noexcept { packet_.reset(obj.packet_->clone()); }

  Container & operator=(const Container & obj) noexcept {
    if (this == &obj) {
      return *this;
    }
    packet_.reset(obj.packet_->clone());
    return *this;
  }

  virtual ~Container() = default;

  virtual Container * clone() const = 0;

  const std::unique_ptr<Packet> & getPacket() const { return packet_; }
};

class WirelessContainer : public Container {
  double signalPower_;

public:
  WirelessContainer(std::unique_ptr<Packet> && packet, double signalPower)
      : Container(std::move(packet)), signalPower_(signalPower) {}

  WirelessContainer * clone() const override { return new WirelessContainer(*this); }
  WirelessContainer * cloneWithSignal(double signalPower) const;

  double getSignalPower() const { return signalPower_; }

protected:
  void setSignalPower(double signalPower) { signalPower_ = signalPower; }
};

}// namespace Network
