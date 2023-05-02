#include <cws/simulation.hpp>
#include <cws/general.hpp>
#include <cws/concurrent_map.hpp>
#include <cws/map.hpp>

#include <cwspb/service/sim_info.pb.h>

int main() {
  Simulation simulation(Dimension{.width=5, .height=10}, SimulationType::LIMITED, 20, 10);
  simulation.run();
}
