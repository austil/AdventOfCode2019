#include <iostream>
#include <algorithm>
#include "IntcodeComputer.cpp"
#include "coordinate.cpp"

enum class Tile {Empty, Wall, Block, HorizontalPaddle, Ball};

using Screen = unordered_map<Coordinate, Tile, CoordinateHash>;

int main(int argc, char const *argv[])
{
  testComputer();

  const auto gameInput = parseIntcode(getPuzzleInput("inputs/aoc_day13_1.txt").front());

  Screen screen;
  auto gameProgram = runProgram(gameInput);
  
  for(size_t i = 0; i < gameProgram.outputs.size(); i += 3) {
    const int x = gameProgram.outputs.at(i);
    const int y = gameProgram.outputs.at(i+1);
    const Tile currentTile = static_cast<Tile>(gameProgram.outputs.at(i+2));
    if(currentTile != Tile::Empty) {
      screen.insert_or_assign({x, y}, currentTile);
    }
  }

  const auto blockTileCount = count_if(screen.cbegin(), screen.cend(), [](pair<Coordinate, Tile> entry){
    return entry.second == Tile::Block;
  });
  cout << "Part1, count of Block Tiles: " << blockTileCount << "\n";

  return 0;
}
