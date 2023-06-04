#pragma once

#include "converters.hpp"
#include "cws/simulation/interface.hpp"
#include "cwspb/service/sv_device.grpc.pb.h"
#include <grpcpp/support/status.h>

class DeviceService final : public cwspb::DeviceService::Service {
private:
  SimulationInterface & interface;

public:
  DeviceService(SimulationInterface & interface) : interface(interface) {}

  // ::grpc::Status GetTemperature(::grpc::ServerContext * context,
  //                               const ::cwspb::RequestSensorTemperature * request,
  //                               ::cwspb::ResponseSensorTemperature * response)
  //                               override
  //                               {

  //   auto map = interface.getMap();

  //   if (!verifyMapSet(map, *response->mutable_base())) {
  //     return grpc::Status::OK;
  //   }

  //   auto coordinates = fromCoordinates(request->coordinates());

  //   auto idx = request->id().id();

  //   SubjectQuery query(SubjectQueryType::SELECT, coordinates,
  //                      std::make_unique<SensorTemperature>(idx));

  //   auto subject = map->getQuery(std::move(query));

  //   if (!verifySubjectExists(subject, *response->mutable_base())) {
  //     return grpc::Status::OK;
  //   }

  //   if (subject->getSubjectId().type != SubjectType::SENSOR) {
  //     verifySubjectExists(nullptr, *response->mutable_base());
  //     return grpc::Status::OK;
  //   }

  //   auto subjectSensor = static_cast<const SubjectSensor *>(subject);

  //   if (subjectSensor->getSensorType() != SensorType::TEMPERATURE) {
  //     verifySubjectExists(nullptr, *response->mutable_base());
  //     return grpc::Status::OK;
  //   }

  //   auto sensorTemperature = static_cast<const SensorTemperature *>(subjectSensor);
  //   toTemperature(*response->mutable_temp(), sensorTemperature->getTemperature());

  //   return grpc::Status::OK;
  // }

private:
  bool verifyMapSet(const std::shared_ptr<const Map> & map,
                    cwspb::Response & response) {
    bool bad = !map;

    if (bad) {
      auto resStatus = response.mutable_status();
      resStatus->set_text("map is not created");
      resStatus->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
    }
    return !bad;
  }

private:
  bool verifySubjectExists(const void * subject, cwspb::Response & response) {
    if (!subject) {
      auto resStatus = response.mutable_status();
      resStatus->set_text("subject does not exist");
      resStatus->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
    }
    return subject;
  }
};
