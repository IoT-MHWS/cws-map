#pragma once

#include "cws/network/packet.hpp"
#include "cws/subject/plain.hpp"
#include <memory>

using PacketList = std::list<std::unique_ptr<Network::Packet>>;

void addPacketToTransmitQueue(Subject::Plain * plain, void * data);

void setTurnableStatus(Subject::Plain * plain, void * data);
