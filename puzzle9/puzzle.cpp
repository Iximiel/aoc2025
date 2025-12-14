#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <limits>
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
        y = h;
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
    unsigned n = 1000;
    long step;
  } settings;
  std::vector<point> vertexes;
  std::vector<edge> edges;
  std::vector<V2> axes;
  std::vector<V2> lazyProj;
  void calcAxes() {
    // in this particular case everything i either vertical or
    // horizontal
    axes.resize(edges.size());
    lazyProj.resize(edges.size());
    for (unsigned i = 0; i < edges.size() - 1; ++i) {
      // the axes is perpendicular to the edge
      axes[i] = {-edges[i].y, edges[i].x};
      // since I will be passing on this A LOT, and for myself is N^2, let's
      // avoid the calcualtion!
      lazyProj[i] = projectOnAxis(axes[i]);
    }
  }

  V2 projectOnAxis(const V2 &ax) const {
    long min = vertexes[0].dot(ax);
    long max = min;
    for (unsigned i = 1; i < vertexes.size() - 1; ++i) {
      long tmp = vertexes[i].dot(ax);
      if (tmp < min) {
        min = tmp;
      } else if (tmp > max) {
        max = tmp;
      }
    }
    return {min, max};
  }
  inline static bool projectionOverlaps(V2 a, V2 b) {
    return !(a.x < b.y || b.x < a.y);
  }

public:
  polygon(const std::vector<point> &myvertexes, long lowy, long maxy)
      : vertexes(myvertexes) {
    // only works for the given polygon
    assert(lowy == 0);
    auto delta = (maxy - lowy);
    if (delta < settings.n) {
      settings.n = delta;
    }
    settings.step = delta / settings.n;
    while (settings.step * settings.n <= maxy) {
      settings.n++;
    }
    bands.resize(settings.n);
    for (unsigned i = 0; i < vertexes.size() - 1; ++i) {
      edges.emplace_back(vertexes[i], vertexes[i + 1]);
      assert(maxy >= edges.back().y);
      if (edges.back().vertical) {
        // vertical:
        const size_t to = edges.back().h / settings.step;
        if (to >= settings.n) {
          std::cerr << "#" << settings.n * settings.step << "\n";
          std::cerr << "*" << to << " " << edges.back().h << " "
                    << settings.step << " " << maxy << "\n";
        }
        assert(to < settings.n);
        for (size_t b = edges.back().l / settings.step; b <= to; ++b) {
          bands[b].push_back(i);
        }
      } else {
        // horizontal:
        size_t b = edges.back().y / settings.step;
        assert(b < settings.n);
        bands[b].push_back(i);
      }
    }
    // rosetta uses an elegant modulo, I do not do an extra division per loop
    edges.emplace_back(vertexes.back(), vertexes[0]);
    {
      unsigned i = edges.size() - 1;
      if (edges.back().vertical) {
        // vertical:
        const size_t to = edges.back().h / settings.step;
        if (to >= settings.n) {
          std::cerr << "#" << settings.n * settings.step << "\n";
          std::cerr << "*" << to << " " << edges.back().h << " "
                    << settings.step << " " << maxy << "\n";
        }
        assert(to < settings.n);
        for (size_t b = edges.back().l / settings.step; b <= to; ++b) {
          bands[b].push_back(i);
        }
      } else {
        // horizontal:
        size_t b = edges.back().y / settings.step;
        assert(b < settings.n);
        bands[b].push_back(i);
      }
    }
    calcAxes();
  }

  polygon(const rectangle &rect)
      : polygon(rect.vertexes(), rect.low.y, rect.high.y) {}

  bool overlaps(const polygon &other) const {

    // loop on these axes
    for (unsigned i = 0; i < axes.size(); ++i) {
      auto otherPrj = other.projectOnAxis(axes[i]);
      if (!projectionOverlaps(otherPrj, lazyProj[i])) {
        return false;
      }
    }
    // loop on the other axes
    for (unsigned i = 0; i < other.axes.size(); ++i) {
      auto otherPrj = projectOnAxis(other.axes[i]);
      if (!projectionOverlaps(otherPrj, other.lazyProj[i])) {
        return false;
      }
    }
    return true;
  }

  bool PIPCheck(const point &a) const {
    const auto x = a.x;
    const auto y = a.y;
    // this assumess that perimeter[0] == perimeter.back()
    // we are casting a ray from  point to the right
    const auto band = y / settings.step;
    if (band >= bands.size()) {
      return false;
    }
    unsigned intersections = 0;

    for (unsigned b = (band > 0) ? (band - 1) : 0; b < band + 1; ++b) {
      if (b >= bands.size()) {
        continue;
      }
      for (const auto &i : bands[b]) {
        const auto &e = edges[i];
        if (e.vertical) {
          // check if the ray passes trought the edge
          if ((e.l <= y && y <= e.h) && (x <= e.x)) {
            // the ray is from left to right
            intersections++;
            if (e.x == x) { // is on the perimeter
              return true;
            }
          }
        } else if ((e.y == y) && (e.l <= x && x <= e.h)) {
          // is on the perimeter
          return true;
        }
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

// try to create a struct with the perimeter as eges with zylims and
// direction tow diminuish the number of edge calculations

bool PIPCheck(const point &a, const std::vector<point> &perimeter) {
  const auto x = a.x;
  const auto y = a.y;
  // this assumess that perimeter[0] == perimeter.back()
  // we are casting a ray from  point to the right
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
  return intersections % 2 != 0;
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
  // return 0;
  // looking for points in the rectangle encased by the whole polynomial that
  // are ouside the polygon
  /*
  std::vector<point> excluded;
    settings.n = maxy / settings.step;
  {
      std::vector<std::string> rows(h.y + 1);
      for (auto &r : rows) {
        r.assign(h.x + 1, '.');
      }*/
  /*  for (long y = 0; y <= h.y; ++y) {
      for (long x = 0; x <= h.x; ++x) {
        if (!pl.PIPCheck({x, y})) {
          excluded.push_back({x, y});
          //        rows[y][x] = 'x';
        }
      }
    }*/
  /*
      for (auto &r : redTiles) {
        if (rows[r.y][r.x] == 'x') {
          rows[r.y][r.x] = 'H';
        }
        rows[r.y][r.x] = '#';
      }
      for (const auto &r : rows) {
        std::cout << r << std::endl;
      }
    }*/
  unsigned long d = std::abs((redTiles[0] - redTiles.back()).x) +
                    std::abs((redTiles[0] - redTiles.back()).y);
  for (unsigned i = 1; i < redTiles.size(); ++i) {
    auto t = redTiles[i] - redTiles[i - 1];
    d += std::abs(t.x) + std::abs(t.y);
  }
  std::cout << d << "\n";

  size_t maxRectangleEncased = 0;

  polygon tiles{redTiles, l.y, h.y};
  for (unsigned i = 0; i < redTiles.size(); ++i) {
    for (unsigned j = i + 1; j < redTiles.size(); ++j) {
      // the following line is added for the lulz
      auto A = area(redTiles[i], redTiles[j]);
      rectangle R = {redTiles[i], redTiles[j]};

      bool isInside = true;
      isInside = tiles.PIPCheck(R);
      /*for (unsigned rt = 0; rt < redTiles.size(); ++rt) {
        if (rt == i || rt == j) {
          continue;
        }
        isInside = !R.insideStrict(redTiles[rt]);
        if (!isInside) {
          // if another vertex is inside part of the rectangle is ouside
          break;
        }
      }
      // slower check to verify

      polygon RP = R.inner();
      isInside = RP.overlaps(tiles);*/
      //      std::cerr << std::boolalpha << i << " " << j << " " << isInside
      //      <<
      //      "\n";

      if (isInside) {
        maxRectangleEncased =
            (A > maxRectangleEncased) ? A : maxRectangleEncased;
      }
    }
  }
  std::cout << "biggest area inside part2: " << maxRectangleEncased << "\n";
}
