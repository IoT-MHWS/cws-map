#pragma once

#include "cws/air/container.hpp"
#include "cws/layer/base.hpp"
#include <list>
#include <memory>

class LayerAir : public Layer {
  Air::Container container;

public:
  LayerAir() {}

  const Air::Container & getAirContainer() const { return container; }

  Air::Container & accessAirContainer() { return container; }
};
