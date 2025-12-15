#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <ostream>
#include <unordered_set>
#include <vector>
// todo: map the distances (in a heap?) the input i the usual > 5KiB
// connect the fuses
// remember use the squared distance, since squares and their (positive) roots
// are in the same order check

struct filedeleter {
  void operator()(FILE *f) { std::fclose(f); }
};

struct V2 {
  long x;
  long y;
  V2 operator-(const V2 &other) const { return {x - other.x, y - other.y}; }
  V2 &operator-=(const V2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  long dot(const V2 &other) const { return x * other.x + y * other.y; }
};
using point = V2;

std::ostream &operator<<(std::ostream &os, const point &p) {
  os << p.x << " " << p.y;
  return os;
}
class rectangle {
  point low;
  point high;

public:
  rectangle(const point &a, const point &b)
      : low{std::min(a.x, b.x), std::min(a.y, b.y)},
        high{std::max(a.x, b.x), std::max(a.y, b.y)} {}
  rectangle inner() const {
    return {{low.x + 1, low.y + 1}, {high.x - 1, high.y - 1}};
  }
  bool inside(const point &a) {
    // the interval is closed!!!
    return (low.x <= a.x && a.x <= high.x) && (low.y <= a.y && a.y <= high.y);
  }
  bool insideStrict(const point &a) {
    // the interval is closed!!!
    return (low.x < a.x && a.x < high.x) && (low.y < a.y && a.y < high.y);
  }
  auto vertexes() const {
    return std::vector<point>{{low.x, high.y}, high, {high.x, low.y}, low};
  }
  friend class polygon;
};

class polygon {
  struct edge {
    long x{-1};
    long y{-1};
    long l;
    long h;
    V2 d{0, 0};
    bool vertical;
    edge(const point &a, const point &b) : d{a - b} {
      if (d.x == 0) {
        vertical = true;
        x = a.x;
        l = std::min(a.y, b.y);
        h = std::max(a.y, b.y);
      } else {
        assert(a.y == b.y);
        vertical = false;
        y = a.y;
        l = std::min(a.x, b.x);
        h = std::max(a.x, b.x);
      }
    }
  };
  std::vector<std::vector<size_t>> bands;
  struct bandStructure {
    unsigned n;
    long step;
  } settings;
  unsigned maxy;
  unsigned miny;
  std::vector<edge> edges;

  // to be used in the ctor
  void updateBands(const size_t i, const edge &e) {
    // std::cerr << i << ((e.vertical) ? "V " : "H ");

    if (e.vertical) {
      // std::cerr << e.l << " -> " << e.h << " (";
      const size_t from = e.l / settings.step;
      // vertical:
      const size_t to = e.h / settings.step;
      // std::cerr << from << " -> " << to << ")\n";
      if (to >= settings.n) {
        std::cerr << "#" << settings.n * settings.step << "\n";
        std::cerr << "*" << to << " " << e.h << " " << settings.step << " "
                  << maxy << "\n";
      }
      assert(to < settings.n);
      for (size_t b = from; b <= to; ++b) {
        bands[b].push_back(i);
      }
    } else {
      // horizontal:
      // std::cerr << e.y << " (";
      size_t b = e.y / settings.step;
      // std::cerr << b << ")\n";
      assert(b < settings.n);
      bands[b].push_back(i);
    }
  }

public:
  polygon(const std::vector<point> &vertexes, long lowy, long highy)
      : maxy(highy), miny(lowy) {
    // only works for the given polygon
    assert(miny == 0);
    auto delta = (maxy - miny);
    settings.n = 4000;
    if (delta < settings.n) {
      // we are in the test setting
      settings.n = 4;
    }
    settings.step = delta / settings.n;
    while (settings.step * settings.n <= maxy) {
      settings.n++;
    }
    std::cout << settings.n << ": " << settings.step;
    std::cout << ", delta " << delta << "\n";
    bands.resize(settings.n);
    for (unsigned i = 0; i < vertexes.size() - 1; ++i) {
      edges.emplace_back(vertexes[i], vertexes[i + 1]);
      // assert(maxy >= edges.back().y);
      //  assert(miny <= edges.back().y);
      updateBands(edges.size() - 1, edges.back());
    }
    // rosetta uses an elegant modulo, I do not do an extra division per loop
    edges.emplace_back(vertexes.back(), vertexes[0]);
    updateBands(edges.size() - 1, edges.back());
  }

  polygon(const rectangle &rect)
      : polygon(rect.vertexes(), rect.low.y, rect.high.y) {}

  bool PIPCheck(const point &a) const {
    const auto x = a.x;
    const auto y = a.y;
    // we are casting a ray from  point to the right
    const auto band = y / settings.step;
    if (band >= bands.size()) {
      return false;
    }
    unsigned intersections = 0;
    //    std::cout << y << " " << band << "\n";

    for (const auto &i : bands[band]) {
      const auto &e = edges[i];
      if (e.vertical) {
        // check if the ray passes trought the edge
        if ((e.l <= y && y <= e.h) && (x <= e.x)) {
          // the ray is from left to right
          if (e.x == x) {
            // is on the perimeter
            return true;
          }
          intersections++;
        }
      } else if ((e.y == y) && (e.l <= x && x <= e.h)) {
        // is on the perimeter
        return true;
      }
    }

    return intersections % 2 != 0;
  }

  bool PIPCheck(const rectangle &r) const {
    bool inside = true;
    for (long x = r.low.x; inside && x <= r.high.x; ++x) {
      for (long y = r.low.y; inside && y <= r.high.y; ++y) {
        inside = PIPCheck({x, y});
      }
    }
    return inside;
  }
};

unsigned long area(const point &a, const point &b) {
  // the perimeter is in the area
  unsigned long x = 1 + ((a.x > b.x) ? a.x - b.x : b.x - a.x);
  unsigned long y = 1 + ((a.y > b.y) ? a.y - b.y : b.y - a.y);
  return x * y;
}

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
  point l{std::numeric_limits<long>::max(), std::numeric_limits<long>::max()};
  point h{0, 0};
  {
    unsigned x, y, z;
    while (fscanf(f.get(), "%i,%i\n", &x, &y) != EOF) {
      redTiles.push_back({x, y});
      if (x < l.x) {
        l.x = x;
      } else if (x > h.x) {
        h.x = x;
      }
      if (y < l.y) {
        l.y = y;
      } else if (y > h.y) {
        h.y = y;
      }
    }
  }
  // translating everithing around 0,0
  for (auto &rt : redTiles) {
    rt -= l;
  }
  std::cout << redTiles.size() << " (" << h << ") (" << l << ")" << std::endl;
  h -= l;
  l -= l;
  std::cout << redTiles.size() << " (" << h << ") (" << l << ")" << std::endl;
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
  std::cout << "biggest area part1: " << maxRectangle << std::endl;

  size_t maxRectangleEncased = 0;

  polygon tiles{redTiles, l.y, h.y};
  for (unsigned i = 0; i < redTiles.size(); ++i) {
    for (unsigned j = i + 1; j < redTiles.size(); ++j) {
      // the following line is added for the lulz
      auto A = area(redTiles[i], redTiles[j]);
      rectangle R = {redTiles[i], redTiles[j]};

      if (tiles.PIPCheck(R)) {
        maxRectangleEncased =
            (A > maxRectangleEncased) ? A : maxRectangleEncased;
      }
    }
  }
  std::cout << "biggest area inside part2: " << maxRectangleEncased << "\n";
}
