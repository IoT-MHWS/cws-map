#include "cws/simulation/interface.hpp"
#include "cws/simulation/simulation.hpp"
#include <cws/general.hpp>
#include <cws/map.hpp>
#include <thread>

void simulation() {
  SimulationInterface interface;

  SimulationMaster master(interface);
  interface.setSimulationMaster(&master);

  interface.run();

  interface.setSimulationState(SimulationState{
      .simulationType = SimulationType::LIMITED,
      .currentTick = 0,
      .lastTick = 10,
      .taskFrequency = 5,
  });

  std::this_thread::sleep_for(std::chrono::seconds(4));

  interface.setSimulationState(SimulationState{
      .simulationType = SimulationType::LIMITED,
      .currentTick = 50,
      .lastTick = 100,
      .taskFrequency = 10,
  });

  std::this_thread::sleep_for(std::chrono::seconds(4));

  interface.exit();
}

int main(int argc, char * argv[]) {
  simulation();
  return 0;
}
