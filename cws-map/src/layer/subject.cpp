#include "cws/layer/subject.hpp"
#include "cws/subject/extension/network.hpp"
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

const std::list<const Subject::ExtTransmitter *>
LayerSubject::getNetworkTransmitters() const {
  std::list<const ExtTransmitter *> transmitters;
  for (auto & sub : subjectList) {
    if (auto trans = dynamic_cast<ExtTransmitter *>(sub.get())) {
      transmitters.push_back(trans);
    }
  }
  return transmitters;
}

const std::list<const Subject::ExtReceiver *>
LayerSubject::getNetworkReceivers() const {
  std::list<const ExtReceiver *> receivers;
  for (auto & sub : subjectList) {
    if (auto receiv = dynamic_cast<ExtReceiver *>(sub.get())) {
      receivers.push_back(receiv);
    }
  }
  return receivers;
}
