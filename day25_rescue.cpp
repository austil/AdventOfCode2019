#include <iostream>
#include <cassert>
#include "IntcodeComputer.cpp"

void pushInput(ProgramState &droid, const string instruction) {
  for(char c : instruction) {
    droid.inputs.push(c);
  }
  droid.inputs.push('\n');
}

string asciiToString(const vector<IntCode> &ascii) {
  string str = "";
  for(char c : ascii) {
    str += c;
  }
  return str;
}

/*
Valid moves:
- north, south, east, or west
- take <name of item>
- drop <name of item>
- inv
*/

const vector<string> replayInputs {
  "west",
  "take mug",
  "north",
  // "take easter egg",
  "south",
  "east", // back to start
  "east",
  "take coin",
  "north",
  "north",
  "take hypercube",
  "south",
  "east",
  // "take manifold",
  "west",
  "south",
  "south",
  "east",
  // "take pointer",
  "west",
  "west",
  "take astrolabe",
  "north",
  "east",
  "north",
  "east"
};

int main(int argc, char const *argv[]) {
  const auto droidProgram = parseIntcode(getPuzzleInput("inputs/aoc_day25_1.txt").front());
  auto droid = runProgram(droidProgram);

  for(string in : replayInputs) {
    pushInput(droid, in);
  }
  droid = runProgram(droid);

  string instruction;
  while(!droid.terminated) {
    cout << asciiToString(droid.outputs) << "\n";
    getline(cin, instruction);
    pushInput(droid, instruction);
    droid.outputs.clear();
    droid = runProgram(droid);
  }
  cout << asciiToString(droid.outputs) << "\n";
  
  return 0;
}

