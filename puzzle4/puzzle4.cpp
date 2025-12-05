#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>

int main() {
  // std::ifstream f("./testInput.txt");
  // constexpr unsigned rowSize = 10;
  std::ifstream f("./Input.txt");
  constexpr unsigned rowSize = 136;
  std::string line;
  std::array<int, 12> digits;
  std::vector<std::bitset<rowSize>> rows;
  // I have to reverse it then
  while (std::getline(f, line)) {
    if (line.size() == 0) {
      continue;
    }
    assert(line.size() == rowSize); //"row size is not correct"
    rows.emplace_back();
    for (int i = 0; i < rowSize; ++i) {
      rows.back()[i] = line[i] == '@';
    }
  }
  std::vector<std::bitset<rowSize>> toRemove = rows;
  for (unsigned r = 0; r < rows.size(); ++r) {
    // using set to because I want to remove bits with &+~, and so I can skip
    // the negate(~) thing
    toRemove[r].set();
  }
  // std::cout << "\n";
  unsigned rolls = 0;

  long rolls1 = 0;
  long rolls2 = 0;
  do {
    rolls = 0;
    for (unsigned r = 0; r < rows.size(); ++r) {
      for (unsigned c = 0; c < rowSize; ++c) {
        if (!rows[r][c]) {
          // //std::cout << 0;
          continue;
        }
        // ternary becasue I am lazy and I do not want to deal with with what
        // happen to a "negative" unsigned
        const auto cmin = (c != 0) ? c - 1 : 0;
        const auto cmax = std::min(c + 2, rowSize);
        int sum = 0;
        if (r > 0) { // above
          auto ra = r - 1;
          for (auto cc = cmin; cc < cmax; ++cc) {
            sum += rows[ra][cc];
          }
        }
        // around
        for (auto cc = cmin; cc < cmax; ++cc) {
          if (cc != c) {
            sum += rows[r][cc];
          }
        }
        if (r != rows.size() - 1) { // below
          auto rb = r + 1;
          for (auto cc = cmin; cc < cmax; ++cc) {
            sum += rows[rb][cc];
          }
        }
        if (sum < 4) {
          rolls++;
          toRemove[r][c] = false;
        }
      }
    }
    for (unsigned r = 0; r < rows.size(); ++r) {
      // no need to reset toRemove each time;
      rows[r] &= toRemove[r];
    }

    if (rolls1 == 0) {
      rolls1 = rolls;
    }
    rolls2 += rolls;
  } while (rolls != 0);
  std::cout << "rolls part1: " << rolls1 << "\n";
  std::cout << "rolls part2: " << rolls2 << "\n";
}
