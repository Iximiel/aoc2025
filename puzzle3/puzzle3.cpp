#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>

unsigned long num(std::array<int, 12> nn) {
  long m = 10;
  unsigned long r = nn[11];
  for (int i = 10; i >= 0; --i) {
    r += nn[i] * m;
    m *= 10;
  }
  return r;
}

int main() {
  // std::ifstream f("./testInput.txt");
  std::ifstream f("./Input.txt");
  long jolts1 = 0;
  long jolts2 = 0;
  std::string line;
  long sum1 = 0;
  long sum2 = 0;
  std::array<int, 12> digits;
  while (std::getline(f, line)) {
    if (line.size() == 0) {
      continue;
    }
    sum2 = 0;
    sum1 = 0;
    char max = line.back();
    for (int i = 11, j = line.size() - 1; i >= 0; --i, --j) {
      if (long tmp = (line[j] - '0') * 10 + (max - '0');
          tmp > sum1 && i != 11) {
        sum1 = tmp;
      }
      if (line[j] > max) {
        max = line[j];
      }
      digits[i] = line[j] - '0';
    }

    if (line.size() > 12) {
      // signed becasue I do not want to wrap around in an infinite loop
      for (long i = line.size() - 13; i >= 0; --i) {
        // std::cerr << "*" << i << " " << line[i] << "\n";
        // here integer promotion happens
        if (long tmp = (line[i] - '0') * 10 + (max - '0'); tmp > sum1) {
          sum1 = tmp;
        }
        if (line[i] > max) {
          max = line[i];
        }
        int thedigit = line[i] - '0';
        for (unsigned d = 0; d < 12; ++d) {
          if (thedigit >= digits[d]) {
            std::swap(thedigit, digits[d]);
          } else {
            break;
          }
        }
      }
    }
    jolts1 += sum1;
    // std::cerr << num(digits) << "\n";
    jolts2 += num(digits);
  }
  std::cout << "jolts part1: " << jolts1 << "\n";
  std::cout << "jolts part2: " << jolts2 << "\n";
}
