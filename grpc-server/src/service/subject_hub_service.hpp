#pragma once

#include "converters.hpp"
#include "cws/simulation/interface.hpp"
#include "cwspb/service/subject_hub.grpc.pb.h"
#include "cwspb/service/subject_hub.pb.h"
#include <grpcpp/support/status.h>

class SubjectHubService final : public cws::SubjectHubService::Service {
private:
  SimulationInterface & interface;

public:
  SubjectHubService(SimulationInterface & interface) : interface(interface) {}

  ::grpc::Status GetTemperature(::grpc::ServerContext * context,
                                const ::cws::RequestSensorTemperature * request,
                                ::cws::ResponseSensorTemperature * response) override {

    auto map = interface.getMap();

    if (!verifyMapSet(map, *response->mutable_base())) {
      return grpc::Status::OK;
    }

    auto coordinates = fromCoordinates(request->coordinates());
    auto subjectId = fromSubjectId(request->id(), toSubjectType(SubjectType::SENSOR));
    SubjectQuerySelect query(coordinates, subjectId);

    auto subject = map->getQuery(std::move(query));

    if (!verifySubjectExists(subject, *response->mutable_base())) {
      return grpc::Status::OK;
    }

    if (subject->getSubjectId().type != SubjectType::SENSOR) {
      verifySubjectExists(nullptr, *response->mutable_base());
      return grpc::Status::OK;
    }

    auto subjectSensor = static_cast<const SubjectSensor *>(subject);

    if (subjectSensor->getSensorType() != SensorType::TEMPERATURE) {
      verifySubjectExists(nullptr, *response->mutable_base());
      return grpc::Status::OK;
    }

    auto sensorTemperature = static_cast<const SensorTemperature *>(subjectSensor);
    toTemperature(*response->mutable_temp(), sensorTemperature->getTemperature());

    return grpc::Status::OK;
  }

private:
  bool verifyMapSet(const std::shared_ptr<const Map> & map, cws::Response & response) {
    bool bad = !map;

    if (bad) {
      auto resStatus = response.mutable_status();
      resStatus->set_text("map is not created");
      resStatus->set_type(cws::ErrorType::ERROR_TYPE_BAD_REQUEST);
    }
    return !bad;
  }

private:
  bool verifySubjectExists(const void * subject, cws::Response & response) {
    if (!subject) {
      auto resStatus = response.mutable_status();
      resStatus->set_text("subject does not exist");
      resStatus->set_type(cws::ErrorType::ERROR_TYPE_BAD_REQUEST);
    }
    return subject;
  }
};
