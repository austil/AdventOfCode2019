using namespace std;

struct Coordinate
{
  int x = 0;
  int y = 0;
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
