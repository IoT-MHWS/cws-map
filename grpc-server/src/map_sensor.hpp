#pragma once

#include "cws/simulation.hpp"
#include "cwspb/service/map_sensor.grpc.pb.h"
#include "cwspb/service/map_sensor.pb.h"
#include <grpcpp/support/status.h>

class MapSensorService final : public cws::MapSensorService::Service {
private:
  Simulation & simulation;

public:
  MapSensorService(Simulation & simulation) : simulation(simulation) {}
};
