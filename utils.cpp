#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <tuple>
#include <vector>

using namespace std;

template <typename K, typename V>
ostream &operator<<(ostream &os, const unordered_map<K, V> &m)
{
  for (const pair<K, V> &p : m)
  {
    os << "{" << p.first << ": " << p.second << "}\n";
  }
  return os;
}

string trim(const string &s)
{
  auto wsfront = find_if_not(s.begin(), s.end(), [](int c) { return isspace(c); });
  return string(wsfront, find_if_not(s.rbegin(), string::const_reverse_iterator(wsfront), [](int c) { return isspace(c); }).base());
}

tuple<string, string> split(const string &s, const string delimiter) {
  const auto delimiterIndex = s.find(delimiter);
  const string firstPart = s.substr(0, delimiterIndex);
  const string secondPart = trim(s.substr(delimiterIndex + delimiter.size()));
  return make_tuple(firstPart, secondPart);
}

vector<string> getPuzzleInput(const string &path)
{
  ifstream inputFile(path);
  vector<string> circuit;
  for (string line; getline(inputFile, line);)
  {
    circuit.push_back(line);
  }
  return circuit;
}
