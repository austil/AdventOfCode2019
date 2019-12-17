#include <iostream>
#include <cassert>
#include "IntcodeComputer.cpp"
#include "coordinate.cpp"

enum class ASCII { newline = 10, hash = 35, dot = 46 };

int main(int argc, char const *argv[])
{
  // Part 1
  const auto p1Input = getPuzzleInput("inputs/aoc_day17_1.txt").front();
  const auto vacuumRobot = runProgram(p1Input);

  Coordinate currentCoord {0, 0};
  unordered_set<Coordinate, CoordinateHash> scafoldCoordinates;
  for(const auto pixel : vacuumRobot.outputs) {
    if(pixel == static_cast<IntCode>(ASCII::hash)) {
      scafoldCoordinates.insert(currentCoord);
    }

    if(pixel == static_cast<IntCode>(ASCII::newline)) {
      currentCoord.x = 0;
      currentCoord.y += 1;
    } else {
      currentCoord.x += 1;
    }
  }

  size_t alignementParameterSum = 0;
  for(const auto sc : scafoldCoordinates) {
    const bool continueUp = scafoldCoordinates.find({sc.x, sc.y+1}) != scafoldCoordinates.cend();
    const bool continueDown = scafoldCoordinates.find({sc.x, sc.y-1}) != scafoldCoordinates.cend();
    const bool continueLeft = scafoldCoordinates.find({sc.x+1, sc.y}) != scafoldCoordinates.cend();
    const bool continueRight = scafoldCoordinates.find({sc.x-1, sc.y}) != scafoldCoordinates.cend();
    const auto isIntersection = continueUp && continueDown && continueLeft && continueRight;
    if(isIntersection) {
      alignementParameterSum += sc.x * sc.y;
    }
  }

  cout << "Part1, alignementParameterSum: " << alignementParameterSum << "\n";

  return 0;
}

