#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main (int argc, char* argv[], char *envp[]){


  char *user;
  int teller = 1;
  user = getenv("USER");
  if (user != NULL) {
    lag_plass();
    start(user , teller , envp);
  }
  return 0;
}
