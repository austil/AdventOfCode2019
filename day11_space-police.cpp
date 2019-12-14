#include <iostream>
#include "IntcodeComputer.cpp"
#include "coordinate.cpp"

enum Color { Black, White };
enum Turn { LeftTurn, RightTurn };
enum Direction { Up, Right, Down, Left};

using PaintedPanels = unordered_map<Coordinate, Color, CoordinateHash>;

PaintedPanels paint(const Color startPanelColor = Black) {
  PaintedPanels panels;

  const auto getPanelColor = [&](Coordinate c) {
    const auto panelColor = panels.find(c);
    if(panelColor == panels.end()) {
      panels.insert({c, Black});
      return Black;
    }
    else {
      return panelColor->second;
    }
  };

  const auto painRobotProgram = parseIntcode(getPuzzleInput("inputs/aoc_day11_1.txt").front());
  auto robotProgram = runProgram(painRobotProgram);
  Coordinate robotCoordinate {0, 0};
  Direction robotDirection = Up;
  bool isFirstPanel = true;
  while(!robotProgram.terminated) {
    const auto currentPanelColor = isFirstPanel ? startPanelColor : getPanelColor(robotCoordinate);
    robotProgram.inputs.push(currentPanelColor);
    robotProgram = runProgram(robotProgram);
    const auto paintedColor = static_cast<Color>(robotProgram.outputs.at(robotProgram.outputs.size() - 2));
    const auto nextTurn = static_cast<Turn>(robotProgram.outputs.back());
    panels.insert_or_assign(robotCoordinate, paintedColor);

    if((robotDirection == Up && nextTurn == LeftTurn) || (robotDirection == Down && nextTurn == RightTurn)) {
      robotDirection = Left;
      robotCoordinate.x -= 1;
    } else if((robotDirection == Up && nextTurn == RightTurn) || (robotDirection == Down && nextTurn == LeftTurn)) {
      robotDirection = Right;
      robotCoordinate.x += 1;
    } else if((robotDirection == Left && nextTurn == RightTurn) || (robotDirection == Right && nextTurn == LeftTurn)) {
      robotDirection = Up;
      robotCoordinate.y += 1;
    } else {
      robotDirection = Down;
      robotCoordinate.y -= 1;
    }
    isFirstPanel = false;
  }

  return panels;
};

string printPaintedPanels(const PaintedPanels &p) {
  const int amplitude = p.size();
  Coordinate minCoord = {0, 0};
  Coordinate maxCoord = {0, 0};
  for (int y = -amplitude; y <= amplitude; y++)
  {
    for (int x = -amplitude; x <= amplitude; x++)
    {
      const Coordinate c = {x, y};
      const auto panelColor = p.find(c);
      if(panelColor != p.end()) {
        if(minCoord.x > x) minCoord.x = x;
        if(minCoord.y > y) minCoord.y = y;
        if(maxCoord.x < x) maxCoord.x = x;
        if(maxCoord.y < y) maxCoord.y = y;
      }
    }
  }

  string res = "";
  for (int y = minCoord.y; y <= maxCoord.y; y++)
  {
    string line = "";
    for (int x = minCoord.x; x <= maxCoord.x; x++)
    {
      const Coordinate c = {x, y};
      const auto panelColor = p.find(c);
      if(panelColor == p.end()) {
        line += " ";
      }
      else {
        line += panelColor->second == Black ? " " : "#";
      }
    }
    res += line;
    res += "$\n";
  }
  return res;
}

int main(int argc, char const *argv[])
{
  testComputer();

  const auto p1Panels = paint();
  cout << "Part1, covered panels (uniq): " << p1Panels.size() << "\n";

  const auto p2Panels = paint(White);
  cout << "Part2, panels:\n\n" << printPaintedPanels(p2Panels) << "\n";

  return 0;
}
