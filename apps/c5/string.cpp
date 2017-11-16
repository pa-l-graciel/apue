#include <stdio.h>
#include <iostream>

namespace cds {

class X {
 public:
  template <typename T>
  static const T test(T t) {
    return t;
  };
};

template <>
const std::string X::test<std::string>(std::string t) {
  return t;
};
}

int main() {
  cds::X x;
  x.test("aaa");
  std::string a("asdasd");
  x.test(a);

  std::string s;
  s = "127.0.0.1";
  printf("%s\n", std::string("rtp://" + s + ":1237").c_str());
  return 0;
}
