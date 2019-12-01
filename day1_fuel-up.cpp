#include <iostream>
#include <cmath>
#include <cassert>
#include "utils.cpp"

int fuelForModule(const int moduleMass) {
  return std::floor(moduleMass / 3) - 2;
}

int fuelForModuleFixed(const int moduleMass) {
  const int initialFuel = std::floor(moduleMass / 3) - 2;
  return initialFuel > 0 ? initialFuel + fuelForModuleFixed(initialFuel) : 0;
}

int main(int argc, char const *argv[])
{
  // Part 1
  assert(fuelForModule(12) == 2);
  assert(fuelForModule(14) == 2);
  assert(fuelForModule(1969) == 654);
  assert(fuelForModule(100756) == 33583);

  const auto firstInput = getPuzzleInput("./inputs/aoc_day1_1.txt");
  int spaceshipFuel = 0;
  for (string moduleMass : firstInput)
  {
    spaceshipFuel += fuelForModule(stoi(moduleMass));
  }
  cout << "spaceshipFuel: " << spaceshipFuel << "\n";

  // Part 2
  assert(fuelForModuleFixed(14) == 2);
  assert(fuelForModuleFixed(1969) == 966);
  assert(fuelForModuleFixed(100756) == 50346);

  int spaceshipFuelFixed = 0;
  for (string moduleMass : firstInput)
  {
    spaceshipFuelFixed += fuelForModuleFixed(stoi(moduleMass));
  }
  cout << "spaceshipFuelFixed: " << spaceshipFuelFixed << "\n";

  return 0;
}
