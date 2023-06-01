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

int run(const std::string & host, int port) {
  SimulationInterface interface;
  SimulationMaster master(interface);

  interface.setSimulationMaster(&master);
  auto state = getDefaultState();
  interface.setState(state);

  interface.run();

  grpc::ServerBuilder builder;

  buildServer(builder, host, port);

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

int main(int argc, char * argv[]) {

  try {
    if (argc != 3) {
      throw std::invalid_argument("Invalid count of arguments. Required 3.");
    }

    std::string host = argv[1];
    int port = std::stoi(argv[2]);
    return run(host, port);

  } catch (std::invalid_argument & e) {
    std::cout << e.what() << std::endl;
  }
}
