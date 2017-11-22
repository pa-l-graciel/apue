#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

int main() {
  struct passwd* p = getpwnam("ys");

  printf("pw_name: %s\n", p->pw_name);
  printf("pw_passwd: %s\n", p->pw_passwd);
  printf("pw_uid: %ld\n", p->pw_uid);
  printf("pw_gid: %ld\n", p->pw_gid);
  printf("pw_gecos: %s\n", p->pw_gecos);
  printf("pw_dir: %s\n", p->pw_dir);
  printf("pw_shell: %s\n", p->pw_shell);

  return 0;
}
