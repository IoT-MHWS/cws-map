#include <cws/general.hpp>
#include <cws/map.hpp>
#include <cws/simulation.hpp>
#include <thread>

void simulation() {
  Simulation simulation;

  simulation.run();

  simulation.exit();
}


int main(int argc, char * argv[]) {
  simulation();
  return 0;
}
