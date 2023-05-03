#pragma once

#include "cws/map.hpp"

struct SubjectQuery {
  // some fields to update map
public:
};

/*
 * Map with extended feature to update map from queries
 */
class MapQuery : public Map {

public:
  explicit MapQuery(Dimension dimension) : Map(dimension) {}

  void updateMap(SubjectQuery &&query) {
    std::cout << "map updated" << std::endl;
  }
};
