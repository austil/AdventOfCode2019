#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <tuple>
#include <vector>
#include <cmath>

using namespace std;

template<typename T>
T id(T v) {
  return v;
}

template <typename K, typename V>
ostream &operator<<(ostream &os, const unordered_map<K, V> &m)
{
  for (const pair<K, V> &p : m)
  {
    os << "{" << p.first << ": " << p.second << "}\n";
  }
  return os;
}

template <typename K, typename V, typename H>
ostream &operator<<(ostream &os, const unordered_map<K, V, H> &m)
{
  for (const pair<K, V> &p : m)
  {
    os << "{" << p.first << ": " << p.second << "}\n";
  }
  return os;
}

template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v)
{
  os << "{\n";
  for (const auto &e : v)
  {
    os << "  " << e << ",\n";
  }
  os << "}\n";
  return os;
}

template <typename T>
ostream &operator<<(ostream &os, const unordered_set<T> &v)
{
  os << "{\n";
  for (const auto &e : v)
  {
    os << "  " << e << ",\n";
  }
  os << "}\n";
  return os;
}

// https://stackoverflow.com/a/17050528/6289951
// Uselessly slow
template <typename T>
vector<vector<T>> cart_product(const vector<vector<T>> &v)
{
  const auto n = pow(v.front().size(), v.size());
  if(n > 100000) {
    cout << "cart_product will take some time, n=" << n << "\n";
  }

  vector<vector<T>> s = {{}};
  for (const auto &u : v)
  {
    vector<vector<T>> r;
    for (const auto &x : s)
    {
      for (const auto y : u)
      {
        r.push_back(x);
        r.back().push_back(y);
      }
      if(n > 100000 && r.size() % 10000 == 0) {
        cout << "cart_product progess: +" << r.size() << " / " << n << "\n";
      }
    }
    s = move(r);
    if(n > 100000) {
      cout << "cart_product progess: " << s.size() << " / " << n << "\n";
    }
  }
  return s;
}

string trim(const string &s)
{
  auto wsfront = find_if_not(s.begin(), s.end(), [](int c) { return isspace(c); });
  return string(wsfront, find_if_not(s.rbegin(), string::const_reverse_iterator(wsfront), [](int c) { return isspace(c); }).base());
}

tuple<string, string> cut(const string &s, const string delimiter) {
  const auto delimiterIndex = s.find(delimiter);
  const string firstPart = s.substr(0, delimiterIndex);
  const string secondPart = trim(s.substr(delimiterIndex + delimiter.size()));
  return make_tuple(firstPart, secondPart);
}

vector<string> split(const string &s, const string delimiter) {
  size_t delimiterIndex = 0;
  string str = s;
  vector<string> tokens;
  while ((delimiterIndex = str.find(delimiter)) != string::npos) {
    const string token = str.substr(0, delimiterIndex);
    tokens.push_back(token);
    str = str.substr(delimiterIndex + delimiter.length());
  }
  tokens.push_back(str);
  return tokens;
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
