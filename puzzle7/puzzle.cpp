#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main() {
// #define TESTING
#ifdef TESTING
  std::ifstream f("./testInput.txt");
#else
  std::ifstream f("./Input.txt");
#endif
  long splits1 = 0;
  long splits2 = 0;
  std::vector<std::string> lines;
  {
    std::string line;
    while (!getline(f, line).eof()) {

      lines.push_back(line);
    }
  }
  auto nbeam = std::count(lines[0].begin(), lines[0].end(), 'S');
  for (unsigned i = 0; i < lines.size() - 1; ++i) {
    // std::cerr << lines[i] << "\n";
    for (unsigned j = 0; j < lines[i].size(); ++j) {
      if (lines[i][j] == 'S') {
        // i+1 can't be lines.size();
        switch (lines[i + 1][j]) {
        case '.':
          // propagate
          lines[i + 1][j] = 'S';
          break;
        case '^': {
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
            if (lines[i + 1][s] == '.') {
              lines[i + 1][s] = 'S';
            }
          }
        } break;
        case 'S': // do nothing
          break;
        default:
          assert(false);
        }
      }
    }
    auto newbeams = std::count(lines[i + 1].begin(), lines[i + 1].end(), 'S');
    // splits1 += newbeams - nbeam;
    //    std::cerr << splits1 << " " << newbeams << "\n";
    nbeam = newbeams;
  }
  std::cout << "splits part1: " << splits1 << "\n";
  std::cout << "splits part2: " << splits2 << "\n";
}
