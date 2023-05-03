#include "cws/simulation/interface.hpp"

/* Simulation */
void SimulationInterface::run() { master->run(); }

void SimulationInterface::exit() { master->exit(); }
