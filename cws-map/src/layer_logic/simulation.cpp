#include <chrono>
#include <iostream>
#include <thread>

#include <cws/simulation.hpp>

void Simulation::run() {
  auto duration = static_cast<int>(1'000'000'000 / taskFrequency);
  std::chrono::nanoseconds taskMaxDuration(duration);

  while (simulationType == SimulationType::INFINITE || repetitionsLeft-- != 0) {
    auto start = std::chrono::high_resolution_clock::now();

    concurrentMap.nextState();
    ++repetitions;

    auto leadTime = std::chrono::high_resolution_clock::now() - start;
    auto waitTime = std::max(taskMaxDuration - leadTime, std::chrono::nanoseconds(0));

    #ifndef NDEBUG
    std::cout << "NEXT:(" << repetitions << ")" <<  std::endl;
    std::cout << "wait time: " << waitTime << std::endl;
    #endif

    std::this_thread::sleep_for(waitTime);
  }
}
