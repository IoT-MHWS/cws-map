#include "verify.hpp"

bool verifyMapCreated(const std::shared_ptr<const Map> & map,
                      cwspb::Response & response) {
  bool bad = !map;
  auto status = response.mutable_status();

  if (bad) {
    status->set_text("map is not created");
    status->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
  }
  return !bad;
}

bool verifyCoordinates(const Coordinates & coord, const Dimension & dimension,
                       cwspb::Response & response) {
  bool bad = (coord.x >= dimension.width || coord.x < 0 ||
              coord.y >= dimension.height || coord.y < 0);
  auto status = response.mutable_status();

  if (bad) {
    status->set_text("coordinates out of bounds");
    status->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
  }
  return !bad;
}

bool verifySubjectExists(const void * subject, cwspb::Response & response) {
  if (!subject) {
    auto resStatus = response.mutable_status();
    resStatus->set_text("subject does not exist");
    resStatus->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
  }
  return subject;
}
