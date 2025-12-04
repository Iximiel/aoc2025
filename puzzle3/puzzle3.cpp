#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
int main() {
  // std::ifstream f("./Input.txt");
  std::ifstream f("./testInput.txt");
  long pw1 = 0;
  long pw2 = 0;
  std::string line;
  long sum1 = 0;
  while (std::getline(f, line)) {
    if (line.size() == 0) {
      continue;
    }
    sum1 = 0;
    char max = line.back();
    // signed becasue I do not want to wrap around in an infinite loop
    for (long i = line.size() - 2; i >= 0; --i) {
      // here integer promotion happens
      long tmp = (line[i] - '0') * 10 + (max - '0');
      if (tmp > sum1) {
        sum1 = tmp;
      }
      if (line[i] > max) {
        max = line[i];
      }
    }

    pw1 += sum1;
  }
  // while (fscanf(f.get(), "%c%i\n", &dir, &step) != EOF){}
  std::cout << "password part1: " << pw1 << "\n";
  std::cout << "password part2: " << pw2 << "\n";
}
