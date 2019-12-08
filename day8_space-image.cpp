#include <iostream>
#include <cassert>
#include "utils.cpp"

vector<string> getLayers(const string digits, int pixelWidth, int pixelHeight) {
  vector<string> layers;
  const int pixelPerLayer = pixelWidth * pixelHeight;
  const int layerCount = digits.size() / pixelPerLayer;
  for (int i = 0; i < layerCount; i++)
  {
    layers.push_back(digits.substr(pixelPerLayer * i, pixelPerLayer));
  }
  return layers;
}

int findCount(string str, string subStrToFind) {
  int findCount = 0;
  int findPos = str.find(subStrToFind);
  while(findPos != string::npos) {
    findCount++;
    findPos = str.find(subStrToFind, findPos + subStrToFind.size());
  }
  return findCount;
}

string layerWithFewestZeros(const vector<string> &layers) {
  auto layerWithLeastZeros = layers.front();
  int leastZerosCount = layers.front().size();
  for(auto currentLayer : layers) {
    int zerosInCurrentLayer = findCount(currentLayer, "0");
    if(zerosInCurrentLayer < leastZerosCount) {
      layerWithLeastZeros = currentLayer;
      leastZerosCount = zerosInCurrentLayer;
    }
  }
  return layerWithLeastZeros;
}

string decodeImage(const vector<string> &layers) {
  string image = layers.back();
  for (int i = layers.size() - 1; i >= 0; i--)
  {
    const auto currentLayer = layers.at(i);
    for (size_t p = 0; p < currentLayer.size(); p++)
    {
      if(currentLayer.at(p) != '2') {
        image.at(p) = currentLayer.at(p);
      }
    }
    
  }
  return image;
}

int main(int argc, char const *argv[])
{
  // Part 1
  const auto testLayers = getLayers("123456789012", 3, 2);
  assert(testLayers.size() == 2);
  assert(testLayers.front().size() == 6);
  assert(testLayers.back().size() == 6);
  assert(findCount("00010", "0") == 4);
  assert(findCount("00012", "2") == 1);
  assert(layerWithFewestZeros(testLayers) == "123456");

  const auto p1Input = getPuzzleInput("./inputs/aoc_day8_1.txt").front();
  const auto p1Layers = getLayers(p1Input, 25, 6);
  const auto p1 = layerWithFewestZeros(p1Layers);
  const auto p1Ones = findCount(p1, "1");
  const auto p1Twos = findCount(p1, "2");
  cout << "Part 1: " << p1Ones << "*" << p1Twos << " = " << (p1Ones * p1Twos) << " \n";


  // Part 2
  assert(decodeImage({"0222", "1122", "2212" ,"0000"}) == "0110");
  const auto p2 = decodeImage(p1Layers);
  cout << "Part 2: " << getLayers(p2, 25, 1) << "\n";

  return 0;
}
