#pragma once

#include "cws/subject/extension/light_source.hpp"
#include "cws/subject/extension/temp_source.hpp"
#include "cws/subject/plain.hpp"

namespace Subject {

using namespace Subject;

class LightEmitter : public Plain, public LightSource, public TempSource {

public:
  LightEmitter(Id id, PlainParams plain, LightSourceParams light, TempSourceParams temp)
      : Plain(id, plain), LightSource(light), TempSource(temp) {}
};

}// namespace Subject
