#include "cws/layer/subject.hpp"
#include "cws/subject/extension/turnable.hpp"
#include "cws/subject/light_emitter.hpp"
#include <cassert>

using namespace Subject;

const std::list<const ExtLightSource *> LayerSubject::getActiveLightSources() const {
  std::list<const ExtLightSource *> sources;
  for (const auto & sub : subjectList) {
    if (auto lightSub = dynamic_cast<ExtLightSource *>(sub.get())) {
      if (auto turnable = dynamic_cast<ExtTurnable *>(lightSub)) {
        if (turnable->getStatus() != TurnableStatus::ON) {
          continue;
        }
      }
      sources.push_back(lightSub);
    }
  }
  return sources;
}
