#include <iostream>
#include <cassert>
#include "IntcodeComputer.cpp"

int main(int argc, char const *argv[])
{
  // Part 1
  const auto droneProgram = parseIntcode(getPuzzleInput("inputs/aoc_day19_1.txt").front());

  size_t tractorBeamAffeted = 0;
  for (int x = 0; x < 50; x++)
  {
    for (int y = 0; y < 50; y++)
    {
      const queue<IntCode> droneInput({x, y});
      const auto drone = runProgram(droneProgram, droneInput);
      const bool isPulled = drone.outputs.back() == 1;
      if(drone.terminated && isPulled) {
        tractorBeamAffeted++;
      }
    }
  }
  cout << "Part1, coordinate affected by the tractor beam: " << tractorBeamAffeted << "\n";
  
  return 0;
}

