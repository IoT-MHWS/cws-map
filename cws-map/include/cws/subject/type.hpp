#pragma once

namespace Subject {

enum class Type {
  UNSPECIFIED = 0,
  PLAIN = 1,
  LIGHT_EMITTER = 2,
  TURNABLE_LIGHT_EMITTER = 3,
  TEMP_EMITTER = 4,
  TURNABLE_TEMP_EMITTER = 5,
  NETWORK_DEVICE = 6,
  INFRARED_CAMERA = 7,
  LIGHT_CAMERA = 8,
};

}
