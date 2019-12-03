#include <iostream>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <cmath>
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

struct movementSummary
{
  int nextX {0};
  int nextY {0};
  unordered_set<string> positions;
};


movementSummary getMovementSummary(const int startX, const int startY, const string movement)
{
  movementSummary summary;
  const char direction = movement.front();
  const int length = stoi(movement.substr(1));

  summary.nextX = startX + (direction == 'R'? length : (direction == 'L' ? -length : 0));
  summary.nextY = startY + (direction == 'U'? length : (direction == 'D' ? -length : 0));

  for (int i = 1; i <= length; i++)
  {
    int x = startX;
    int y = startY;
    if      (direction == 'U') { y += i; }
    else if (direction == 'D') { y -= i; }
    else if (direction == 'L') { x -= i; }
    else if (direction == 'R') { x += i; }
    else {
      cerr << "Unexpected direction for movement: " << movement << "\n";
      throw;
    }
    const auto pos = to_string(x) + "," + to_string(y);
    summary.positions.insert(pos);
  }

  return summary;
}

int nearestCrossing(const vector<vector<string>> wires)
{
  vector<unordered_set<string>> positionsUsedByWires;
  for(auto &wire : wires) {
    int x = 0;
    int y = 0;
    unordered_set<string> positions;
    for(auto &movement: wire) {
      movementSummary summary = getMovementSummary(x, y, movement);
      x = summary.nextX;
      y = summary.nextY;
      positions.insert(summary.positions.begin(), summary.positions.end());
    }
    positionsUsedByWires.push_back(positions);
  }

  int shortestDistance = 0;
  for(auto position : positionsUsedByWires[0]) {
    if(positionsUsedByWires[1].find(position) != positionsUsedByWires[1].end()) {
      const auto [x,y] = cut(position, ",");
      const int distance = abs(stoi(x)) + abs(stoi(y));
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
  movementSummary m1 = getMovementSummary(0, 0, "R2");
  assert(m1.positions.size() == 2);
  assert(m1.nextX == 2);
  assert(m1.nextY == 0);
  movementSummary m2 = getMovementSummary(0, 0, "D8");
  assert(m2.positions.size() == 8);
  assert(m2.nextX == 0);
  assert(m2.nextY == -8);
  movementSummary m3 = getMovementSummary(8, 0, "U5");
  assert(m3.nextX == 8);
  assert(m3.nextY == 5);

  const vector<string> wires1 = {"R8,U5,L5,D3", "U7,R6,D4,L4"};
  assert(nearestCrossing(parse(wires1)) == 6);
  const vector<string> wires2 = {"R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83"};
  assert(nearestCrossing(parse(wires2)) == 159);
  const vector<string> wires3 = {"R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7"};
  assert(nearestCrossing(parse(wires3)) == 135);

  cout << "part1 - shortest distance: " << nearestCrossing(parse(getPuzzleInput("./inputs/aoc_day3_1.txt"))) << "\n";

  return 0;
}
