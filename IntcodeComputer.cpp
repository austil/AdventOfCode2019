#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <queue>
#include "utils.cpp"

using number = long long;
struct ProgramState
{
  vector<number> memory {};
  queue<number> inputs {};
  vector<number> outputs {};
  size_t instructionPointer = 0;
  int relativeBase = 0;
  bool terminated = false;
};

ProgramState runProgram(const ProgramState initialState) {
  ProgramState state {initialState};

  const auto fillMem = [&](size_t toAddress) { 
    if(state.memory.size() <= toAddress) {
      cout << "out of bound memory access: " << toAddress << "\n";
      for(size_t i = state.memory.size(); i <= toAddress; i++) {
        state.memory.push_back(0);
      }
    }
  };

  const auto memWrite = [&](size_t address, number value) {
    fillMem(address);
    state.memory.at(address) = value;
  };

  bool run = true;
  while(run) {
    const string instruction = to_string(state.memory.at(state.instructionPointer));
    const int code = instruction.size() > 2 
      ? stoi(instruction.substr(instruction.size() - 2))
      : stoi(instruction);
    
    const string parametersMode = instruction.size() > 2
      ? instruction.substr(0, instruction.size() - 2)
      : "";

    const auto getParamValue = [&](int paramNumber){
      const int parametersModeIndex = parametersMode.size() - paramNumber;
      const bool isImmediateMode = parametersModeIndex >= 0 ? parametersMode.at(parametersModeIndex) == '1' : false;
      const bool isRelativeMode = parametersModeIndex >= 0 ? parametersMode.at(parametersModeIndex) == '2' : false;
      
      size_t address = state.memory.at(state.instructionPointer + paramNumber);
      if (isImmediateMode) {
        address = state.instructionPointer + paramNumber;
      }
      else if (isRelativeMode) {
        address = state.relativeBase + state.memory.at(state.instructionPointer + paramNumber); 
      }

      if (address < 0) {
        cerr << "Illegal program memory access: negative address\n";
        throw;
      }
      
      fillMem(address);
      return state.memory.at(address);
    };

    if(code == 1 || code == 2) { // * & +
      const number op1 = getParamValue(1);
      const number op2 = getParamValue(2);
      const number dest = state.memory.at(state.instructionPointer + 3);
      memWrite(dest, code == 1 ? op1 + op2 : op1 * op2);
      state.instructionPointer += 4;
    }
    else if (code == 3) { // input
      if(state.inputs.empty()) {
        return state;
      }
      const int input = state.inputs.front();
      state.inputs.pop();
      const int dest = state.memory.at(state.instructionPointer + 1);
      memWrite(dest, input);
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
      const int dest = state.memory.at(state.instructionPointer + 3);
      memWrite(dest, (code == 7 && op1 < op2) ? 1 : ((code == 8 && op1 == op2) ? 1 : 0));
      state.instructionPointer += 4;
    }
    else if (code == 9) { // adjusts relative base
      const int value = getParamValue(1);
      state.relativeBase += value;
      state.instructionPointer += 2;
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
ProgramState runProgram(const vector<number> program, queue<number> inputs = {}) {
  ProgramState initialState;
  initialState.memory = program;
  initialState.inputs = inputs;
  return runProgram(initialState);
};

vector<number> parseIntcode(const string str) {
  const vector<string> opCodes = split(str, ",");
  vector<number> program;
  for(auto oc : opCodes) {
    program.push_back(stoll(oc));
  }
  return program;
}

// Convenient signature for testing
ProgramState runProgram(string program, queue<number> inputs = {}) {
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
  queue<number> testInput({42, 551});
  assert(runProgram("3,0,4,0,99", testInput).outputs.front() == 42);
  assert(runProgram("3,0,4,0,3,1,4,1,99", testInput).outputs == parseIntcode("42, 551"));

  assert(runProgram("1002,4,3,4,33").memory == parseIntcode("1002,4,3,4,99"));

  // Day 5 part 2 tests, opcode 5, 6, 7, 8
  queue<number> inputIsZero({0});
  queue<number> inputIsLessThanEight({3});
  queue<number> inputIsEight({8});
  queue<number> inputIsMoreThanEight({4847});

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

  queue<number> emptyInput {};
  auto haltedState = runProgram(compareToEight, emptyInput);
  assert(haltedState.terminated == false);
  haltedState.inputs.push(8);
  assert(runProgram(haltedState).terminated == true);
  assert(runProgram(haltedState).outputs.front() == 1000);

  // Day 9 test, relative mode, opcode 9
  const auto replicatingProgram = parseIntcode("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99");
  assert(runProgram(replicatingProgram).outputs == replicatingProgram);
  assert(to_string(runProgram("1102,34915192,34915192,7,4,7,99,0").outputs.back()).size() == 16);
  assert(runProgram("104,1125899906842624,99").outputs.back() == 1125899906842624);

  cout << "Intcode Computer test successful\n\n";
}
