#include <iostream>
#include <queue>
#include "IntcodeComputer.cpp"

int main(int argc, char const *argv[])
{
  testComputer();

  // Part 1
  queue<int> p1Inputs({1});
  const auto p1 = runProgram(parseIntcode(getPuzzleInput("./inputs/aoc_day5_1.txt").front()), p1Inputs);
  cout << "Part1, program outputs: " << p1.outputs << "\n";

  // Part 2
  queue<int> p2Inputs({5});
  const auto p2 = runProgram(parseIntcode(getPuzzleInput("./inputs/aoc_day5_1.txt").front()), p2Inputs);
  cout << "Part2, program outputs: " << p2.outputs << "\n";

  return 0;
}
