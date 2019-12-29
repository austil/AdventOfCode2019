#include <iostream>
#include <cassert>
#include "IntcodeComputer.cpp"

void pushSpringscriptInput(ProgramState &droid, const vector<string> &instructions) {
  if(instructions.size() > 15) {
    cerr << "Springscript too large (more than 15 instructions)\n";
    throw;
  }
  for(string instruction : instructions) {
    for(char c : instruction) {
      droid.inputs.push(c);
    }
    droid.inputs.push('\n');
  }
}

string asciiToString(const vector<IntCode> &ascii) {
  string str = "";
  for(char c : ascii) {
    str += c;
  }
  return str;
}

const vector<string> walkSpringscript {
  // Hole incoming
  "NOT A T",
  "NOT B J",
  "OR T J",
  "NOT C T",
  "OR T J",
  // Landing possible
  "AND D J",
  "WALK"
};

const vector<string> runSpringscript {
  // Hole incoming
  "NOT A T",
  "NOT B J",
  "OR T J",
  "NOT C T",
  "OR T J",
  // Landing a first jump is possible
  "AND D J",
  // and landing an immediate second if necessary
  "NOT H T",
  "NOT T T",
  "OR E T",
  "AND T J",
  "RUN"
};

int main(int argc, char const *argv[])
{
  // Part 1
  const auto springdroidProgram = parseIntcode(getPuzzleInput("inputs/aoc_day21_1.txt").front());
  auto springdroid = runProgram(springdroidProgram);

  pushSpringscriptInput(springdroid, walkSpringscript);
  springdroid = runProgram(springdroid);

  // cout << asciiToString(springdroid.outputs) << "\n";
  cout << "Part1, hull damage: " << springdroid.outputs.back() << "\n";

  // Part 2
  auto springdroid2 = runProgram(springdroidProgram);

  pushSpringscriptInput(springdroid2, runSpringscript);
  springdroid2 = runProgram(springdroid2);

  // cout << asciiToString(springdroid2.outputs) << "\n";
  cout << "Part2, hull damage: " << springdroid2.outputs.back() << "\n";

  return 0;
}
