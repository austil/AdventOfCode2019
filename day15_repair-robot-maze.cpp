#include <iostream>
#include <algorithm>
#include "IntcodeComputer.cpp"
#include "coordinate.cpp"

enum class Move {North=1, South, West, East};
enum class DroidStatus {HitWall=1, Moved, MovedAndFoundSystem};
enum class Tile {Empty, Wall, OxygenSystem};

using Screen = unordered_map<Coordinate, Tile, CoordinateHash>;

string screenToString(const Screen &s) {
  int minX = 0;
  int maxX = 0;
  int minY = 0;
  int maxY = 0;

  for(const auto [coord, tile] : s) {
    if(minX > coord.x) minX = coord.x;
    else if(maxX < coord.x) maxX = coord.x;
    
    if(minY > coord.y) minY = coord.y;
    else if(maxY < coord.y) maxY = coord.y;
  }

  string res = "";
  for (int y = 0; y <= maxY; y++) {
    for (int x = 0; x <= maxX; x++) {
      const auto t = s.at({x,y});
      res += 
        t == Tile::Empty ? " " :
        t == Tile::Wall ? "%" :
        "O";
    }
    res += "\n";
  }
  return res;
}

int main(int argc, char const *argv[]){
  const auto remoteInput = getPuzzleInput("inputs/aoc_day15_1.txt").front();
  auto droid = runProgram(remoteInput);
  string instruction;
  while(!droid.terminated) {
    getline(cin, instruction);
    droid.outputs.clear();
    droid = runProgram(droid);
  }

  return 0;
}
