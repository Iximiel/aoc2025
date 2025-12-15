#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

template <unsigned l> struct problem {
  std::array<unsigned long, l> nums;
  char op;
  unsigned long res() const {
    unsigned long r = nums[0];
    for (auto i = 1u; i < l; ++i) {
      switch (op) {
      case '+':
        r += nums[i];
        break;
      case '*':
        r *= nums[i];
        break;
      default:
        assert(false);
      }
    }
    return r;
  }
};

struct problem2 {
  std::vector<std::string> nums;
  char op;
  unsigned long res() const {
    unsigned long r = std::stoul(nums[0]);
    for (auto i = 1u; i < nums.size(); ++i) {
      switch (op) {
      case '+':
        r += stoul(nums[i]);
        break;
      case '*':
        r *= stoul(nums[i]);
        break;
      default:
        assert(false);
      }
    }
    return r;
  }
};

int main() {
// #define TESTING
#ifdef TESTING
  std::ifstream f("./testInput.txt");
  constexpr int lines = 3;
#else
  std::ifstream f("./Input.txt");
  constexpr int lines = 4;
#endif
  unsigned NProbs = 0;
  {
    long solution1 = 0;
    {
      std::string line;
      std::getline(f, line);
      std::istringstream ss(line);
      while (!ss.eof()) {
        std::string s;
        ss >> s;
        if (s.size() > 0) {
          ++NProbs;
        }
      }
    }
    f.seekg(0);
    std::vector<problem<lines>> prb(NProbs);
    for (int l = 0; l < lines; ++l) {
      for (unsigned n = 0; n < NProbs; ++n) {
        f >> prb[n].nums[l];
      }
    }
    for (auto &pr : prb) {
      f >> pr.op;
      solution1 += pr.res();
    }

    std::cout << "solution part1: " << solution1 << "\n";
  }
  {
    f.seekg(0);
    std::vector<problem2> prb(NProbs);
    // the -1 is to simplify the logic in the counter and automagically skip the
    // extra space
    std::vector<int> lengths(NProbs, -1);
    // no space to separate the latest new line
    lengths.back()++;
    std::string line;
    for (auto i = 0u; i <= lines; ++i) {
      std::getline(f, line);
    }

    long n = -1;
    for (const auto c : line) {
      if (c == '+' || c == '*') {
        ++n;
        prb[n].op = c;
      }
      assert(n >= 0);
      lengths[n]++;
    }

    f.seekg(0);

    for (auto l = 0u; l < lines; ++l) {
      // auto pw = fastpow(10, (lines - l - 1));
      // std::cerr << l << " " << pw << "\n";
      std::getline(f, line);
      std::string::size_type start = 0;
      for (auto i = 0u; i < prb.size(); ++i) {
        auto data = line.substr(start, lengths[i] + 1);
        start += lengths[i] + 1;
        if (l == 0) {
          prb[i].nums.resize(lengths[i]);
        }
        // data.size() has also the extra space that separates the two entries
        for (auto n = 0u; n < data.size(); ++n) {
          if (data[n] != ' ' && n < prb[i].nums.size()) {
            prb[i].nums[n] += data[n];
          }
        }
      }
    }
    long solution2 = 0;
    for (const auto &pr : prb) {
      solution2 += pr.res();
    }
    std::cout << "solution part2: " << solution2 << "\n";
  }
}
