#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>
#include "IntcodeComputer.cpp"

int main(int argc, char const *argv[])
{
  testComputer();
  
  // Part 1
  vector<string> input = getPuzzleInput("./inputs/aoc_day2_1.txt");
  const auto part1 = runProgram(input.front()).memory;
  cout << "part1, pos0: " << part1[0] << "\n";
  
  // Part 2
  auto tweakableInput = parseIntcode(input.front());
  for (size_t noun = 0; noun < 99; noun++)
  {
    for (size_t verb = 0; verb < 99; verb++)
    {
      tweakableInput[1] = noun;
      tweakableInput[2] = verb;
      const auto part2 = runProgram(tweakableInput).memory;
      if(part2[0] == 19690720) {
        cout << "part2, answer for output 19690720: noun = " << noun << " & verb = " << verb << "\n";
        cout << "100 * noun + verb = " << 100 * noun + verb << "\n";
        return 0;
      }
    }
  }

  return 0;
}
