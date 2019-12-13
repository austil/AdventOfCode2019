#include <iostream>
#include "IntcodeComputer.cpp"

int main(int argc, char const *argv[])
{
  testComputer();

  const auto p1 = runProgram(getPuzzleInput("./inputs/aoc_day9_1.txt").front());
  cout << "Part1, BOOST Keycode: " << p1.outputs.back() << "\n";

  return 0;
}
