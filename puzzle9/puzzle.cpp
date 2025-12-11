#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <memory>
#include <numeric>
#include <ostream>
#include <vector>
// todo: map the distances (in a heap?) the input i the usual > 5KiB
// connect the fuses
// remember use the squared distance, since squares and their (positive) roots
// are in the same order check

struct filedeleter {
  void operator()(FILE *f) { std::fclose(f); }
};
struct point {
  long x;
  long y;
};
std::ostream &operator<<(std::ostream &os, const point &p) {
  os << p.x << " " << p.y;
  return os;
}
unsigned long distance(const point &a, const point &b) {
  unsigned long x = (a.x > b.x) ? a.x - b.x : b.x - a.x;
  unsigned long y = (a.y > b.y) ? a.y - b.y : b.y - a.y;
  return x * x + y * y;
}
unsigned long area(const point &a, const point &b) {
  // the perimeter is in the area
  unsigned long x = 1 + ((a.x > b.x) ? a.x - b.x : b.x - a.x);
  unsigned long y = 1 + ((a.y > b.y) ? a.y - b.y : b.y - a.y);
  return x * y;
}

// everithing is public because I do not want to setup getters
int main() {
// #define TESTING
#ifdef TESTING
  // std::ifstream f("./testInput.txt");
  std::unique_ptr<FILE, filedeleter> f{std::fopen("./testInput.txt", "r")};
  constexpr unsigned connections = 10;
#else
  // std::ifstream f("./Input.txt");
  std::unique_ptr<FILE, filedeleter> f{std::fopen("./Input.txt", "r")};
  constexpr unsigned connections = 1000;
#endif
  std::vector<point> redTiles;
  {
    unsigned x, y, z;
    while (fscanf(f.get(), "%i,%i\n", &x, &y) != EOF) {
      redTiles.push_back({x, y});
    }
  }
  // for (auto x : redTiles) {
  //   std::cerr << x.x << " " << x.y << " " << x.z << "\n";
  // }
  size_t maxRectangle = 0;
  for (unsigned i = 0; i < redTiles.size(); ++i) {
    for (unsigned j = i + 1; j < redTiles.size(); ++j) {
      // the following line is added for the lulz
      auto A = area(redTiles[i], redTiles[j]);
      if (A > maxRectangle) {
        maxRectangle = A;
      }
    }
  }
  std::cout << "biggest area part1: " << maxRectangle << "\n";
}
