#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <queue>
#include "utils.cpp"

struct ProgramState
{
  vector<int> memory {};
  queue<int> inputs {};
  vector<int> outputs {};
  size_t instructionPointer = 0;
  bool terminated = false;
};

ProgramState runProgram(const ProgramState initialState) {
  ProgramState state {initialState};
  bool run = true;
  while(run) {
    const string instruction = to_string(state.memory[state.instructionPointer]);
    const int code = instruction.size() > 2 
      ? stoi(instruction.substr(instruction.size() - 2))
      : stoi(instruction);
    
    const string parametersMode = instruction.size() > 2
      ? instruction.substr(0, instruction.size() - 2)
      : "";
    
    const auto isImmediateMode = [&](int paramNumber){
      const int parametersModeIndex = parametersMode.size() - paramNumber;
      return parametersModeIndex >= 0 ? parametersMode.at(parametersModeIndex) == '1' : false;
    };

    const auto getParamValue = [&](int paramNumber){
      return isImmediateMode(paramNumber) 
        ? state.memory[state.instructionPointer + paramNumber]
        : state.memory[state.memory[state.instructionPointer + paramNumber]];
    };

    if(code == 1 || code == 2) { // * & +
      const int op1 = getParamValue(1);
      const int op2 = getParamValue(2);
      const int dest = state.memory[state.instructionPointer + 3];
      state.memory[dest] = code == 1 ? op1 + op2 : op1 * op2;
      state.instructionPointer += 4;
    }
    else if (code == 3) { // input
      if(state.inputs.empty()) {
        return state;
      }
      const int input = state.inputs.front();
      state.inputs.pop();
      const int dest = state.memory[state.instructionPointer + 1];
      state.memory[dest] = input;
      state.instructionPointer += 2;
    }
    else if (code == 4) { // output
      const int value = getParamValue(1);
      state.outputs.push_back(value);
      state.instructionPointer += 2;
    }
    else if (code == 5 || code == 6) { // jump if true & jump if false
      const int op1 = getParamValue(1);
      if((code == 5 && op1 != 0) || (code == 6 && op1 == 0)) {
        const int op2 = getParamValue(2);
        state.instructionPointer = op2;
      } else {
        state.instructionPointer += 3;
      }
    }
    else if (code == 7 || code == 8) { // less than & equals
      const int op1 = getParamValue(1);
      const int op2 = getParamValue(2);
      const int dest = state.memory[state.instructionPointer + 3];
      state.memory[dest] = (code == 7 && op1 < op2) ? 1 : ((code == 8 && op1 == op2) ? 1 : 0);
      state.instructionPointer += 4;
    }
    else if (code == 99) {
      run = false;
    }
    else {
      cerr << "opCode not supported: " << code << " at position: " << state.instructionPointer << "\n";
      cerr << initialState.memory << "\n";
      cerr << state.memory << "\n";
      throw;
    }
  }
  state.terminated = true;
  return state;
}

// Signature backward compatibility
ProgramState runProgram(const vector<int> program, queue<int> inputs = {}) {
  ProgramState initialState;
  initialState.memory = program;
  initialState.inputs = inputs;
  return runProgram(initialState);
};

vector<int> parseIntcode(const string str) {
  const vector<string> opCodes = split(str, ",");
  vector<int> program;
  for(auto oc : opCodes) {
    program.push_back(stoi(oc));
  }
  return program;
}

// Convenient signature for testing
ProgramState runProgram(string program, queue<int> inputs = {}) {
  return runProgram(parseIntcode(program), inputs);
}

void testComputer() {
  cout << "Intcode Computer test begin\n";

  // Day 2 tests, opcode 1, 2 & 99
  assert(runProgram("1,0,0,0,99").memory == parseIntcode("2,0,0,0,99"));
  assert(runProgram("2,3,0,3,99").memory == parseIntcode("2,3,0,6,99"));
  assert(runProgram("2,4,4,5,99,0").memory == parseIntcode("2,4,4,5,99,9801"));
  assert(runProgram("1,1,1,4,99,5,6,0,99").memory == parseIntcode("30,1,1,4,2,5,6,0,99"));
  assert(runProgram("1,9,10,3,2,3,11,0,99,30,40,50").memory == parseIntcode("3500,9,10,70, 2,3,11,0, 99, 30,40,50"));

  // Day 5 part 1 tests, opcode 3, 4 & immediate mode
  queue<int> testInput({42, 551});
  assert(runProgram("3,0,4,0,99", testInput).outputs.front() == 42);
  assert(runProgram("3,0,4,0,3,1,4,1,99", testInput).outputs == parseIntcode("42, 551"));

  assert(runProgram("1002,4,3,4,33").memory == parseIntcode("1002,4,3,4,99"));

  // Day 5 part 2 tests, opcode 5, 6, 7, 8
  queue<int> inputIsZero({0});
  queue<int> inputIsLessThanEight({3});
  queue<int> inputIsEight({8});
  queue<int> inputIsMoreThanEight({4847});

  const auto equalsEight_p = "3,9,8,9,10,9,4,9,99,-1,8";
  const auto equalsEight_i = "3,3,1108,-1,8,3,4,3,99";
  assert(runProgram(equalsEight_p, inputIsEight).outputs.front() == 1);
  assert(runProgram(equalsEight_i, inputIsEight).outputs.front() == 1);
  assert(runProgram(equalsEight_p, inputIsLessThanEight).outputs.front() == 0);
  assert(runProgram(equalsEight_i, inputIsLessThanEight).outputs.front() == 0);

  const auto lessThanEight_p = "3,9,7,9,10,9,4,9,99,-1,8";
  const auto lessThanEight_i = "3,3,1107,-1,8,3,4,3,99";
  assert(runProgram(lessThanEight_p, inputIsLessThanEight).outputs.front() == 1);
  assert(runProgram(lessThanEight_i, inputIsLessThanEight).outputs.front() == 1);
  assert(runProgram(lessThanEight_p, inputIsEight).outputs.front() == 0);
  assert(runProgram(lessThanEight_i, inputIsEight).outputs.front() == 0);

  const auto isNotZero_p = "3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9";
  const auto isNotZero_i = "3,3,1105,-1,9,1101,0,0,12,4,12,99,1";
  assert(runProgram(isNotZero_p, inputIsEight).outputs.front() == 1);
  assert(runProgram(isNotZero_i, inputIsEight).outputs.front() == 1);
  assert(runProgram(isNotZero_p, inputIsZero).outputs.front() == 0);
  assert(runProgram(isNotZero_i, inputIsZero).outputs.front() == 0);

  const auto compareToEight = "3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99";
  assert(runProgram(compareToEight, inputIsMoreThanEight).outputs.front() == 1001);
  assert(runProgram(compareToEight, inputIsEight).outputs.front() == 1000);
  assert(runProgram(compareToEight, inputIsLessThanEight).outputs.front() == 999);

  // Day 7 part 2 tests, halt when waiting for input
  assert(runProgram(compareToEight, inputIsLessThanEight).terminated == true);

  queue<int> emptyInput {};
  auto haltedState = runProgram(compareToEight, emptyInput);
  assert(haltedState.terminated == false);
  haltedState.inputs.push(8);
  assert(runProgram(haltedState).terminated == true);
  assert(runProgram(haltedState).outputs.front() == 1000);

  cout << "Intcode Computer test successful\n\n";
}
