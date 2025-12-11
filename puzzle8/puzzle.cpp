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
  long z;
};
std::ostream &operator<<(std::ostream &os, const point &p) {
  os << p.x << " " << p.y << " " << p.z;
  return os;
}
unsigned long distance(const point &a, const point &b) {
  unsigned long x = (a.x > b.x) ? a.x - b.x : b.x - a.x;
  unsigned long y = (a.y > b.y) ? a.y - b.y : b.y - a.y;
  unsigned long z = (a.z > b.z) ? a.z - b.z : b.z - a.z;
  return x * x + y * y + z * z;
}
struct weightedCouple {
  unsigned long d;
  size_t i;
  size_t j;
  bool operator<(const weightedCouple x) const { return d < x.d; }
};

// everithing is public because I do not want to setup getters
struct Circuits {
  std::vector<unsigned long> circuits;
  Circuits(size_t s) : circuits(s) {
    std::iota(circuits.begin(), circuits.end(), 0ul);
  }
  auto findBase(unsigned long x) -> auto {
    auto root = circuits[x];
    while (root != x) {
      x = root;
      root = circuits[x];
    }
    return x;
  }
  void unite(const unsigned long i, const unsigned long j) {
    assert(i < circuits.size());
    assert(j < circuits.size());
    auto ib = findBase(i);
    auto jb = findBase(j);
    circuits[jb] = ib;
  }

private:
  unsigned long collapse_rec(unsigned i) {
    if (circuits[i] != i) {
      auto x = collapse_rec(circuits[i]);
      circuits[i] = x;
    }
    return circuits[i];
  }

public:
  void collapse() {
    for (unsigned i = 0; i < circuits.size(); ++i) {
      collapse_rec(i);
    }
  }
  auto counts() {
    std::vector<size_t> sizes;
    for (unsigned c = 0; c < circuits.size(); ++c) {
      size_t cnt = std::count(circuits.begin(), circuits.end(), c);
      if (cnt > 0) {
        sizes.push_back(cnt);
      }
    }
    std::sort(sizes.begin(), sizes.end(), std::greater<>());
    return sizes;
  }
};
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
  std::vector<point> boxes;
  {
    unsigned x, y, z;
    while (fscanf(f.get(), "%i,%i,%i\n", &x, &y, &z) != EOF) {
      boxes.push_back({x, y, z});
    }
  }
  // for (auto x : boxes) {
  //   std::cerr << x.x << " " << x.y << " " << x.z << "\n";
  // }
  std::vector<weightedCouple> couples;
  couples.reserve(connections);
  for (unsigned i = 0; i < boxes.size(); ++i) {
    for (unsigned j = i + 1; j < boxes.size(); ++j) {
      // the following line is added for the lulz
      assert(distance(boxes[i], boxes[j]) == distance(boxes[j], boxes[i]));
      weightedCouple x{distance(boxes[i], boxes[j]), i, j};
      if (couples.size() < connections) {
        couples.push_back(x);
        if (couples.size() == connections) {
          std::make_heap(couples.begin(), couples.end());
        }
      } else {
        if (x.d < couples[0].d) {
          // we pop away the highest distance and we add
          // the new couple
          std::pop_heap(couples.begin(), couples.end());
          // the highes value is now the last element,
          // we substitute that and
          // we push the new element in the heap
          couples.back() = x;
          std::push_heap(couples.begin(), couples.end());
        }
      }
    }
  }
  std::sort_heap(couples.begin(), couples.end());
  // linking phase: creating some graphs and merging them
  //  de facto the couples list is a list of linkages between two nodes
  // each box belong to its own circuit
  Circuits circuits(boxes.size());
  // setting up a set for not tranversing everithing
  for (auto [w, i, j] : couples) {
    circuits.unite(i, j);
    // we fold on the smallest or on the already existing circuit
  }
  circuits.collapse();
  std::vector<size_t> sizes = circuits.counts();
  uint64_t mul3 = sizes[0] * sizes[1] * sizes[2];
  std::cout << "the three circuits part1: " << mul3 << "\n";
  //  std::cout << "timelines part2: " << splits2 << "\n";
}
