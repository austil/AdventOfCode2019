#include <iostream>
#include <queue>
#include <algorithm>
#include "IntcodeComputer.cpp"

int maxThrusterSignal(const string program) {
  int biggestSignal = 0;
  vector<int> phaseSettingSequence {0, 1, 2, 3, 4};
  do {
    queue<int> programInput;
    int signal = 0;
    for (int phaseSetting : phaseSettingSequence)
    {
      programInput = {};
      programInput.push(phaseSetting);
      programInput.push(signal);
      const auto amplifierOutput = runProgram(program, programInput);
      signal = amplifierOutput.outputs.back();
    }
    if (signal > biggestSignal) {
      biggestSignal = signal;
    }
  } while (next_permutation(phaseSettingSequence.begin(), phaseSettingSequence.end()));
  return biggestSignal;
}

int main(int argc, char const *argv[])
{
  testComputer();

  const auto ampControl1 = "3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0";
  assert(maxThrusterSignal(ampControl1) == 43210);
  const auto ampControl2 = "3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0";
  assert(maxThrusterSignal(ampControl2) == 54321);
  const auto ampControl3 = "3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0";
  assert(maxThrusterSignal(ampControl3) == 65210);

  const auto p1 = maxThrusterSignal(getPuzzleInput("./inputs/aoc_day7_1.txt").front());
  cout << "Part1, max thruster output: " << p1 << "\n";

  return 0;
}
