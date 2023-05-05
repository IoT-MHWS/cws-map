#pragma once

#include "cws/simulation/interface.hpp"
#include "cwspb/service/map.grpc.pb.h"
#include "cwspb/service/map.pb.h"
#include <grpcpp/support/status.h>

class MapService final : public cws::MapService::Service {
private:
  SimulationInterface & interface;

public:
  MapService(SimulationInterface & interface) : interface(interface) {}
};
