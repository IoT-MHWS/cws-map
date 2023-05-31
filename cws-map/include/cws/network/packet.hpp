#pragma once

#include <vector>

namespace Network {

using byte = unsigned char;

class Packet {
  std::vector<byte> content_;

public:
  Packet(std::vector<byte> && content) : content_(content) {}

  virtual ~Packet() = default;

  virtual Packet * clone() { return new Packet(*this); }
};

};// namespace Network
