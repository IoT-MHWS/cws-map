#pragma once

#include "cws/map.hpp"

struct SubjectQuery {
  // some fields to update map
public:
};

/*
 * Map with extended feature to update map from queries
 */
class SimulationMap : public Map {

public:
  explicit SimulationMap(Dimension dimension) : Map(dimension) {}

  void update(SubjectQuery && query);
};
