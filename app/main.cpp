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

  SimulationStateIn state;
  state.simType.set(SimulationType::LIMITED);
  state.simStatus.set(SimulationStatus::RUNNING);
  state.currentTick.set(0);
  state.lastTick.set(10);
  state.taskFrequency.set(5);

  interface.setState(state);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  state.reset();

  state.lastTick.set(100);
  state.taskFrequency.set(10);

  interface.setState(state);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  state.reset();

  state.simType.set(SimulationType::INFINITE);
  state.taskFrequency.set(30);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  state.reset();
  state.simStatus.set(SimulationStatus::STOPPED);

  interface.setState(state);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  interface.exit();
}

int main(int argc, char * argv[]) {
  simulation();
  return 0;
}
