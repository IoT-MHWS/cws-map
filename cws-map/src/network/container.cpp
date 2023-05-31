#include "cws/network/container.hpp"

using namespace Network;

WirelessContainer * WirelessContainer::cloneWithSignal(double signalPower) const {
  auto eptr = this->clone();
  eptr->setSignalPower(signalPower);
  return eptr;
}
