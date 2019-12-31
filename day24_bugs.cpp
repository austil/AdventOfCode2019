#include <iostream>
#include <list>
#include <cassert>
#include "utils.cpp"

using grid = vector<vector<char>>;
using recursiveGrid = vector<grid>;

enum class Side { TOP, RIGHT, BOTTOM, LEFT };

const auto BUG = '#';
const auto EMPTY_SPACE = '.';

grid emptyGrid(size_t size = 5) {
  grid g;
  for (size_t y = 0; y < size; y++) {
    vector<char> row(size, EMPTY_SPACE); 
    g.push_back(row);
  }
  return g;
}

string layoutToString(const grid &layout) {
  string str = "";
  for(const auto row : layout){
    const string line(row.data());
    str += line;
  }
  return str;
}

int bugCount(const grid &layout, const optional<Side> sideOnly = nullopt) {
  int total = 0;
  for (size_t i = 0; i < layout.size(); i++){
    const auto row = layout.at(i);
    if(sideOnly.has_value()) {
      const auto side = sideOnly.value();
      if((side == Side::LEFT && row.front() == BUG) || (side == Side::RIGHT && row.back() == BUG)) {
        total++;
      }
      else if((side == Side::TOP && i == 0) || (side == Side::BOTTOM && i == layout.size() - 1)) {
        total += count(row.cbegin(), row.cend(), BUG);
      }
    }
    else {
      total += count(row.cbegin(), row.cend(), BUG);
    }
  }
  return total;
}

int bugCount(const recursiveGrid &recLayout) {
  int total = 0;
  for(const auto currentLayout : recLayout) {
    total += bugCount(currentLayout);
  }
  return total;
}

int biodiversityRating(const grid &layout) {
  int score = 0;
  for (size_t y = 0; y < layout.size(); y++) {
    for (size_t x = 0; x < layout.size(); x++) {
      const auto inPlace = layout.at(y).at(x);
      if(inPlace == BUG) {
        score += pow(2, y * layout.size() + x);
      }
    }
  }
  return score;
}

grid parseInput(const vector<string> &input) {
  grid grid;
  for(const auto line : input){
    const vector<char> data(line.begin(), line.end());
    grid.push_back(data);
    assert(data.size() == 5);
  }
  assert(grid.size() == 5);
  return grid;
}

grid getNextMinute(const grid &currentLayout,
  const grid &parentLayout = emptyGrid(),
  const optional<grid> &childLayout = nullopt
) {
  grid nextLayout = currentLayout;
  for (size_t y = 0; y < currentLayout.size(); y++) {
    const auto rowSize = currentLayout.at(y).size();
    assert(rowSize == currentLayout.size());
    for (size_t x = 0; x < rowSize; x++) {
      const char inPlace = currentLayout.at(y).at(x);

      const int bugAbove = 
        y == 3 && x == 2 && childLayout.has_value() ? bugCount(childLayout.value(), Side::BOTTOM) : 
        y == 0 ? parentLayout.at(1).at(2) == BUG :
        currentLayout.at(y-1).at(x) == BUG;
      const int bugBelow = 
        y == 1 && x == 2 && childLayout.has_value() ? bugCount(childLayout.value(), Side::TOP) : 
        y == rowSize-1 ? parentLayout.at(3).at(2) == BUG :
        currentLayout.at(y+1).at(x) == BUG;
      const int bugAtLeft = 
        y == 2 && x == 3 && childLayout.has_value() ? bugCount(childLayout.value(), Side::RIGHT) :
        x == 0 ? parentLayout.at(2).at(1) == BUG :
        currentLayout.at(y).at(x-1) == BUG;
      const int bugAtRight = 
        y == 2 && x == 1 && childLayout.has_value() ? bugCount(childLayout.value(), Side::LEFT) :
        x == rowSize-1 ? parentLayout.at(2).at(3) == BUG :
        currentLayout.at(y).at(x+1) == BUG;

      const int adjacentBugCount = bugAbove + bugBelow + bugAtLeft + bugAtRight;
      const bool recursionPoint = x == 2 && y == 2 && childLayout.has_value();

      if(recursionPoint || (inPlace == BUG && adjacentBugCount != 1)) {
        nextLayout.at(y).at(x) = EMPTY_SPACE;
      }
      else if(inPlace == EMPTY_SPACE && (adjacentBugCount == 1 || adjacentBugCount == 2)) {
        nextLayout.at(y).at(x) = BUG;
      }
    }
  }
  return nextLayout;
}

recursiveGrid getNextMinute(const recursiveGrid &currentRecLayout) {
  recursiveGrid nextRecLayout;
  if(currentRecLayout.front() != emptyGrid()) {
    nextRecLayout.push_back(getNextMinute(emptyGrid(), emptyGrid(), currentRecLayout.front()));
  }
  for (size_t i = 0; i < currentRecLayout.size(); i++){
    const auto parentLayout = i > 0 ? currentRecLayout.at(i-1) : emptyGrid();
    const auto childLayout = i < currentRecLayout.size() - 1 ? currentRecLayout.at(i+1) : emptyGrid();
    const auto next = getNextMinute(currentRecLayout.at(i), parentLayout, childLayout);
    nextRecLayout.push_back(next);
  }
  if(currentRecLayout.back() != emptyGrid()) {
    nextRecLayout.push_back(getNextMinute(emptyGrid(), currentRecLayout.back(), emptyGrid()));
  }
  return nextRecLayout;
}

grid getFirstRepeatingLayout(const grid &startLayout) {
  unordered_set<string> layoutSeen;
  auto currentLayout = startLayout;
  while(layoutSeen.find(layoutToString(currentLayout)) == layoutSeen.cend()) {
    layoutSeen.insert(layoutToString(currentLayout));
    currentLayout = getNextMinute(currentLayout);
  }
  return currentLayout;
}

const vector<vector<string>> testInput {
  {
    "....#",
    "#..#.",
    "#..##",
    "..#..",
    "#....",
  },
  {
    "#..#.",
    "####.",
    "###.#",
    "##.##",
    ".##..",
  },
  {
    "#####",
    "....#",
    "....#",
    "...#.",
    "#.###",
  }
};

const vector<string> testFirstRepeating {
  ".....",
  ".....",
  ".....",
  "#....",
  ".#...",
};

const vector<vector<string>> repeatingProblem {
  // Current
  {
    "#.###",
    "###.#",
    "#....",
    "##..#",
    "#..##",
  },
  // Parent
  {
    ".#.#.",
    "##..#",
    "#....",
    "##..#",
    ".#.#.",
  },
  // Child
  {
    "#####",
    "...#.",
    "...##",
    "..#..",
    "..#..",
  },
  // Expected
  {
    "#....",
    "....#",
    "...##",
    ".####",
    "####.",
  }
};

int main(int argc, char const *argv[]){
  // Part 1
  const auto testLayout = parseInput(testInput.at(0));
  assert(testLayout.at(4).at(4) == '.');
  assert(testLayout.at(4).at(0) == '#');

  const auto testLayoutAfter1 = parseInput(testInput.at(1));
  const auto testLayoutAfter2 = parseInput(testInput.at(2));
  assert(getNextMinute(testLayout) == testLayoutAfter1);
  assert(getNextMinute(getNextMinute(testLayout)) == testLayoutAfter2);

  const auto testRepeating = parseInput(testFirstRepeating);
  assert(getFirstRepeatingLayout(testLayout) == testRepeating);
  assert(biodiversityRating(testRepeating) == 2129920);

  const auto startLayout = parseInput(getPuzzleInput("inputs/aoc_day24_1.txt"));
  const auto p1Score = biodiversityRating(getFirstRepeatingLayout(startLayout));
  cout << "Part 1, first repeating bio score: " << p1Score << "\n";

  // Part 2
  assert(bugCount(testLayout) == 8);
  assert(bugCount(testLayout, Side::TOP) == 1);
  assert(bugCount(testLayout, Side::BOTTOM) == 1);
  assert(bugCount(testLayout, Side::LEFT) == 3);
  assert(bugCount(testLayout, Side::RIGHT) == 2);

  const vector<grid> testRecLayout {testLayout};
  assert(bugCount(testRecLayout) == 8);
  const auto testRecLayoutAfter1 = getNextMinute(testRecLayout);
  assert(bugCount(testRecLayoutAfter1) == 27);
  const auto testRecLayoutAfter2 = getNextMinute(testRecLayoutAfter1);
  assert(bugCount(testRecLayoutAfter2) == 20);

  assert(bugCount(parseInput(repeatingProblem.at(2)), Side::BOTTOM) == 1);
  const auto repeatingProblemTest = getNextMinute(
    parseInput(repeatingProblem.at(0)),
    parseInput(repeatingProblem.at(1)),
    parseInput(repeatingProblem.at(2))
  );
  assert(repeatingProblemTest == parseInput(repeatingProblem.at(3)));

  auto rlTest = testRecLayout;
  for (size_t i = 1; i <= 6; i++){
    rlTest = getNextMinute(rlTest);
  }
  assert(bugCount(rlTest) == 62);
  for (size_t i = 1; i <= 4; i++){
    rlTest = getNextMinute(rlTest);
  }
  assert(bugCount(rlTest) == 99);

  const vector<grid> startRecLayout {startLayout};
  auto rl = startRecLayout;
  for (size_t i = 0; i < 200; i++){
    rl = getNextMinute(rl);
  }
  cout << "Part 1, after 200min : " << rl.size() << " levels & " << bugCount(rl) << " bugs\n";
}

