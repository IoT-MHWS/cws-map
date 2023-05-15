#include "cws/subject/temp_emitter.hpp"

namespace Subject {

void TempEmitter::nextTemperature() {
  updateTemperature(getCurTempParams().heatProduction);
}

TempSourceParams TurnableTempEmitter::getCurTempParams() const {
  if (getStatus() == TurnableStatus::ON) {
    return getDefTempParams();
  } else {
    return offTempParams_;
  }
}

}// namespace Subject
