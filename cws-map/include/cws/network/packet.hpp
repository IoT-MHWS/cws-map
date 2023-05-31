#pragma once

#include <vector>

using byte = unsigned char;

namespace Network {

class Packet {
  std::vector<byte> content_;

public:
  Packet(std::vector<byte> && content) : content_(std::move(content)) {}

  virtual Packet * clone() const { return new Packet(*this); }

  const std::vector<byte> & getContent() const { return content_; }
  const std::size_t getSize() const { return content_.size(); }
};

}// namespace Network
