#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
struct range {
  long min;
  long max;
  bool between(const long num) const { return min <= num && num <= max; }
};
int main() {
  // std::ifstream f("./testInput.txt");
  std::ifstream f("./Input.txt");
  std::string line;
  long fresh1 = 0;
  long fresh2 = 0;
  // I have to reverse it then
  bool databaseReading = true;
  std::vector<range> database;
  std::vector<long> ingredients;
  while (std::getline(f, line)) {
    if (line.size() == 0) {
      databaseReading = false;
      continue;
    }
    if (databaseReading) {
      auto pivot = line.find('-');
      database.emplace_back(range{std::stol(line.substr(0, pivot)),
                                  std::stol(line.substr(pivot + 1))});
      // std::cout << line.substr(0, pivot) << " , " << line.substr(pivot + 1)
      // << "\n";
    } else {
      ingredients.emplace_back(std::stol(line));
    }
  }
  for (const auto i : ingredients) {
    for (const auto &r : database) {
      if (r.between(i)) {
        // std::cout << i << " " << r.min << " " << r.max << "\n";
        ++fresh1;
        break;
      }
    }
  }
  std::cout << "fresh part1: " << fresh1 << "\n";
  std::cout << "fresh part2: " << fresh2 << "\n";
}
