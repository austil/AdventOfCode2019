#include <iostream>
#include <cassert>
#include <string>
#include "utils.cpp"

unordered_map<string, string> parseToOrbitMap(const vector<string> &orbitsInfo)
{
  unordered_map<string, string> reverseOrbitGraph;
  for (auto orbit : orbitsInfo)
  {
    const auto [center, satellite] = cut(orbit, ")");
    reverseOrbitGraph[satellite] = center;
  }
  return reverseOrbitGraph;
}

struct OrbitsPathObject
{
  string object;
  size_t transfertFromSatellite;
};

struct OrbitsPathObjectHash
{
  size_t operator() (const OrbitsPathObject &o) const noexcept {
    return hash<string>()(o.object);
  }
};

bool operator== (const OrbitsPathObject &p1, const OrbitsPathObject &p2) {
  return p1.object == p2.object;
}

vector<OrbitsPathObject> orbitsPath(
  const unordered_map<string, string> &orbitGraph,
  string object,
  vector<OrbitsPathObject> path = {}
)
{
  const auto orbiting = orbitGraph.find(object);
  if (orbiting == orbitGraph.end())
  {
    return path;
  }
  path.push_back({orbiting->second, path.size()});
  return orbitsPath(orbitGraph, orbiting->second, path);
}

int orbitsSum(const unordered_map<string, string> &orbitGraph)
{
  int orbitSum = 0;
  for (auto it = orbitGraph.begin(); it != orbitGraph.end(); ++it)
  {
    orbitSum += orbitsPath(orbitGraph, it->first).size();
  }
  return orbitSum;
}

int minimalOrbitTransfer(
  const unordered_map<string, string> &orbitGraph,
  string start,
  string destination
)
{
  const auto startObjOrbitsPath = orbitsPath(orbitGraph, start);
  const auto destObjOrbitsPath = orbitsPath(orbitGraph, destination);
  
  unordered_set<OrbitsPathObject, OrbitsPathObjectHash> alreadySeen (startObjOrbitsPath.begin(), startObjOrbitsPath.end());
  for(auto opo : destObjOrbitsPath) {
    const auto objectSeen = alreadySeen.find(opo);
    if (objectSeen != alreadySeen.end()) {
      return opo.transfertFromSatellite + (*objectSeen).transfertFromSatellite;
    }
  }

  return 0;
}

vector<string> testInput1{"COM)B", "B)C", "C)D", "D)E", "E)F", "B)G", "G)H", "D)I", "E)J", "J)K", "K)L"};
vector<string> testInput2{"COM)B", "B)C", "C)D", "D)E", "E)F", "B)G", "G)H", "D)I", "E)J", "J)K", "K)L", "K)YOU", "I)SAN"};

int main(int argc, char const *argv[])
{
  // Part 1
  assert(orbitsPath(parseToOrbitMap(testInput1), "D").size() == 3);
  assert(orbitsPath(parseToOrbitMap(testInput1), "L").size() == 7);
  assert(orbitsPath(parseToOrbitMap(testInput1), "COM").size() == 0);
  assert(orbitsSum(parseToOrbitMap(testInput1)) == 42);

  const auto inputOrbitMap = parseToOrbitMap(getPuzzleInput("./inputs/aoc_day5_1.txt"));
  const auto p1 = orbitsSum(inputOrbitMap);
  cout << "Part 1, total number of direct and indirect orbits: " << p1 << "\n";

  // Part 2
  assert(minimalOrbitTransfer(parseToOrbitMap(testInput2), "YOU", "SAN") == 4);
  assert(minimalOrbitTransfer(parseToOrbitMap(testInput2), "F", "SAN") == 2);
  assert(minimalOrbitTransfer(parseToOrbitMap(testInput2), "H", "SAN") == 4);

  const auto p2 = minimalOrbitTransfer(inputOrbitMap, "YOU", "SAN");
  cout << "Part 2, minimum number of orbital transfers required: " << p2 << "\n";

  return 0;
}
