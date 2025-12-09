#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

struct range {
  long min;
  long max;
  bool contains(const long num) const { return min <= num && num <= max; }
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
    } else {
      ingredients.emplace_back(std::stol(line));
    }
  }
  for (const auto i : ingredients) {
    for (const auto &r : database) {
      if (r.contains(i)) {
        ++fresh1;
        break;
      }
    }
  }

  // sort database first
  std::sort(database.begin(), database.end(),
            [](range x, range y) { return x.min < y.min; });
  // but i lazily keep the n^2 in time collapsing algo

  for (unsigned i = 0; i < database.size(); ++i) {
    if (database[i].min == 0 && database[i].max == 0) {
      continue;
    }
    // todo do the reverse
    for (unsigned j = 0; j < database.size(); ++j) {
      if (i == j) {
        continue;
      }
      if (database[j].min == 0 && database[j].max == 0) {
        continue;
      }
      if (database[i].contains(database[j].min)) {
        if (!database[i].contains(database[j].max)) {
          //+++[      ]++++++
          //+++++++[    ]++++
          database[i].max = database[j].max;
          // everithing is contains
          // nothing to do
        } else { // we join the entry
          //+++[      ]++++++
          //++++++[ ]++++++++
        }
        database[j].min = database[j].max = 0;
      } else if (database[i].contains(database[j].max)) {
        //  +++[      ]++++++
        //  +[    ]++++++++++
        database[i].min = database[j].min;
        database[j].min = database[j].max = 0;
        // the completely whitin case is above
      }
    }
  }
  for (const auto r : database) {
    if (r.max != 0 && r.min != 0) {
      std::cerr << r.min << " " << r.max << " " << r.max - r.min + 1 << "\n";
      // its a closed interval
      fresh2 += r.max - r.min + 1;
    }
  }
  std::cout << "fresh part1: " << fresh1 << "\n";
  std::cout << "fresh part2: " << fresh2 << "\n";
}
