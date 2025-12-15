#include <algorithm>
#include <bitset>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

struct filedeleter {
  void operator()(FILE *f) { std::fclose(f); }
};

using lamps = std::bitset<10>;

struct machine {
  lamps target = 0;
  std::array<unsigned, 10> joltage = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned fields;
  std::vector<lamps> buttons;

  unsigned long bfs_part1() const {
    struct deep {
      lamps state = 0;
      unsigned long depth = 0;
    };
    std::queue<deep> states{{}};
    states.front().state.reset();
    states.front().depth = 0;
    while (true) {
      auto state = states.front();
      states.pop();
      for (const auto &b : buttons) {
        auto tmp = state.state ^ b;
        if (tmp == target) {
          return state.depth + 1;
        }
        states.push({tmp, state.depth + 1});
      }
    }
  }

  bool joltageChek(const std::array<unsigned, 10> &j) const {
    for (unsigned i = 0; i < 10; ++i) {
      if (j[i] != joltage[i]) {
        return false;
      }
    }
    return true;
  }

  bool withinLimits(const std::array<unsigned, 10> &j) const {
    for (unsigned i = 0; i < 10; ++i) {
      if (j[i] > joltage[i]) {
        return false;
      }
    }
    return true;
  }
  struct deep2 {
    lamps state = 0;
    unsigned long depth = 0;
    std::array<unsigned, 10> joltage;
    deep2 operator^(const lamps toggle) const {
      deep2 tmp = *this;
      tmp.state ^= toggle;
      for (unsigned i = 0; i < 10; ++i) {
        if (toggle[i]) {
          tmp.joltage[i]++;
        }
      }
      return tmp;
    }
    friend std::ostream &operator<<(std::ostream &, const deep2 &);
  };
  unsigned long bfs_part2() const {
    std::queue<deep2> states{{}};
    states.front().state.reset();
    states.front().depth = 0;
    std::fill(states.front().joltage.begin(), states.front().joltage.end(), 0);
    while (true) {
      auto state = states.front();
      states.pop();
      // std::cerr << state << "\n";
      for (const auto &b : buttons) {
        auto tmp = state ^ b;
        if (tmp.state == target && joltageChek(tmp.joltage)) {
          return state.depth + 1;
        }
        if (withinLimits(tmp.joltage)) {
          tmp.depth++;
          states.push(tmp);
        }
      }
      //  std::cin.get();
    }
    assert(false);
  }

  unsigned long dfs_part2_rec(deep2 state) const {
    for (const auto &b : buttons) {
      auto tmp = state ^ b;
      if (tmp.state == target && joltageChek(tmp.joltage)) {
        return state.depth + 1;
      }
      if (withinLimits(tmp.joltage)) {
        tmp.depth++;
        auto toRet = dfs_part2_rec(tmp);
        if (toRet > 0) {
          return toRet;
        }
      }
    }
    return 0;
  }

  unsigned long dfs_part2() const {
    deep2 state;
    state.state.reset();
    state.depth = 0;
    std::fill(state.joltage.begin(), state.joltage.end(), 0);
    return dfs_part2_rec(state);
  }
};

std::ostream &operator<<(std::ostream &os, const machine::deep2 &d) {
  os << d.state << "(" << d.depth << ")";
  for (unsigned i = 0; i < 10; ++i) {
    os << " " << d.joltage[i];
  }
  return os;
}

int main() {
#define TESTING
#ifdef TESTING
  std::ifstream f("./testInput.txt");
// std::unique_ptr<FILE, filedeleter> f{std::fopen("./testInput.txt", "r")};
#else
  std::ifstream f("./Input.txt");
  // std::unique_ptr<FILE, filedeleter> f{std::fopen("./Input.txt", "r")};
#endif
  std::string line;
  std::vector<machine> mcs;
  while (std::getline(f, line)) {
    std::cerr << line << "\n";
    std::istringstream ss(line);
    machine m;
    std::string tmp;
    ss >> tmp;

    tmp.erase(std::remove_if(tmp.begin(), tmp.end(),
                             [](char c) { return c == '[' || c == ']'; }),
              tmp.end());
    for (int i = tmp.size() - 1; i >= 0; --i) {
      auto c = tmp[i];
      m.target <<= 1;
      switch (c) {
      case '.':
        break;
      case '#':
        m.target[0] = 1;
        break;
      }
    }
    while (!ss.eof()) {
      ss >> tmp;
      if (tmp[0] == '(') {
        tmp.erase(std::remove_if(
                      tmp.begin(), tmp.end(),
                      [](char c) { return c == '(' || c == ')' || c == ','; }),
                  tmp.end());
        lamps b;
        for (const char c : tmp) {
          b[c - '0'] = 1;
        }
        m.buttons.push_back(b);
      }
      if (tmp[0] == '{') {
        tmp.erase(std::remove_if(tmp.begin(), tmp.end(),
                                 [](char c) { return c == '{' || c == '}'; }),
                  tmp.end());
        m.fields = std::count(tmp.begin(), tmp.end(), ',') + 1;
        std::replace(tmp.begin(), tmp.end(), ',', ' ');
        std::istringstream lazy(tmp);
        for (unsigned i = 0; i < m.fields; ++i) {
          lazy >> m.joltage[i];
        }
        // assert(lazy.eof());
      }
    }
    std::copy(m.joltage.begin(), m.joltage.end(),
              std::ostream_iterator<unsigned>(std::cerr, " "));
    std::cerr << "\n";
    mcs.push_back(m);
  }

#ifdef TESTING
  {
    // a few test
    lamps work;
    // use xor to toggle the lights
    work ^= mcs[1].buttons[2];
    work ^= mcs[1].buttons[3];
    work ^= mcs[1].buttons[4];
    assert(mcs[1].target == work);

    work.reset();
    work ^= mcs[2].buttons[2];
    work ^= mcs[2].buttons[1];
    assert(mcs[2].target == work);

    work.reset();
    work ^= mcs[0].buttons[4];
    work ^= mcs[0].buttons[5];
    assert(mcs[0].target == work);
  }
#endif // TESTING
       // let's do a breadth first search;
  {
    unsigned long steps = 0;
    for (const auto &m : mcs) {
      steps += m.bfs_part1();
    }
    std::cout << "sum of steps (part1) : " << steps << "\n";
  }
  {
    unsigned long steps = 0;
    for (const auto &m : mcs) {
      steps += m.dfs_part2();
      std::cerr << steps << "\n";
      std::cin.get();
    }
    std::cout << "sum of steps (part2) : " << steps << "\n";
  }
}
