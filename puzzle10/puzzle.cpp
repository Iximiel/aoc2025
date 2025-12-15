#include <algorithm>
#include <bitset>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
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
  struct button {
    lamps behaviour = 0;
    unsigned cost = 0;
  };
  std::vector<button> buttons;

  unsigned long bfs() const {
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
        auto tmp = state.state ^ b.behaviour;
        if (tmp == target) {
          return state.depth + 1;
        }
        states.push({tmp, state.depth + 1});
      }
    }
  }
};

int main() {
// #define TESTING
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
        machine::button b;
        for (const char c : tmp) {
          b.behaviour[c - '0'] = 1;
        }
        m.buttons.push_back(b);
      }
      if (tmp[0] == '{') {
        tmp.erase(std::remove_if(tmp.begin(), tmp.end(),
                                 [](char c) { return c == '{' || c == '}'; }),
                  tmp.end());
        std::replace(tmp.begin(), tmp.end(), ',', ' ');
        std::istringstream lazy(tmp);
        for (auto &b : m.buttons) {
          lazy >> b.cost;
        }
        // assert(lazy.eof());
      }
    }
    mcs.push_back(m);
  }

#ifdef TESTING
  {
    // a few test
    lamps work;
    // use xor to toggle the lights
    work ^= mcs[1].buttons[2].behaviour;
    work ^= mcs[1].buttons[3].behaviour;
    work ^= mcs[1].buttons[4].behaviour;
    assert(mcs[1].target == work);

    work.reset();
    work ^= mcs[2].buttons[2].behaviour;
    work ^= mcs[2].buttons[1].behaviour;
    assert(mcs[2].target == work);

    work.reset();
    work ^= mcs[0].buttons[4].behaviour;
    work ^= mcs[0].buttons[5].behaviour;
    assert(mcs[0].target == work);
  }
#endif // TESTING
       // let's do a breadth first search;
  unsigned long steps = 0;
  for (const auto &m : mcs) {
    steps += m.bfs();
  }
  std::cout << "sum of steps (part1) : " << steps << "\n";
}
