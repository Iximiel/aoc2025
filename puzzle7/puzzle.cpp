#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
// #define TESTING
#ifdef TESTING
  std::ifstream f("./testInput.txt");
  constexpr unsigned lineL = 15;
#else
  constexpr unsigned lineL = 141;
  std::ifstream f("./Input.txt");
#endif
  long splits1 = 0;
  long splits2 = 0;
  std::vector<std::string> lines;
  std::vector<std::array<long, lineL>> paths;
  {
    std::string line;
    while (!getline(f, line).eof()) {
      lines.push_back(line);
      paths.emplace_back();
      std::fill(paths.back().begin(), paths.back().end(), 0);
      assert(line.size() == lineL);
    }
  }
  // this is lazy, I know
  std::fill(paths[0].begin(), paths[0].end(), 1);

  for (unsigned i = 0; i < lines.size() - 1; ++i) {
    for (unsigned j = 0; j < lines[i].size(); ++j) {
      auto timelines = paths[i][j];
      if (lines[i][j] == 'S') {
        // i+1 can't be lines.size();
        switch (lines[i + 1][j]) {
        case '.':
        //[[fallthrough]]
        // this is what changes in part2: you want to "ovewrite"
        case 'S':
          // propagate
          lines[i + 1][j] = 'S';
          paths[i + 1][j] += timelines;
          break;
        case '^': {
          paths[i + 1][j] = -1;
          splits1++;
          // split
          std::vector<unsigned> next;
          if (j > 0) {
            next.push_back(j - 1);
          }
          if ((j + 1) < lines.size()) {
            next.push_back(j + 1);
          }
          for (const auto s : next) {
            lines[i + 1][s] = 'S';
            paths[i + 1][s] += timelines;
          }
        } break;
        default:
          assert(false);
        }
      }
    }
  }
  splits2 = std::accumulate(paths.back().begin(), paths.back().end(), 0);
  std::cout << "splits part1: " << splits1 << "\n";
  std::cout << "splits part2: " << splits2 << "\n";
}
