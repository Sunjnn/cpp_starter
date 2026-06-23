#include <iostream>

#include "mylib.hpp"

using std::cout;

using mylib::greet;

int main() {
  cout << greet("World") << '\n';
  return 0;
}
