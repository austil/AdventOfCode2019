#include <iostream>
#include <cassert>
#include "utils.cpp"

struct Coordinate
{
  size_t x = 0;
  size_t y = 0;
};

ostream &operator<<(ostream &os, const Coordinate &c)
{
  os << "(" << c.x << "," << c.y << ")";
  return os;
}

struct CoordinateHash
{
  size_t operator() (const Coordinate &c) const noexcept {
    return hash<string>()(to_string(c.x) + "," + to_string(c.y));
  }
};

bool operator== (const Coordinate &c1, const Coordinate &c2) {
  return c1.x == c2.x && c1.y == c2.y;
}

bool operator!= (const Coordinate &c1, const Coordinate &c2) {
  return !(c1 == c2);
}

vector<Coordinate> parse(vector<string> map)
{
  vector<Coordinate> asteroidCoordinates;
  for (size_t y = 0; y < map.size(); y++)
  {
    for (size_t x = 0; x < map.at(y).size(); x++)
    {
      const auto here = map.at(y).at(x);
      if (here == '#')
      {
        asteroidCoordinates.push_back({x, y});
      }
    }
  }
  return asteroidCoordinates;
}

bool areAligned(Coordinate a, Coordinate b, Coordinate c) {
  const auto crossProduct = (c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y);
  return crossProduct == 0;
}

int squaredDistance(Coordinate a, Coordinate b) {
  return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

Coordinate getAlignedSurrounded(Coordinate a, Coordinate b, Coordinate c) {
  const auto ab = squaredDistance(a, b);
  const auto bc = squaredDistance(b, c);
  const auto ca = squaredDistance(c, a);
  const auto aS = ab + ca;
  const auto bS = ab + bc;
  const auto cS = bc + ca;
  return (aS == bS && bS > cS) ? c : ((aS == cS && cS > bS) ? b : a);
};

struct StationCandidate 
{
  Coordinate coordinate {0, 0};
  int reachability = 0;
};

using blockedLineOfSightMap = unordered_map<Coordinate, unordered_set<Coordinate, CoordinateHash>, CoordinateHash>;

StationCandidate getBestAsteroid(const vector<Coordinate> &asteroids) {
  blockedLineOfSightMap blockedLineOfSight;
  for(const auto a : asteroids) {
    blockedLineOfSight[a] = {};
  }

  const auto n = asteroids.size();
  const vector<Coordinate> half1(asteroids.begin(), asteroids.end() - n / 2);
  const vector<Coordinate> half2(asteroids.begin() + n / 2, asteroids.end());
  const vector<vector<Coordinate>> possibleTriplets = cart_product<Coordinate>({asteroids, half1, half2});

  for(const auto triplet : possibleTriplets) {
    const bool noDuplicate = triplet[0] != triplet[1] 
      && triplet[0] != triplet[2] 
      && triplet[1] != triplet[2];

    if(noDuplicate && areAligned(triplet[0], triplet[1], triplet[2])) {
      const auto blocking = getAlignedSurrounded(triplet[0], triplet[1], triplet[2]);
      const auto blocked1 = triplet[0] != blocking ? triplet[0] : (triplet[1] != blocking ? triplet[1] : triplet[2]);
      const auto blocked2 = triplet[2] != blocking ? triplet[2] : (triplet[1] != blocking ? triplet[1] : triplet[0]);
      blockedLineOfSight[blocked1].insert(blocked2);
      blockedLineOfSight[blocked2].insert(blocked1);
      // cout << "Asteroid can't see each others: " << blocked1 << " & " << blocked2 << "\n";
    }
  }

  StationCandidate best;
  best.coordinate = asteroids.front();
  best.reachability = 0;
  for(const auto a : asteroids) {
    const int reachability = asteroids.size() - 1 - blockedLineOfSight[a].size();
    if(reachability > best.reachability) {
      best.reachability = reachability;
      best.coordinate = a;
    }
  }

  return best;
}

const vector<string> testMap1 = {
  ".#..#",
  ".....",
  "#####",
  "....#",
  "...##",
};

int main(int argc, char const *argv[])
{
  // Part 1
  const auto testAsteroids1 = parse(testMap1);
  assert(testAsteroids1.size() == 10);
  assert(testAsteroids1.front().x == 1);
  assert(testAsteroids1.front().y == 0);
  assert(testAsteroids1.back().x == 4);
  assert(testAsteroids1.back().y == 4);

  assert(cart_product<Coordinate>({testAsteroids1, testAsteroids1, testAsteroids1}).size() == 1000);
  assert(areAligned({0, 0}, {2, 4}, {1, 1}) == false);
  assert(areAligned({0, 0}, {2, 2}, {1, 1}) == true);
  assert(getAlignedSurrounded({0, 0}, {2, 2}, {1, 1}).x == 1);
  assert(getAlignedSurrounded({0, 0}, {1, 1}, {2, 2}).y == 1);

  assert(getBestAsteroid(testAsteroids1).coordinate.x == 3);
  assert(getBestAsteroid(testAsteroids1).coordinate.y == 4);
  assert(getBestAsteroid(testAsteroids1).reachability == 8);

  const auto p1Input = parse(getPuzzleInput("inputs/aoc_day10_1.txt"));
  cout << "Part1, got " << p1Input.size() << " asteroid\n"; 
  const auto p1 = getBestAsteroid(p1Input);
  cout << "Part1, asteroid " << p1.coordinate << " have the best reachability: " << p1.reachability << "\n";

  return 0;
}
