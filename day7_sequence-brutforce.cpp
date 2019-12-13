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

int maxLoopedThrusterSignal(const string programStr) {
  const auto program = parseIntcode(programStr);
  vector<int> phaseSettingSequence = {5, 6, 7, 8, 9};
  int biggestSignal = 0;
  do {
    ProgramState programStates[5];
    int terminatedCount = 0;
    bool phaseInitDone = false;
    while(terminatedCount < 5) {
      for (int i = 0; i < 5; i++)
      {
        if(!phaseInitDone) {
          programStates[i].memory = program;
          programStates[i].inputs.push(phaseSettingSequence[i]);
        }

        const int inputSignal = (i == 0) 
          ? (!phaseInitDone ? 0 : programStates[4].outputs.back()) 
          : programStates[i-1].outputs.back();
        programStates[i].inputs.push(inputSignal);

        const auto amplifierOutput = runProgram(programStates[i]);
        if(amplifierOutput.terminated) {
          terminatedCount++;
        }
        programStates[i] = amplifierOutput;
      }
      phaseInitDone = true;
    }
    const auto signal = programStates[4].outputs.back();
    if (signal > biggestSignal) {
      biggestSignal = signal;
    }
  } while (next_permutation(phaseSettingSequence.begin(), phaseSettingSequence.end()));
  return biggestSignal;
}

int main(int argc, char const *argv[])
{
  testComputer();

  // Part 1
  const auto ampControl1 = "3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0";
  assert(maxThrusterSignal(ampControl1) == 43210);
  const auto ampControl2 = "3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0";
  assert(maxThrusterSignal(ampControl2) == 54321);
  const auto ampControl3 = "3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0";
  assert(maxThrusterSignal(ampControl3) == 65210);

  const auto p1 = maxThrusterSignal(getPuzzleInput("./inputs/aoc_day7_1.txt").front());
  cout << "Part1, max thruster output: " << p1 << "\n";

  // Part 2
  const auto ampLoopedControl1 = "3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5";
  assert(maxLoopedThrusterSignal(ampLoopedControl1) == 139629729);
  const auto ampLoopedControl2 = "3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10";
  assert(maxLoopedThrusterSignal(ampLoopedControl2) == 18216);

  const auto p2 = maxLoopedThrusterSignal(getPuzzleInput("./inputs/aoc_day7_1.txt").front());
  cout << "Part1, max thruster output: " << p2 << "\n";

  return 0;
}
