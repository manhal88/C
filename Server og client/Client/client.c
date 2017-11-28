#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h> 
#define STRING_SIZE 64
#define PACKAGE_SIZE 1024
/*-----------------------------------------------------------*/
typedef uint8_t *PAKKE_PEKER;
void error(const char *msg);

void check_cmd(char *path_fp , int client_level_fp , int cmd_fp , int path_size_fp  , int sock);
void print_help_menu();
void print_cd_menu();
void sett_in_pakke(PAKKE_PEKER pakke , int cmd);
void hent_fra_pakke(PAKKE_PEKER pakke , char *path_fp , int *client_levelp , int *cmdp , int *path_sizep);

PAKKE_PEKER lag_pakke();
void Free_pakke(PAKKE_PEKER pack);

void Sett_CLIENT_i_pakke(PAKKE_PEKER pakke , int client_navn);//
void Sett_CLIENT_LEVEL_i_pakke(PAKKE_PEKER pakke , int client_level);//
void Sett_COMMAND_i_pakke(PAKKE_PEKER pakke , int cmd);//
void Sett_PATH_SIZE_i_pakke(PAKKE_PEKER pakke , int path_size);
void Sett_PATH_i_pakke(PAKKE_PEKER pakke , const char* path);

int Hent_CLIENT_fra_pakke(PAKKE_PEKER pakke);//
int Hent_CLIENT_LEVEL_fra_pakke(PAKKE_PEKER pakke);//
int Hent_COMMAND_fra_pakke(PAKKE_PEKER pakke);//
int Hent_PATH_SIZE_fra_pakke(PAKKE_PEKER pakke);
char* Hent_PATH_fra_pakke(PAKKE_PEKER pakke);

void free_list(char *param[], int z);
void read_files(char *line, int choice);
void analyze_package(PAKKE_PEKER pakke , int sock);
/*-----------------------------------------------------------*/

int client(int argc, char* argv[], char *envp[]){
  int sock;
  int portno;
  int strlength;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buf[STRING_SIZE];
  int cmd;
  int retval;
  struct timeval tv;
  int r,w;
  fd_set rd, rd_tmp;

  if (argc < 3) {
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
  }
  portno = atoi(argv[2]);
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    error("ERROR opening socket");
  } else {
    printf("Successfully created new socket:%d\n", sock);
  }
  int activate = 1;
  /* avoid "address already in use" error */
  if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &activate, sizeof(activate)) == -1) {
    perror("setsockopt:");
    return -1;
  }
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  memcpy(&serv_addr.sin_addr, server->h_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  if (connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    error("ERROR connecting\n");
  }


  FD_ZERO(&rd);
  FD_SET(0, &rd);
  FD_SET(sock, &rd);

  printf("cmd (? for help)> ");
  fflush(stdout);
  while (1) {
    rd_tmp = rd;
    tv.tv_sec = 0;
    tv.tv_usec = 50;

    retval = select(sock+1, &rd_tmp, NULL, NULL, &tv);
    if (retval == -1){
      perror("select(1)\n");
    } else if (retval > 0) {

      // Checks for input on standard input
      if (FD_ISSET(0, &rd_tmp)) {
	PAKKE_PEKER pakke = lag_pakke();
	bzero(&buf, STRING_SIZE);
	//scanf("%d", &cmd);
	
	fgets(buf, STRING_SIZE , stdin);
	cmd = buf[0] - '0';
	if( cmd == -2) cmd = 8; // det som tilsvarer   ..(-2)  setter jeg lik 8
	if( cmd == -1) cmd = 9; // det som tilsvarer   / (-1)  setter jeg lik 9
	if( cmd == -16) cmd = 10; //det som tilsvarer ' '(-16) setter jeg lik 10
	sett_in_pakke(pakke , cmd);
	w =  write(sock, pakke , PACKAGE_SIZE);
	if (w <= 0){
	  Free_pakke(pakke);
	  close(sock);
	  exit(1);
	}
	Free_pakke(pakke);
	// Checks for input on network socket
      } else if (FD_ISSET(sock, &rd_tmp)) {
	PAKKE_PEKER pakke = lag_pakke();
	bzero(&buf, STRING_SIZE);
	r = read(sock, pakke , PACKAGE_SIZE);
	if (r <= 0){
	  Free_pakke(pakke);
	  close(sock);
	  exit(1);
	} else {
	  analyze_package( pakke , sock);
	  Free_pakke(pakke);
	  printf("cmd (? for help)> ");
	  fflush(stdout);
	}
      }
    }
    FD_ZERO(&rd_tmp);
  }
  return 0;
}
void analyze_package(PAKKE_PEKER pakke , int sock){
  int *client_levelp , client_level_fp;
  int *cmdp , cmd_fp;
  int *path_sizep , path_size_fp;
  client_levelp = &client_level_fp;          /* get address */
  cmdp = &cmd_fp;                            /* get address */
  path_sizep = &path_size_fp;                /* get address */
  char path_fp[STRING_SIZE];
  memset (path_fp , 0 , sizeof(path_fp));
  hent_fra_pakke( pakke , path_fp , client_levelp , cmdp , path_sizep);
  check_cmd(path_fp , client_level_fp , cmd_fp , path_size_fp , sock );
}
void sett_in_pakke(PAKKE_PEKER pakke , int cmd){
  char tom[STRING_SIZE];
  bzero(&tom, STRING_SIZE);
  Sett_CLIENT_i_pakke( pakke , 0);
  Sett_CLIENT_LEVEL_i_pakke( pakke , 0);
  Sett_COMMAND_i_pakke( pakke , cmd);
  Sett_PATH_SIZE_i_pakke( pakke , 0);
  Sett_PATH_i_pakke( pakke , tom);
}
void hent_fra_pakke(PAKKE_PEKER pakke , char *path_fp , int *client_levelp , int *cmdp , int *path_sizep){
  char * temp_path = Hent_PATH_fra_pakke( pakke);
  strcat( path_fp , temp_path); // kopierer
  *client_levelp = Hent_CLIENT_LEVEL_fra_pakke( pakke);
  *cmdp = Hent_COMMAND_fra_pakke( pakke);
  *path_sizep = Hent_PATH_SIZE_fra_pakke( pakke);
}

void check_cmd(char *path_fp , int client_level_fp , int cmd_fp , int path_size_fp  , int sock){

  //printf("path===============%s\n", path_fp);
  //printf("client_level_fp====%d\n", client_level_fp);
  //printf("cmd_fp=============%d\n", cmd_fp);
  if (client_level_fp == 0){
    if (cmd_fp == 1){
      read_files(path_fp, 0 );
    }
    else if( cmd_fp == 2){
      printf("! %s\n", path_fp);
    }
    else if( cmd_fp == 15){
      print_help_menu();
    }
    else if( cmd_fp == 8){
      printf("! OK\n");
    }
    else if( cmd_fp == 65){
      close(sock);
      //error("hadet\n");
      exit(0);
    }
  }

  else if (client_level_fp == 1){
    if (cmd_fp == 3){
      print_cd_menu();
    }
    else if (cmd_fp == 9){
      read_files(path_fp, 2 );
    }
    else if( cmd_fp == 15){
      print_cd_menu();
    }
    else if( cmd_fp == 65){
    }
  }
  else if (client_level_fp == 2){
    if (cmd_fp == 4){
      read_files(path_fp, 1 );
    }
    else if(cmd_fp == 5){
      read_files(path_fp, 1 );
    }
    else if( cmd_fp == 65){
      print_help_menu();
    }
  }
  else if (client_level_fp == 3){
    if (cmd_fp == 10){
      read_files(path_fp, 1 );
    }
    //print filene
  }
  else if (client_level_fp == 111){
    if (cmd_fp > 0 && cmd_fp < 10){
      printf("! %s\n", path_fp);
    }
  }
}
void print_help_menu(){
 printf ("! Please press a key:\n! [1] list content of current directory (ls)\n! [2] print name of current directory (pwd)\n! [3] change current directory (cd)\n! [4] get file information\n! [5] display file (cat)\n! [?] this menu\n! [q] quit\n");
}

void print_cd_menu(){
  printf ("! ..     the parent directory\n! / a new absolute directory\n!   a new directory relative to the current position\n! [?]    this menu\n! [q]    leave this menu\n");
}

void read_files(char *line, int choice){
  int x;// teller som = strlen(line)
  int y = 0;//teller som = strlen(temp)
  int z = 0;//teller som = strlen(param[x])
  int a = 0;
  char *param[STRING_SIZE];
  char temp[STRING_SIZE];
  memset (temp , 0 , sizeof(temp));
  memset (param , 0 , sizeof(param));
  if (line[0] == '\n'){
    printf("Du skrev ingenting\n");
  } else {
    for(x = 0; x <= strlen(line) ; x++) {// i denne for-loop kopierer fra (line) til (temp) bokstav for bokstav
      if (!isspace(line[x]) && line[x] != '\0') {
	temp[y] = line[x];
	y++;
      }
      if (isspace(line[x]) && line[x] != '\0') {
	param[z] = malloc(strlen(temp) + 1);
	strcpy(param[z] ,temp);
	y = 0;
	z++;
	memset (temp , 0 , sizeof(temp));
      }
    }
    if(choice == 1){
      for (x = 0; x < z ; x++){
	printf("! [%d] %s\n", x+1 , param[x]);
      }
    } else {
      for (x = 0; x < z ; x++){
	printf("! %s\n" , param[x]);
      }
    }
  }
  if(choice == 1) {
    printf("! [q] leave this menu\n");
  }
  free_list(param , z+1);
}
/*     memset(line, 0 , sizeof(line)); */
/*     strcpy(line , temp); */
/*     printf("! [%d] %s\n",y, temp[y]); */
/*   } */
/* } */

void free_list(char *param[], int z) {
  int y;
  for (y = 0; y < z; y++){
    free(param[y]);
  }
}
