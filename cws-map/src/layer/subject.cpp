#include "cws/layer/subject.hpp"
#include "cws/subject/extension/turnable.hpp"

using namespace Subject;

const std::list<const LightSourceAlt *> LayerSubject::getActiveLightSources() const {
  std::list<const LightSourceAlt *> sources;
  for (const auto & sub : subjectList) {
    if (sub->isTempSource()) {
      if (sub->isTurnable() &&
          ((Turnable *)sub.get())->getStatus() != TurnableStatus::ON) {
        continue;
      }
      sources.push_back((LightSourceAlt *)sub.get());
    }
  }
  return sources;
}
