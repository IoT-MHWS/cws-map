#include <cws/map.hpp>

void LayersState::buildNextState(const LayersState & state,
                                 const Structures & structures) {

}

void States::buildNextState(const Structures & structures,
                            const Dimension & dimension) {
  LayersState nextState(dimension);
  nextState.buildNextState(currentState, structures);
}

void Map::buildNextState() { states.buildNextState(structures, dimension); }
