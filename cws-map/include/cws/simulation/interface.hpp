#pragma once

#include <cstddef>

#include "cws/simulation/simulation.hpp"

class SimulationInterface {
private:
  SimulationMaster * master;

public:
  SimulationInterface(){};

  void setSimulationMaster(SimulationMaster * master) { this->master = master; }

public:
  void run();
  void exit();
};
