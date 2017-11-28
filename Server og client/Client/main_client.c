#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main (int argc, char* argv[], char *envp[]){


  if (argc == 3){
    printf("Entering client mode\n");
    client(argc,argv,envp);
  } else {
    fprintf(stderr,"ERROR, no IP or port provided\n");
    exit(1);
  }
  return 0;
}
