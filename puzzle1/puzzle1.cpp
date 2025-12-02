#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>

struct filedeleter {
  void operator()(FILE *f) { std::fclose(f); }
};

int main() {
  constexpr int dialmax = 99 + 1;
  int dialval = 50;
  std::unique_ptr<FILE, filedeleter> f{std::fopen("./Input.txt", "r")};
  assert((f.get(), "error in opening file"));
  char dir;
  int step;
  long pw = 0;
  // I have never used fscanf before
  while (fscanf(f.get(), "%c%i\n", &dir, &step) != EOF) {
    switch (dir) {
    case 'L':
      dialval -= step;
      break;
    case 'R':
      dialval += step;
    }
    if (dialval >= dialmax) {
      dialval %= dialmax;
    } else {
      // this is the evil part of the puzzle
      while (dialval < 0) {
        dialval += dialmax;
      }
    }
    if (dialval == 0) {
      ++pw;
    }
  }
  std::cout << "password: " << pw << "\n";
}
