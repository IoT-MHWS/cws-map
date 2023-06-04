#pragma once

#include "converters.hpp"
#include "cws/simulation/interface.hpp"
#include "cwspb/service/sv_simulation.grpc.pb.h"
#include <grpcpp/support/status.h>

class SimulationService final : public cwspb::SimulationService::Service {
private:
  SimulationInterface & interface;

public:
  SimulationService(SimulationInterface & simulation) : interface(simulation) {}

  grpc::Status GetSimulationState(::grpc::ServerContext * context,
                                  const cwspb::Request * request,
                                  cwspb::ResponseSimulationState * response) override {
    auto stateI = interface.getState();
    auto state = response->mutable_state();
    toSimulationState(*state, stateI);
    auto base = response->mutable_base();
    auto status = base->mutable_status();

    return grpc::Status::OK;
  }

  grpc::Status SetSimulationState(::grpc::ServerContext * context,
                                  const cwspb::RequestSimulationState * request,
                                  cwspb::Response * response) override {
    const auto & stateI = request->state();
    SimulationStateIn state = fromSimulationState(stateI);
    interface.setState(state);

    return grpc::Status::OK;
  }
};
