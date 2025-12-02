#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>

struct filedeleter {
  void operator()(FILE *f) { std::fclose(f); }
};
struct divtie {
  int &quot;
  int &rem;
  divtie(int &q, int &r) : quot(q), rem(r) {}
  divtie &operator=(const std::div_t &d) {
    quot = d.quot;
    rem = d.rem;
    return *this;
    // using
    //```
    // auto x=div()
    // q=x.quot
    // r=x.rem
    //```
    // would have taken much less effort xD
  }
};

int main() {
  constexpr int dialmax = 99 + 1;
  int dialval = 50;
  std::unique_ptr<FILE, filedeleter> f{std::fopen("./Input.txt", "r")};
  assert((f.get(), "error in opening file"));
  char dir;
  int step;
  long pw1 = 0;
  long pw2 = 0;
  // I have never used fscanf before
  while (fscanf(f.get(), "%c%i\n", &dir, &step) != EOF) {
    int clicks = 0;
    switch (dir) {
    case 'L':
      dialval -= step;
      break;
    case 'R':
      dialval += step;
    }

    while (dialval > dialmax) {
      ++clicks;
      dialval -= dialmax;
    }
    // this is evil
    if (dialval < 0 && (dialval + step) == 0) {
      clicks--;
    }
    while (dialval < 0) {
      dialval += dialmax;
      ++clicks;
    }

    if (dialval == 0 || dialval == dialmax) {
      dialval = 0;
      ++clicks;
      ++pw1;
    }
    pw2 += clicks;
  }
  std::cout << "password part1: " << pw1 << "\n";
  std::cout << "password part2: " << pw2 << "\n";
}
