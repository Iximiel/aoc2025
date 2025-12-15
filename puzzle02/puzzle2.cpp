#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>

struct filedeleter {
  void operator()(FILE *f) { std::fclose(f); }
};

int main() {
  std::unique_ptr<FILE, filedeleter> f{std::fopen("./Input.txt", "r")};
  assert((f.get(), "error in opening file"));
  char start[251];
  char end[251];
  unsigned long pw1 = 0;
  unsigned long pw2 = 0;
  const auto regPart1 = std::regex(R"=((.*)\1)=");
  const auto regPart2 = std::regex(R"=((.*)\1+)=");
  std::smatch match;
  while (fscanf(f.get(), "%250[0-9]-%250[0-9]\n", start, end) != EOF) {
    // some of the numbers are > max of unsignde int!!!!
    unsigned long startN = std::stoul(std::string(start));
    unsigned long endN = std::stoul(std::string(end));
    for (auto i = startN; i <= endN; ++i) {
      auto s = (std::ostringstream() << i).str();
      if (std::regex_match(s, match, regPart1)) {
        pw1 += i;
      }
      if (std::regex_match(s, match, regPart2)) {
        pw2 += i;
      }
    }
  }

  std::cout << "code part1: " << pw1 << "\n";
  std::cout << "password part2: " << pw2 << "\n";
}
