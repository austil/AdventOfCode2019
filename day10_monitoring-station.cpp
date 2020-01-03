#include <iostream>
#include <map>
#include <set>
#include <cassert>
#include "utils.cpp"
#include "coordinate.cpp"

const auto PI = 3.141592653589793238463;

vector<Coordinate> parse(vector<string> map)
{
  vector<Coordinate> asteroidCoordinates;
  for (size_t y = 0; y < map.size(); y++) {
    for (size_t x = 0; x < map.at(y).size(); x++) {
      const auto here = map.at(y).at(x);
      if (here == '#') {
        asteroidCoordinates.push_back({static_cast<int>(x), static_cast<int>(y)});
      }
    }
  }
  return asteroidCoordinates;
}

bool areAligned(Coordinate a, Coordinate b, Coordinate c) {
  return (a.y - b.y) * (a.x - c.x) == (a.y - c.y) * (a.x - b.x);
}

int squaredDistance(Coordinate a, Coordinate b) {
  return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}

Coordinate getAlignedSurrounded(Coordinate a, Coordinate b, Coordinate c) {
  const auto ab = squaredDistance(a, b);
  const auto bc = squaredDistance(b, c);
  const auto ca = squaredDistance(c, a);
  // cout << ab << " " << bc << " " << ca << "\n";
  const auto maxDistance = max({ab, bc, ca});
  return maxDistance == ca ? b : (maxDistance == bc ? a : c);
};

struct StationCandidate 
{
  Coordinate coordinate {0, 0};
  int reachability = 0;
};

vector<vector<Coordinate>> getTriplets(const vector<Coordinate> &asteroids) {
  vector<vector<Coordinate>> possibleTriplets;
  for (size_t i = 0; i < asteroids.size(); i++){
    for (size_t j = i+1; j < asteroids.size(); j++){
      for (size_t k = j+1; k < asteroids.size(); k++){
        possibleTriplets.push_back({asteroids.at(i), asteroids.at(j), asteroids.at(k)});
      }
    }
  }
  return possibleTriplets;
};

using blockedLineOfSightMap = unordered_map<Coordinate, unordered_set<Coordinate, CoordinateHash>, CoordinateHash>;

StationCandidate getBestAsteroid(const vector<Coordinate> &asteroids) {
  blockedLineOfSightMap blockedLineOfSight;
  for(const auto a : asteroids) {
    blockedLineOfSight[a] = {};
  }

  const auto possibleTriplets = getTriplets(asteroids);
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
      // cout << "Asteroid can't see each others: " << blocked1 << " & " << blocked2 << " because " << blocking << "\n";
    }
  }

  StationCandidate best;
  best.coordinate = asteroids.front();
  best.reachability = 0;
  for(const auto a : asteroids) {
    const int reachability = asteroids.size() - 1 - blockedLineOfSight[a].size();
    // cout << a << " " << reachability << "\n";
    if(reachability > best.reachability) {
      best.reachability = reachability;
      best.coordinate = a;
    }
  }
  // cout << "\n";

  return best;
}

struct compDistanceToStation {
  Coordinate monitoringStation;
  bool operator()(const Coordinate &a, const Coordinate &b) {
    return squaredDistance(monitoringStation, a) > squaredDistance(monitoringStation, b);
  };
};

vector<Coordinate> vaporizeAsteroids(const vector<Coordinate> &asteroids, const Coordinate &monitoringStation) {
  compDistanceToStation comp {monitoringStation};
  map<double, set<Coordinate, decltype(comp)>> asteroidAngles;
  for(const auto &a : asteroids) {
    if(a != monitoringStation) {
      const double angleFromAbscissa = atan2(monitoringStation.y - a.y, monitoringStation.x - a.x) * (180 / PI);
      const double angleFromLaserStart = 
        (90 <= angleFromAbscissa && angleFromAbscissa <= 180) ? angleFromAbscissa - 90 :
        (-180 <= angleFromAbscissa && angleFromAbscissa <= 90) ? 270 + angleFromAbscissa : 0;
      const double roundedAngle = round(angleFromLaserStart * 1000.0) / 1000.0;
      asteroidAngles[roundedAngle].insert(a);
      // cout << a << " : " << angleFromAbscissa << " => " << roundedAngle << "\n";
    }
  }

  vector<Coordinate> vaporized;
  size_t asteroidVaporizedLastSpin = 1;

  while(asteroidVaporizedLastSpin > 0) {
    asteroidVaporizedLastSpin = 0;
    for(auto it = asteroidAngles.begin(); it != asteroidAngles.end(); ++it) {
      auto &asteroidsInLine = it->second;
      // cout << it->first << " " << it->second.size() << "\n";
      if(!asteroidsInLine.empty()) {
        vaporized.push_back(*asteroidsInLine.begin());
        asteroidsInLine.erase(asteroidsInLine.begin());
        asteroidVaporizedLastSpin++;
      }
    }
    // cout << "Vaporized " << asteroidVaporizedLastSpin << "\n";
  }
  return vaporized;
}

const vector<string> testMap1 = {
  ".#..#",
  ".....",
  "#####",
  "....#",
  "...##",
};

const vector<string> testMap2 = {
  "......#.#.",
  "#..#.#....",
  "..#######.",
  ".#.#.###..",
  ".#..#.....",
  "..#....#.#",
  "#..#....#.",
  ".##.#..###",
  "##...#..#.",
  ".#....####",
};

const vector<string> testMap3 = {
  ".#....#####...#..",
  "##...##.#####..##",
  "##...#...#.#####.",
  "..#.....X...###..",
  "..#.#.....#....##",
};

int main(int argc, char const *argv[])
{
  assert(areAligned({0, 0}, {2, 4}, {1, 1}) == false);
  assert(areAligned({0, 0}, {2, 2}, {1, 1}) == true);
  assert(getAlignedSurrounded({1,0}, {3,2}, {4,3}).x == 3);
  assert(getAlignedSurrounded({1,0}, {4,3}, {3,2}).x == 3);
  assert(getAlignedSurrounded({3,2}, {4,3}, {1,0}).x == 3);
  assert(getAlignedSurrounded({4,3}, {3,2}, {1,0}).x == 3);

  // Part 1
  const auto testAsteroids1 = parse(testMap1);
  assert(testAsteroids1.size() == 10);
  assert(testAsteroids1.front().x == 1);
  assert(testAsteroids1.front().y == 0);
  assert(testAsteroids1.back().x == 4);
  assert(testAsteroids1.back().y == 4);

  const auto t1Res = getBestAsteroid(testAsteroids1);
  assert(t1Res.coordinate.x == 3);
  assert(t1Res.coordinate.y == 4);
  assert(t1Res.reachability == 8);

  const auto t2Res = getBestAsteroid(parse(testMap2));
  assert(t2Res.coordinate.x == 5);
  assert(t2Res.coordinate.y == 8);
  assert(t2Res.reachability == 33);

  const auto p1Input = parse(getPuzzleInput("inputs/aoc_day10_1.txt"));
  cout << "Part1, got " << p1Input.size() << " asteroid\n"; 
  const auto p1 = getBestAsteroid(p1Input);
  cout << "Part1, asteroid " << p1.coordinate << " have the best reachability: " << p1.reachability << "\n";

  // Part 2
  const Coordinate testMonitoringStation {8,3};
  const auto testAsteroids3 = parse(testMap3);
  const auto testVaporizeOrder = vaporizeAsteroids(testAsteroids3, testMonitoringStation);
  assert(testVaporizeOrder.size() == testAsteroids3.size());
  assert(testVaporizeOrder.at(0) == id<Coordinate>({8, 1}));
  assert(testVaporizeOrder.at(30) == id<Coordinate>({8, 0}));

  const auto vaporizeOrder = vaporizeAsteroids(p1Input,  p1.coordinate);
  cout << "Part2, 200th asteroid vaporized: " << vaporizeOrder.at(199) << "\n";

  return 0;
}
