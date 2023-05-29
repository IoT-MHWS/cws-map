#include "cws/layer/subject.hpp"
#include "cws/subject/extension/turnable.hpp"
#include "cws/subject/light_emitter.hpp"
#include <cassert>

using namespace Subject;

const std::list<const LightSourceAlt *> LayerSubject::getActiveLightSources() const {
  std::list<const LightSourceAlt *> sources;
  for (const auto & sub : subjectList) {
    if (sub->isTempSource()) {
      if (sub->isTurnable() &&
          dynamic_cast<Turnable *>(sub.get())->getStatus() != TurnableStatus::ON) {
        continue;
      }
      // requires rtti
      auto src = dynamic_cast<LightSourceAlt *>(sub.get());
      assert(src != nullptr);
      sources.push_back(src);
    }
  }
  return sources;
}
