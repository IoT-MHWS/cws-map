#include "cws/subject/light_emitter.hpp"

namespace Subject {

LightSourceParams TurnableLightEmitter::getCurLightParams() const {
  if (getStatus() == TurnableStatus::ON) {
    return getDefLightParams();
  } else {
    return offLightParams_;
  }
}

}// namespace Subject
