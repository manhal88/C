#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main (int argc, char* argv[], char *envp[]){

  if (argc == 2) {
    printf("Entering server mode\n");
    server(argc,argv,envp);
  } else {
    fprintf(stderr,"ERROR, no IP or port provided\n");
    exit(1);
  }
  return 0;
}
