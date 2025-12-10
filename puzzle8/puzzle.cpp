#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

// todo: map the distances (in a heap?) the input i the usual > 5KiB
// connect the fuses
// remember use the squared distance, since squares and their (positive) roots
// are in the same order check

int main() {
// #define TESTING
#ifdef TESTING
  std::ifstream f("./testInput.txt");
  constexpr unsigned lineL = 15;
#else
  constexpr unsigned lineL = 141;
  std::ifstream f("./Input.txt");
#endif
  std::vector<std::string> lines;
  std::vector<std::array<unsigned long long, lineL>> paths;
  {
    std::string line;
    while (!getline(f, line).eof()) {
      lines.push_back(line);
      paths.emplace_back();
      std::fill(paths.back().begin(), paths.back().end(), 0);
      assert(line.size() == lineL);
    }
  }
  uint64_t mul3 = 0;
  std::cout << "the three circuits part1: " << mul3 << "\n";
  //  std::cout << "timelines part2: " << splits2 << "\n";
}
