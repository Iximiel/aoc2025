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
  point operator-(const point &other) const {
    return {x - other.x, y - other.y};
  }
};
std::ostream &operator<<(std::ostream &os, const point &p) {
  os << p.x << " " << p.y;
  return os;
}

unsigned long area(const point &a, const point &b) {
  // the perimeter is in the area
  unsigned long x = 1 + ((a.x > b.x) ? a.x - b.x : b.x - a.x);
  unsigned long y = 1 + ((a.y > b.y) ? a.y - b.y : b.y - a.y);
  return x * y;
}

bool PIPCheck(const point &a, const point &b,
              const std::vector<point> &perimeter) {
  // this assumess that perimeter[0] == perimeter.back()
  // we are casting a ray from each point to the right
  point up = {std::max(a.x, b.x), std::max(a.y, b.y)};
  point low = {std::min(a.x, b.x), std::min(a.y, b.y)};
  for (long x = low.x; x <= up.x; ++x) {
    for (long y = low.y; y <= up.y; ++y) {
      auto prev = perimeter[0];
      unsigned intersections = 0;
      for (unsigned i = 1; i < perimeter.size(); ++i) {
        auto edge = perimeter[i] - prev;
        // assert(edge.x == 0 || edge.y == 0);
        if (edge.y == 0 && y == perimeter[i].y) {
          // the edge is horizontal, the point is inside if it is on the edge
          if ((edge.x > 0) ? (prev.x < x && x < perimeter[i].x)
                           : (prev.x > x && x > perimeter[i].x)) {
            intersections = 1;
            break;
          }
        } else {
          // the edge is vertical
          if (x < perimeter[i].x) {
            if ((edge.y > 0) ? (prev.y < y && y < perimeter[i].y)
                             : (prev.y > y && y > perimeter[i].y)) {
              if (x == perimeter[i].x) { // the point is on the edge
                intersections = 1;
                break;
              }
              ++intersections;
            }
          }
        }
        prev = perimeter[i];
      }
      if (intersections % 2 == 0) {
        return false;
      }
    }
  }
  return true;
}
//  everithing is public because I do not want to setup getters
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

  size_t maxRectangleEncased = 0;
  // needed for the PIP (point in poligon) algorithm
  redTiles.push_back(redTiles[0]);
  for (unsigned i = 0; i < redTiles.size() - 1; ++i) {
    for (unsigned j = i + 1; j < redTiles.size() - 1; ++j) {
      // the following line is added for the lulz
      auto A = area(redTiles[i], redTiles[j]);
      auto isInside = PIPCheck(redTiles[i], redTiles[j], redTiles);
      //     std::cerr << std::boolalpha << i << " " << j << " " << isInside <<
      //     "\n";
      if (A > maxRectangleEncased && isInside) {
        maxRectangleEncased = A;
      }
    }
  }
  std::cout << "biggest area iside part2: " << maxRectangleEncased << "\n";
}
