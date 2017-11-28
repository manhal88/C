#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <inttypes.h>

#define CLIENT_NAVN_POS 0//0+1
#define CLIENT_LEVEL_POS 2//2+3

#define COMMAND_POS 4//4+5
#define PATH_SIZE_POS 6//6+7
#define CLIENT_PATH_POS 8//8+PATH_SIZE_POS

typedef uint8_t *PAKKE_PEKER;
PAKKE_PEKER lag_pakke();
void Free_pakke(PAKKE_PEKER pakke);

PAKKE_PEKER lag_pakke(){
  PAKKE_PEKER pakke = (PAKKE_PEKER)malloc(1024);
  memset(pakke , 0 , 1024);
  return pakke;
}
void Free_pakke(PAKKE_PEKER pakke) {
  free(pakke);
}
/*___________________sett client navn i pakke___________________ok____*/
void Sett_CLIENT_i_pakke(PAKKE_PEKER pakke , int client_navn) {
 
  uint8_t b_endian = client_navn / 256;
  uint8_t s_endian = client_navn % 256;
  pakke[CLIENT_NAVN_POS] = b_endian;
  pakke[CLIENT_NAVN_POS+1] = s_endian;
  uint8_t *p = &pakke[CLIENT_NAVN_POS];
  uint16_t *q = (uint16_t*)p;
  /* if(*q == client_navn){ */
  /*   fprintf(stderr, "BIG ENDIAN navn på denne maskinen"); */
  /* }else{ */
  /*   fprintf(stderr, "SMALL ENDIAN navn på denne maskinen"); */
  /* } */
}
int Hent_CLIENT_fra_pakke(PAKKE_PEKER pakke) {
  uint8_t client_navn = pakke[CLIENT_NAVN_POS+1];
  //uint8_t client_navn = pakke[CLIENT_NAVN_POS];
  return client_navn;
}
/*_____________________________^^^____________________________________*/

/*___________________sett client level i pakke_________________ok_____*/
void Sett_CLIENT_LEVEL_i_pakke(PAKKE_PEKER pakke , int client_level) {
  
  uint8_t b_endian = client_level / 256;
  uint8_t s_endian = client_level % 256;
  pakke[CLIENT_LEVEL_POS] = b_endian;
  pakke[CLIENT_LEVEL_POS+1] = s_endian;
  uint8_t *p = &pakke[CLIENT_LEVEL_POS];
  uint16_t *q = (uint16_t*)p;

  /* if(*q == client_level){ */
  /*   fprintf(stderr, "BIG ENDIAN level på denne maskinen"); */
  /* }else{ */
  /*   fprintf(stderr, "SMALL ENDIAN level på denne maskinen"); */
  /* } */
}
int Hent_CLIENT_LEVEL_fra_pakke(PAKKE_PEKER pakke) {
  uint8_t client_level = pakke[CLIENT_LEVEL_POS+1];
  //uint8_t client_level = pakke[CLIENT_LEVEL_POS]
  return client_level;
}
/*_____________________________^^^____________________________________*/

/*___________________sett COMMAND i pakke__________________ok____*/
void Sett_COMMAND_i_pakke(PAKKE_PEKER pakke , int cmd) {
  uint8_t b_endian = cmd / 256;
  uint8_t s_endian = cmd % 256;
  pakke[COMMAND_POS] = b_endian;
  pakke[COMMAND_POS+1] = s_endian;
  uint8_t *p = &pakke[COMMAND_POS];
  uint16_t *q = (uint16_t*)p;

  /* if(*q == cmd){ */
  /*   fprintf(stderr, "BIG ENDIAN cmd på denne maskinen"); */
  /* }else{ */
  /*   fprintf(stderr, "SMALL ENDIAN cmd på denne maskinen"); */
  /* } */
}
int Hent_COMMAND_fra_pakke(PAKKE_PEKER pakke) {
  uint8_t cmd = pakke[COMMAND_POS+1];
  // uint8_t cmd = pakke[COMMAND_POS];
  return cmd;
}
/*_____________________________^^^___________________________________*/

/*___________________sett path size i pakke____________________ok___*/
void Sett_PATH_SIZE_i_pakke(PAKKE_PEKER pakke , int path_size) {
  uint8_t b_endian = path_size / 256;
  uint8_t s_endian = path_size % 256;
  pakke[PATH_SIZE_POS] = b_endian;
  pakke[PATH_SIZE_POS+1] = s_endian;
  uint8_t *p = &pakke[PATH_SIZE_POS];
  uint16_t *q = (uint16_t*)p;

  /* if(*q == path_size){ */
  /*   fprintf(stderr, "BIG ENDIAN path på denne maskinen"); */
  /* }else{ */
  /*   fprintf(stderr, "SMALL ENDIAN path på denne maskinen"); */
  /* } */
}
int Hent_PATH_SIZE_fra_pakke(PAKKE_PEKER pakke) {
  uint8_t path_size = pakke[PATH_SIZE_POS+1];
  //uint8_t path_size = pakke[PATH_SIZE_POS];
  return path_size;
}
/*_____________________________^^^__________________________________*/

/*______________________sett path i pakke_______________________ok___*/
void Sett_PATH_i_pakke(PAKKE_PEKER pakke , const char* path) {
  char*p = (char*) &pakke[CLIENT_PATH_POS];
  strcpy(p, path);
}

char* Hent_PATH_fra_pakke(PAKKE_PEKER pakke) {
  char *path;
  path = (char*) (pakke + CLIENT_PATH_POS);
  return path;
}
/*_____________________________^^^___________________________________*/
