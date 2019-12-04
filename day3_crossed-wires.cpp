#include <iostream>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <cmath>
#include <string>
#include "utils.cpp"

vector<vector<string>> parse(const vector<string> strs)
{
  vector<vector<string>> wires;
  for (auto line : strs)
  {
    wires.push_back(split(line, ","));
  }
  return wires;
}

std::hash<std::string> hash_fn;
struct position
{
  int x {0};
  int y {0};
  int wireLength {0};

  size_t operator() (const position &p) const noexcept {
    return hash_fn(to_string(p.x) + "," + to_string(p.y));
  }
};

bool operator== (const position &p1, const position &p2) {
  return p1.x == p2.x && p1.y == p2.y;
}


vector<position> getMovementSummary(position startPosition, const string movement)
{
  vector<position> positions;
  const char direction = movement.front();
  const int length = stoi(movement.substr(1));

  for (int i = 1; i <= length; i++)
  {
    int x = startPosition.x;
    int y = startPosition.y;
    if      (direction == 'U') { y += i; }
    else if (direction == 'D') { y -= i; }
    else if (direction == 'L') { x -= i; }
    else if (direction == 'R') { x += i; }
    else {
      cerr << "Unexpected direction for movement: " << movement << "\n";
      throw;
    }
    const position pos = { x, y, startPosition.wireLength + length };
    positions.push_back(pos);
  }

  return positions;
}

int nearestCrossing(const vector<vector<string>> wires)
{
  vector<unordered_set<position, position>> positionsUsedByWires;
  for(auto &wire : wires) {
    position currentPosition = { 0, 0, 0 };
    unordered_set<position, position> positions;
    for(auto &movement: wire) {
      auto summary = getMovementSummary(currentPosition, movement);
      currentPosition = summary.back();
      positions.insert(summary.begin(), summary.end());
    }
    positionsUsedByWires.push_back(positions);
  }

  int shortestDistance = 0;
  for(auto position : positionsUsedByWires[0]) {
    if(positionsUsedByWires[1].find(position) != positionsUsedByWires[1].end()) {
      const int distance = abs(position.x) + abs(position.y);
      // cout << position << " @ " << distance << "\n";
      if(shortestDistance == 0 || shortestDistance > distance) {
        shortestDistance = distance;
      }
    }
  }

  return shortestDistance;
}

int main(int argc, char const *argv[])
{
  // Part 1
  vector<position> m1 = getMovementSummary({0, 0, 0}, "R2");
  assert(m1.size() == 2);
  assert(m1.back().wireLength == 2);
  assert(m1.back().x == 2);
  assert(m1.back().y == 0);
  vector<position> m2 = getMovementSummary({0, 0, 0}, "D8");
  assert(m2.size() == 8);
  assert(m2.back().wireLength == 8);
  assert(m2.back().x == 0);
  assert(m2.back().y == -8);
  vector<position> m3 = getMovementSummary({8, 0, 10}, "U5");
  assert(m3.back().wireLength == 15);
  assert(m3.back().x == 8);
  assert(m3.back().y == 5);

  const vector<string> wires1 = {"R8,U5,L5,D3", "U7,R6,D4,L4"};
  assert(nearestCrossing(parse(wires1)) == 6);
  const vector<string> wires2 = {"R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83"};
  assert(nearestCrossing(parse(wires2)) == 159);
  const vector<string> wires3 = {"R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7"};
  assert(nearestCrossing(parse(wires3)) == 135);

  cout << "part1 - shortest distance: " << nearestCrossing(parse(getPuzzleInput("./inputs/aoc_day3_1.txt"))) << "\n";

  return 0;
}
