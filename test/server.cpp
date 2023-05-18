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
