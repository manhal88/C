#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <stddef.h>
#define STRING_SIZE 64
#define PACKAGE_SIZE 1024
/*-----------------------------------------------------------*/
typedef uint8_t *PAKKE_PEKER;

void analyze_package(PAKKE_PEKER pakke, int filedes);
int read_from_client (int filedes,  char *envp[]);
int write_to_client (int filedes);
void error(const char *msg);

PAKKE_PEKER lag_pakke();
void Free_pakke(PAKKE_PEKER pack);

void sett_in_pakke(PAKKE_PEKER pakke , int client_navn);
/*-----------------------------------------------------------*/



int read_from_client (int client_navn, char *envp[]){
  PAKKE_PEKER pakke = lag_pakke();
  int r;
  r = read(client_navn , pakke , PACKAGE_SIZE);
  if (r <= 0){
    Free_pakke(pakke);
    return 0;
  }
  analyze_package(pakke , client_navn);
  return r;
}
int write_to_client (int client_navn){
  PAKKE_PEKER pakke = lag_pakke();
  int w;
  sett_in_pakke(pakke , client_navn);
  w = write(client_navn , pakke , PACKAGE_SIZE);// &
  Free_pakke(pakke);
  return w;
}

int server(int argc, char *argv[], char *envp[]) {
  int sock , portno , i , newsockfd, retval, dsize;
  fd_set active_fd_set, read_fd_set, write_fd_set;
  socklen_t clilen;
  socklen_t size;
  struct sockaddr_in serv_addr, cli_addr;

  if (argc < 2) {
    fprintf(stderr,"ERROR, no IP or port provided\n");
    exit(1);
  }
  portno = atoi(argv[1]);
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//<<<her
  if (sock < 0) {
    error("ERROR opening socket");
  }
  int activate = 1;
  /* avoid "address already in use" error */
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &activate, sizeof(activate)) == -1) {
    perror("setsockopt:");
    return -1;
  }
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  //serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);//<<her
  serv_addr.sin_port = htons(portno);
  if (retval = bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
    error("ERROR on binding");
  }
  printf("Listener on port %d \n", portno);
  printf("sock er %d \n", sock);
  if (retval = listen(sock, 5) < 0){ //SOMAXCONN
    error("Could not initialize listen queue");
  }
  dsize = getdtablesize();
  FD_ZERO (&active_fd_set);
  FD_SET(sock, &active_fd_set);
  printf("Time to chat\n");

  while (1) {
    read_fd_set = active_fd_set;
    write_fd_set = active_fd_set;
    retval = select(dsize, &read_fd_set, NULL, NULL, NULL);
    if (retval == -1){
      perror("ERROR on select");
    }
    if (FD_ISSET(sock, &read_fd_set)) {
      size = sizeof(cli_addr);
      newsockfd = accept(sock, (struct sockaddr *) &cli_addr , &size);
      if (newsockfd < 0) {
	error("ERROR on accept");
      }
      FD_SET (newsockfd, &active_fd_set);
      continue;
    }
    for (i=0; i<dsize; i++){
      if (i != sock && FD_ISSET(i, &read_fd_set)){
	int client = read_from_client (i , envp);
	if (client <= 0){
	  close(i);
	  FD_CLR (i, &active_fd_set);
	} else {
	  int client = write_to_client(i);
	  if (client <= 0){
	    close(i);
	    FD_CLR (i, &active_fd_set);
	  }
	}
      }
    }
  }
}
