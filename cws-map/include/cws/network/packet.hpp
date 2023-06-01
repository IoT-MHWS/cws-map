#pragma once

#include <string>
#include <vector>

namespace Network {

class Packet {
  std::vector<std::byte> content_;

public:
  Packet(std::vector<std::byte> && content) : content_(content) {}

  virtual ~Packet() = default;

  virtual Packet * clone() { return new Packet(*this); }

  const std::vector<std::byte> & getContent() const { return content_; }
};

};// namespace Network
