#include "mylib.hpp"

#include <string>

using std::string;

namespace mylib {

string greet(const string &name) {
  return "Hello, " + name + "!";
}

} // namespace mylib
