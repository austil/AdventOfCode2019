#include <iostream>
#include <cassert>
#include "IntcodeComputer.cpp"

int main(int argc, char const *argv[])
{
  // Part 1
  const auto droneProgram = parseIntcode(getPuzzleInput("inputs/aoc_day19_1.txt").front());

  const auto inBeam = [&](IntCode x, IntCode y) {
    const queue<IntCode> droneInput({x, y});
    const auto drone = runProgram(droneProgram, droneInput);
    const bool isPulled = drone.outputs.back() == 1;
    assert(drone.terminated);
    return isPulled;
  };

  size_t tractorBeamAffeted = 0;
  for (int x = 0; x < 50; x++)
  {
    for (int y = 0; y < 50; y++)
    {
      if(inBeam(x, y)) {
        tractorBeamAffeted++;
      }
    }
  }
  cout << "Part1, coordinate affected by the tractor beam: " << tractorBeamAffeted << "\n";

  // Part 2
  bool search = true;
  int maxY = 0;
  int previousMinX = 0;
  while(search) {

    int x = previousMinX;
    optional<int> minX;
    while(!minX.has_value()) {
      if(!minX.has_value() && inBeam(x, maxY)) {
        minX = x;
      }
      else {
        x++;
      }
    }

    const bool bottomRightInBeam = inBeam(minX.value() + 99, maxY);
    if(bottomRightInBeam) {
      const bool topLeftInBeam = inBeam(minX.value(), maxY - 99);
      const bool topRightInBeam = inBeam(minX.value() + 99, maxY - 99);
      if(topLeftInBeam && topRightInBeam) {
        cout << "Part2: " << minX.value() * 10000 + (maxY - 99) << "\n";
        search = false;
      }
    }

    previousMinX = minX.value();
    maxY++;
  }
  
  return 0;
}

