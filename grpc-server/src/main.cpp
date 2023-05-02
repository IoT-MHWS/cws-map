#include <iostream>

#include "grpcpp/completion_queue.h"
#include "grpcpp/security/server_credentials.h"
#include "grpcpp/server_builder.h"
#include "map_sensor.hpp"
#include "map.hpp"
#include "simulation.hpp"


void buildServer(grpc::ServerBuilder & builder, const std::string & address,
                 const int port) {
  std::string uri = address + ":" + std::to_string(port);
  builder.AddListeningPort(uri, grpc::InsecureServerCredentials());
}

template<typename T>
void registerService(grpc::ServerBuilder & builder, T & service) {
  builder.RegisterService(&service);
}


int main(int argc, char * argv[]) {
  grpc::ServerBuilder builder;

  buildServer(builder, "127.0.0.1", 8200);

  SimulationService simulationService;
  MapService mapService;
  MapSensorService sensorService;

  registerService(builder, simulationService);
  registerService(builder, mapService);
  registerService(builder, sensorService);

  auto server(builder.BuildAndStart());

#ifndef NDEBUG
    std::cout << "server started" << std::endl;
#endif

  server->Wait();
  return 0;
}
