#include <iostream>
#include <algorithm>
#include "IntcodeComputer.cpp"
#include "coordinate.cpp"

enum class Tile {Empty, Wall, Block, HorizontalPaddle, Ball};
enum class JoystickMove {Left=-1, Neutral=0, Right=1};

using Screen = unordered_map<Coordinate, Tile, CoordinateHash>;

Screen readGameScreen(const vector<IntCode> &programOutputs) {
  Screen screen;
  for(size_t i = 0; i < programOutputs.size(); i += 3) {
    const int x = programOutputs.at(i);
    const int y = programOutputs.at(i+1);
    if(x >= 0 && y >= 0) {
      const Tile currentTile = static_cast<Tile>(programOutputs.at(i+2));
      screen.insert_or_assign({x, y}, currentTile);
    }
  }
  return screen;
}

IntCode readGameScore(const vector<IntCode> &programOutputs) {
  IntCode score = 0;
  for(size_t i = 0; i < programOutputs.size(); i += 3) {
    const int x = programOutputs.at(i);
    const int y = programOutputs.at(i+1);
    if(x == -1 && y == 0) {
      score = programOutputs.at(i+2);
    }
  }
  return score;
}

string screenToString(const Screen &s) {
  size_t maxX = 0;
  size_t maxY = 0;

  for(const auto [coord, tile] : s) {
    if(maxX < coord.x) maxX = coord.x;
    if(maxY < coord.y) maxY = coord.y;
  }

  string res = "";
  for (size_t y = 0; y <= maxY; y++) {
    for (size_t x = 0; x <= maxX; x++) {
      const auto t = s.at({x,y});
      res += 
        t == Tile::Empty ? " " :
        t == Tile::Wall ? "%" :
        t == Tile::Block ? "#" :
        t == Tile::HorizontalPaddle ? "_" :
        "O";
    }
    res += "\n";
  }
  return res;
}

int main(int argc, char const *argv[])
{
  testComputer();

  const auto gameInput = parseIntcode(getPuzzleInput("inputs/aoc_day13_1.txt").front());

  // Part 1
  const auto gameProgram = runProgram(gameInput);
  const auto screen = readGameScreen(gameProgram.outputs);
  const auto blockTileCount = count_if(screen.cbegin(), screen.cend(), [](pair<Coordinate, Tile> entry){
    return entry.second == Tile::Block;
  });
  cout << "Part1, count of Block Tiles: " << blockTileCount << "\n";

  // Part 2
  auto freeGameInput = gameInput;
  freeGameInput[0] = 2;
  auto freeGameProgram = runProgram(freeGameInput);

  while (!freeGameProgram.terminated) {
    // Screen reading could be optimized, only updated tiles are outputed, no need to read everything
    const auto screen = readGameScreen(freeGameProgram.outputs);

    const auto ball = find_if(screen.cbegin(), screen.cend(), [](pair<Coordinate, Tile> entry){
      return entry.second == Tile::Ball;
    });
    const auto paddle = find_if(screen.cbegin(), screen.cend(), [](pair<Coordinate, Tile> entry){
      return entry.second == Tile::HorizontalPaddle;
    });

    if(ball == screen.cend()) {
      cerr << "Ball not found\n";
      throw;
    }
    if(paddle == screen.cend()) {
      cerr << "Paddle not found\n";
      throw;
    }

    const Coordinate ballCoordinate = ball->first;
    const Coordinate paddleCoordinate = paddle->first;
    const JoystickMove nextMove = 
      ballCoordinate.x < paddleCoordinate.x ? JoystickMove::Left :
      ballCoordinate.x > paddleCoordinate.x ? JoystickMove::Right :
      JoystickMove::Neutral;

    cout << screenToString(screen);
    cout << "Score: " << readGameScore(freeGameProgram.outputs) << "\n";
    cout << ballCoordinate << ", ";
    cout << paddleCoordinate << " => ";
    cout << static_cast<IntCode>(nextMove) << "\n\n";

    freeGameProgram.inputs.push(static_cast<IntCode>(nextMove));
    freeGameProgram = runProgram(freeGameProgram);
  }
  
  cout << "Part2: game over, final score is " << readGameScore(freeGameProgram.outputs) << "\n";

  return 0;
}
