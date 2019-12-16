#include <iostream>
#include <cassert>
#include "utils.cpp"

const vector<int> basePattern {0, 1, 0, -1};

int getPatternValue(size_t repeatTime, size_t i) {
  return basePattern.at((i / repeatTime) % basePattern.size());
}

vector<int> getRepeatingPattern(size_t repeatTime, size_t length) {
  vector<int> repeatingPattern;
  for(size_t i = 1; i <= length; i++) {
    repeatingPattern.push_back(getPatternValue(repeatTime, i));
  }
  return repeatingPattern;
}

vector<int> nextPhase(vector<int> signal) {
  vector<int> nextSignal;
  for (size_t i = 1; i <= signal.size(); i++) {
    int nextDigit = 0;
    for (size_t d = 0; d < signal.size(); d++){
      nextDigit += getPatternValue(i, d + 1) * signal.at(d);
    }
    nextSignal.push_back(abs(nextDigit % 10));
  }
  return nextSignal;
};

vector<int> parse(string signal) {
  vector<int> digits;
  for(const auto c : signal) {
    digits.push_back(c - '0');
  }
  return digits;
}

vector<int> FFT(vector<int> signal, int numberOfPhases) {
  if(numberOfPhases <= 0) {
    return signal;
  }
  return FFT(nextPhase(signal), numberOfPhases - 1);
};

int main(int argc, char const *argv[])
{
  // Part 1
  const auto iv = id<vector<int>>;
  assert(getRepeatingPattern(1, 3) == iv({1, 0, -1}));
  assert(getRepeatingPattern(1, 6) == iv({1, 0, -1, 0, 1, 0}));
  assert(getRepeatingPattern(2, 8) == iv({0, 1, 1, 0, 0, -1, -1, 0}));
  assert(getRepeatingPattern(3, 11) == iv({0, 0, 1, 1, 1, 0, 0, 0, -1, -1, -1}));
  assert(getRepeatingPattern(3, 12) == iv({0, 0, 1, 1, 1, 0, 0, 0, -1, -1, -1, 0}));
  assert(getRepeatingPattern(5, 48).size() == 48);

  assert(nextPhase({1,2,3,4,5,6,7,8}) == iv({4,8,2,2,6,1,5,8}));
  assert(parse("12345678") == iv({1,2,3,4,5,6,7,8}));

  const auto t1 = FFT(parse("80871224585914546619083218645595"), 100);
  const vector<int> t1FirstEight(t1.cbegin(), t1.cbegin() + 8);
  assert(t1FirstEight == parse("24176176"));

  const auto t2 = FFT(parse("19617804207202209144916044189917"), 100);
  const vector<int> t2FirstEight(t2.cbegin(), t2.cbegin() + 8);
  assert(t2FirstEight == parse("73745418"));

  const auto t3 = FFT(parse("69317163492948606335995924319873"), 100);
  const vector<int> t3FirstEight(t3.cbegin(), t3.cbegin() + 8);
  assert(t3FirstEight == parse("52432133"));

  const auto part1Input = getPuzzleInput("inputs/aoc_day16_1.txt").front();
  cout << "Part1 input length: " << part1Input.size() << "\n";
  const auto part1FFT = FFT(parse(part1Input), 100);
  const vector<int> part1FirstEight(part1FFT.cbegin(), part1FFT.cbegin() + 8);
  cout << "Part1, first 8 FFT digit: " << part1FirstEight << "\n";

  // Part 2
  string repeatedInput = "";
  for (size_t i = 0; i < 10000; i++){
    repeatedInput += part1Input;
  }
  cout << "Part2 input length: " << repeatedInput.size() << "\n";
  // part 2 is not the same as part 1

  return 0;
}
