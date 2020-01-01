#include <iostream>
#include <numeric>
#include <cassert>
#include "utils.cpp"

struct vec3 {
  long x {0};
  long y {0};
  long z {0};
};


ostream &operator<<(ostream &os, const vec3 &v){
  os << "(" << v.x << "," << v.y << "," << v.z << ")";
  return os;
}

bool operator== (const vec3 v1, const vec3 v2) {
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

bool operator!= (const vec3 v1, const vec3 v2) {
  return !(v1 == v2);
}

vec3 operator+ (const vec3 v1, const vec3 v2) {
  return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

struct moon {
  vec3 position;
  vec3 velocity;
};

vector<moon> parseInput(const vector<string> &input) {
  vector<moon> moons;
  for(const auto line : input) {
    moon currentMoon;
    const auto values = split(line, ",");
    for(const auto v : values) {
      const auto [label, value] = cut(v, "=");
      const auto n = stoi(value);
      if(label == "<x") currentMoon.position.x = n;
      else if(label == " y") currentMoon.position.y = n;
      else if(label == " z") currentMoon.position.z = n;
    }
    moons.push_back(currentMoon);
  }
  return moons;
}

vector<moon> nextTimeStep(vector<moon> moons) {
  // Apply gravity to velocity
  for (size_t i = 0; i < moons.size(); i++) {
    auto &m1 = moons.at(i);
    for (size_t j = i+1; j < moons.size(); j++) {
      auto &m2 = moons.at(j);

      m1.velocity.x += m1.position.x > m2.position.x ? -1 : m1.position.x < m2.position.x ? 1 : 0;
      m1.velocity.y += m1.position.y > m2.position.y ? -1 : m1.position.y < m2.position.y ? 1 : 0;
      m1.velocity.z += m1.position.z > m2.position.z ? -1 : m1.position.z < m2.position.z ? 1 : 0;

      m2.velocity.x += m2.position.x > m1.position.x ? -1 : m2.position.x < m1.position.x ? 1 : 0;
      m2.velocity.y += m2.position.y > m1.position.y ? -1 : m2.position.y < m1.position.y ? 1 : 0;
      m2.velocity.z += m2.position.z > m1.position.z ? -1 : m2.position.z < m1.position.z ? 1 : 0;
    }
  }
  // Apply velocity to moon position
  for(auto &m : moons) {
    m.position = m.position + m.velocity;
  }
  return moons;
}

long norm(const vec3 v) {
  return abs(v.x) + abs(v.y) + abs(v.z);
}

long systemEnergy(const vector<moon> &system) {
  long energy = 0;
  for(const auto moon : system) {
    const auto potentialEnergy = norm(moon.position);
    const auto kineticEnergy = norm(moon.velocity);
    energy += potentialEnergy * kineticEnergy;
  }
  return energy;
}

vec3 stepsToLoopPerCoordinates(const vector<moon> &ms) {
  vec3 stepsToLoopPerCoord;
  unordered_set<string> xsAlreadySeen;
  unordered_set<string> ysAlreadySeen;
  unordered_set<string> zsAlreadySeen;

  auto moons = ms;
  while(stepsToLoopPerCoord.x == 0 || stepsToLoopPerCoord.y == 0 || stepsToLoopPerCoord.z == 0) {
    string xs, ys, zs;
    for(const auto m : moons) {
      xs += to_string(m.position.x) + ",";
      xs += to_string(m.velocity.x) + " ";
      ys += to_string(m.position.y) + ",";
      ys += to_string(m.velocity.y) + " ";
      zs += to_string(m.position.z) + ",";
      zs += to_string(m.velocity.z) + " ";
    }

    if(stepsToLoopPerCoord.x == 0 && xsAlreadySeen.find(xs) != xsAlreadySeen.cend()) {
      stepsToLoopPerCoord.x = xsAlreadySeen.size();
    }
    if(stepsToLoopPerCoord.y == 0 && ysAlreadySeen.find(ys) != ysAlreadySeen.cend()) {
      stepsToLoopPerCoord.y = ysAlreadySeen.size();
    }
    if(stepsToLoopPerCoord.z == 0 && zsAlreadySeen.find(zs) != zsAlreadySeen.cend()) {
      stepsToLoopPerCoord.z = zsAlreadySeen.size();
    }

    xsAlreadySeen.insert(xs);
    ysAlreadySeen.insert(ys);
    zsAlreadySeen.insert(zs);
    moons = nextTimeStep(moons);
  }

  return stepsToLoopPerCoord;
}

const vector<string> testInput1 {
  "<x=-1, y=0, z=2>",
  "<x=2, y=-10, z=-7>",
  "<x=4, y=-8, z=8>",
  "<x=3, y=5, z=-1>",
};

const vector<string> testInput2 {
  "<x=-8, y=-10, z=0>",
  "<x=5, y=5, z=10>",
  "<x=2, y=-7, z=3>",
  "<x=9, y=-8, z=-3>",
};

int main(int argc, char const *argv[])
{
  // Parsing tests
  const auto testMoons1 = parseInput(testInput1);
  assert(testMoons1.size() == 4);
  assert(testMoons1.front().velocity == id<vec3>({0, 0, 0}));
  assert(testMoons1.front().position == id<vec3>({-1, 0, 2}));
  assert(testMoons1.back().velocity == id<vec3>({0, 0, 0}));
  assert(testMoons1.back().position == id<vec3>({3, 5, -1}));

  // TimeStep tests
  const auto testMoonsAfter1 = nextTimeStep(testMoons1);
  assert(testMoonsAfter1.size() == 4);
  assert(testMoonsAfter1.front().velocity == id<vec3>({3, -1, -1}));
  assert(testMoonsAfter1.front().position == id<vec3>({2, -1, 1}));
  assert(testMoonsAfter1.back().velocity == id<vec3>({-1, -3, 1}));
  assert(testMoonsAfter1.back().position == id<vec3>({2, 2, 0}));

  auto testMoonsTs = testMoons1;
  for (size_t i = 0; i < 10; i++) {
    testMoonsTs = nextTimeStep(testMoonsTs);
  }
  assert(testMoonsTs.size() == 4);
  assert(testMoonsTs.front().velocity == id<vec3>({-3, -2, 1}));
  assert(testMoonsTs.front().position == id<vec3>({2, 1, -3}));
  assert(testMoonsTs.back().velocity == id<vec3>({1, -1, -1}));
  assert(testMoonsTs.back().position == id<vec3>({2, 0, 4}));
  
  // Metrics tests
  assert(systemEnergy(testMoonsTs) == 179);

  // Part 1
  const auto input = parseInput(getPuzzleInput("inputs/aoc_day12_1.txt"));
  auto p1Moons = input;
  for (size_t i = 0; i < 1000; i++) {
    p1Moons = nextTimeStep(p1Moons);
  }
  cout << "Part1, system energy : " << systemEnergy(p1Moons) << "\n";

  // Part 2
  const auto testLoopPerCoord1 = stepsToLoopPerCoordinates(testMoons1);
  const auto testStepsToLoop1 = lcm(lcm(testLoopPerCoord1.x, testLoopPerCoord1.y), testLoopPerCoord1.z);
  assert(testStepsToLoop1 == 2772);

  const auto testMoons2 = parseInput(testInput2);
  const auto testLoopPerCoord2 = stepsToLoopPerCoordinates(testMoons2);
  const auto testStepsToLoop2 = lcm(lcm(testLoopPerCoord2.x, testLoopPerCoord2.y), testLoopPerCoord2.z);
  assert(testStepsToLoop2 == 4686774924);

  const auto loopPerCoord = stepsToLoopPerCoordinates(input);
  const auto stepsToLoop = lcm(lcm(loopPerCoord.x, loopPerCoord.y), loopPerCoord.z);
  cout << "Part2, steps to loop: " << stepsToLoop << "\n";

  return 0;
}
