#pragma once

#include "cws/simulation.hpp"
#include "cwspb/service/map.grpc.pb.h"
#include "cwspb/service/map.pb.h"
#include <grpcpp/support/status.h>

class MapService final : public cws::MapService::Service {
private:
  Simulation & simulation;

public:
  MapService(Simulation & simulation) : simulation(simulation) {}
};
