#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
  char* p = envp[0];

  for (int i = 0; envp[i] != nullptr; ++i) {
    printf("%s\n", envp[i]);
  }
  return 0;
}
