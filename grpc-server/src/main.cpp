#include <iostream>

#include "service/map_service.hpp"
#include "service/simulation_service.hpp"
#include "service/subject_hub_service.hpp"
#include <cws/simulation/simulation.hpp>
#include <grpcpp/completion_queue.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>

void buildServer(grpc::ServerBuilder & builder, const std::string & address,
                 const int port) {
  std::string uri = address + ":" + std::to_string(port);
  builder.AddListeningPort(uri, grpc::InsecureServerCredentials());
}

template<typename T>
void registerService(grpc::ServerBuilder & builder, T & service) {
  builder.RegisterService(&service);
}

SimulationStateIn getDefaultState() {
  SimulationStateIn state;
  state.simType.set(SimulationType::INFINITE);
  state.simStatus.set(SimulationStatus::STOPPED);
  state.currentTick.set(0);
  state.lastTick.set(0);
  state.taskFrequency.set(1);
  return state;
}

int main(int argc, char * argv[]) {
  SimulationInterface interface;
  SimulationMaster master(interface);

  interface.setSimulationMaster(&master);
  auto state = getDefaultState();
  interface.setState(state);

  interface.run();

  grpc::ServerBuilder builder;

  buildServer(builder, "127.0.0.1", 8200);

  SimulationService simulationService(interface);
  MapService mapService(interface);
  SubjectHubService subjectHubService(interface);

  registerService(builder, simulationService);
  registerService(builder, mapService);
  registerService(builder, subjectHubService);

  auto server(builder.BuildAndStart());

#ifndef NDEBUG
  std::cout << "server: started" << std::endl;
#endif

  server->Wait();
  interface.exit();

  return 0;
}
