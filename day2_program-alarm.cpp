#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>
#include "utils.cpp"

vector<int> parse(const string str) {
  const vector<string> opCodes = split(str, ",");
  vector<int> program;
  for(auto oc : opCodes) {
    program.push_back(stoi(oc));
  }
  return program;
}

vector<int> runProgram(const vector<int> program) {
  vector<int> state {program};
  size_t position = 0;
  bool run = true;
  while(run) {
    const int code = state[position];
    if(code == 1 || code == 2) {
      const int op1 = state[state[position + 1]];
      const int op2 = state[state[position + 2]];
      const int dest = state[position + 3];
      state[dest] = code == 1 ? op1 + op2 : op1 * op2;
      position += 4;
    }
    else if (code == 99) {
      run = false;
    }
    else {
      cerr << "opCode not supported: " << code << " at position: " << position << "\n";
      cerr << program << "\n";
      cerr << state << "\n";
      throw;
    }
  }
  // cerr << program << "\n";
  // cerr << state << "\n";  
  return state;
}

int main(int argc, char const *argv[])
{
  // Part 1
  assert(runProgram(parse("1,0,0,0,99")) == parse("2,0,0,0,99"));
  assert(runProgram(parse("2,3,0,3,99")) == parse("2,3,0,6,99"));
  assert(runProgram(parse("2,4,4,5,99,0")) == parse("2,4,4,5,99,9801"));
  assert(runProgram(parse("1,1,1,4,99,5,6,0,99")) == parse("30,1,1,4,2,5,6,0,99"));
  assert(runProgram(parse("1,9,10,3,2,3,11,0,99,30,40,50")) == parse("3500,9,10,70, 2,3,11,0, 99, 30,40,50"));
  
  vector<string> input = getPuzzleInput("./inputs/aoc_day2_1.txt");
  const auto part1 = runProgram(parse(input.front()));
  cout << "part1, pos0: " << part1[0] << "\n";
  
  // Part 2
  auto tweakableInput = parse(input.front());
  for (size_t noun = 0; noun < 99; noun++)
  {
    for (size_t verb = 0; verb < 99; verb++)
    {
      tweakableInput[1] = noun;
      tweakableInput[2] = verb;
      const auto part2 = runProgram(tweakableInput);
      if(part2[0] == 19690720) {
        cout << "part2, answer for output 19690720: noun = " << noun << " & verb = " << verb << "\n";
        cout << "100 * noun + verb = " << 100 * noun + verb << "\n";
        return 0;
      }
    }
  }

  return 0;
}
