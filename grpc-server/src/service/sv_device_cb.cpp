#include "service/sv_device_cb.hpp"

#include "cws/subject/extension/turnable.hpp"
#include "cws/subject/network.hpp"

void addPacketToTransmitQueue(Subject::Plain * plain, void * data) {
  auto packetList = reinterpret_cast<PacketList *>(data);
  if (auto trans = dynamic_cast<Subject::NetworkDevice *>(plain)) {
    trans->transmitPackets(std::move(*packetList));
  }
}

void setTurnableStatus(Subject::Plain * plain, void * data) {
  auto status = reinterpret_cast<Subject::TurnableStatus *>(data);
  if (auto turnable = dynamic_cast<Subject::ExtTurnable *>(plain)) {
    turnable->setStatus(*status);
  }
}
