#pragma once

#include "cws/general.hpp"
#include "cws/map.hpp"

class ConcurrentMap : private Map {
public:
  ConcurrentMap(Dimension dimension) : Map(dimension) {}
  
  Dimension getDimension() const { return Map::getDimension(); }

  void nextState() { return Map::nextState(); }
};
