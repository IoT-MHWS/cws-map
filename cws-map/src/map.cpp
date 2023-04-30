#include <cws/map.hpp>

void LayersState::buildNextState(const LayersState & state,
                                 const Structures & structures) {

}

void States::nextState(const Structures & structures,
                            const Dimension & dimension) {
  LayersState stateNext(dimension);
  stateNext.buildNextState(stateCurrent, structures);
}

void Map::nextState() { states.nextState(structures, dimension); }
