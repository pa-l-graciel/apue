#include <stdlib.h>
#include <iostream>

void PrintEnv(const char* name) {
  char* val = getenv(name);
  if (val != nullptr)
    std::cout << val << std::endl;
  else
    std::cout << "env is null" << std::endl;
}

int main() {
  PrintEnv("HOME");
  // Note: argument for putenv must be allocated on the heap
  char* c = new char[256];
  snprintf(c, 256, "MY_NAME=YS");
  putenv(c);
  PrintEnv("MY_NAME");
  return 0;
}
