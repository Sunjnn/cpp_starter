#include "mylib.hpp"

#include <iostream>

using std::cout;

using mylib::greet;

int main() {
  cout << greet("World") << '\n';
  return 0;
}
