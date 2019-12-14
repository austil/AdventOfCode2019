#include <iostream>
#include "IntcodeComputer.cpp"

int main(int argc, char const *argv[])
{
  testComputer();

  const queue<IntCode> p1Input({1});
  const auto p1Program = getPuzzleInput("./inputs/aoc_day9_1.txt").front();
  const auto p1 = runProgram(p1Program, p1Input);
  assert(p1.terminated);
  cout << "Part1, BOOST Keycode: " << p1.outputs << "\n";

  const queue<IntCode> p2Input({2});
  const auto p2 = runProgram(p1Program, p2Input);
  assert(p2.terminated);
  cout << "Part2, distress signal coordinates: " << p2.outputs << "\n";

  return 0;
}
