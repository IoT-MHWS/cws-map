#include "cwspb/general.pb.h"
#include "grpcpp/create_channel.h"
#include "grpcpp/security/credentials.h"
#include <cws/concurrent_map.hpp>
#include <cws/general.hpp>
#include <cws/map.hpp>
#include <cws/simulation.hpp>
#include <cwspb/service/simulation.grpc.pb.h>
#include <cwspb/service/simulation.pb.h>

void simulation() {
  Simulation simulation(Dimension{.width = 5, .height = 10}, SimulationType::LIMITED,
                        20, 10);

  cws::Dimension dimension;

  simulation.run();
}

grpc::Status performRequest(std::unique_ptr<cws::SimulationService::Stub> & stub,
                            const cws::Request & request,
                            cws::ResponseSimulationState & response) {
  grpc::ClientContext context;
  auto status = stub->GetSimulationState(&context, request, &response);
  return status;
}

void setRequest(cws::Request & request) {}

void processStatus(const grpc::Status & status) {
  if (!status.ok()) {
    std::cout << "ERROR(code=" << status.error_code() << "): " << status.error_message()
              << std::endl;
  }
}

void getResponse(const grpc::Status & status,
                 const cws::ResponseSimulationState & response) {
  processStatus(status);
}

void runClient(const std::string & address, const int port) {

  std::string uri = address + ":" + std::to_string(port);
  auto channel = grpc::CreateChannel(uri, grpc::InsecureChannelCredentials());

  std::unique_ptr<cws::SimulationService::Stub> stub =
      cws::SimulationService::NewStub(channel);

  cws::Request request;
  cws::ResponseSimulationState response;
  setRequest(request);
  auto status = performRequest(stub, request, response);
  getResponse(status, response);
}

int main(int argc, char * argv[]) {
  // simulation();
  runClient("127.0.0.1", 8200);
  return 0;
}
