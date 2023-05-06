#include <cws/map.hpp>

void Layers::nextState() { }


void Map::nextState(const Map &cur) { layers.nextState(); }
