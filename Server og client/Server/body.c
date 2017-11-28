#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "sys/stat.h"
#include <netinet/in.h>
#include <netdb.h>
#include <dirent.h>
#include <errno.h>
#define STRING_SIZE 64+64
#define PACKAGE_SIZE 1024
/*-----------------------------------------------------------*/
typedef uint8_t *PAKKE_PEKER;
void list_of_dir(char *filelist);
void get_current_dir(char *str1);

void print_list();
int remove_from_list(int client_navn);
struct pakke_beholder* add_to_list(int client_navn,int client_level, int cmd,char *path , char *files , char *temp);
struct pakke_beholder* search_in_list(int client_navn);
void get_info_from_list(int client_navn , int *client_level , int *cmd , char *path , char *files , char *temp);
void finn_file_form_path(int cmd , char *client_path , char *valgete_fil);
void hent_fra_pakke(PAKKE_PEKER pakke , char *path , int *client_level , int *cmd , int *path_size);
void reggular_or_directory(char *path_fl, char *valgete_fil , char *temp);
void to_parent_directory(char *path_fl);
void to_related_directory(char *path_fl, char *valgete_fil);
void get_directory_files(char *path_fl , char *temp);
void finn_dir_form_path(int cmd , char *client_path , char *valgete_fil);
void update_dir(char *filelist , char *path_fl);

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
/*-----------------------------------------------------------*/


void error(const char *msg) {
  perror(msg);
  exit(0);
}

void analyze_package(PAKKE_PEKER pakke , int client_navn){
  //int client_navn;

  int *client_levelp , client_level_fp;
  int *client_levell , client_level_fl;
  int *cmdp , cmd_fp;
  int *cmdl , cmd_fl;
  int *path_sizep , path_size_fp;
  int *path_sizel , path_size_fl;

  client_levelp = &client_level_fp;          /* get address */
  client_levell = &client_level_fl;
  cmdp = &cmd_fp;                            /* get address */
  cmdl = &cmd_fl;
  path_sizep = &path_size_fp;                /* get address */
  path_sizel = &path_size_fl;

  char path_fp[STRING_SIZE];
  char path_fl[STRING_SIZE];
  char files[STRING_SIZE];
  char temp[STRING_SIZE];
  memset (path_fp , 0 , sizeof(path_fp));
  memset (path_fl , 0 , sizeof(path_fl));
  memset (files , 0 , sizeof(files));
  memset (temp , 0 , sizeof(temp));

  hent_fra_pakke( pakke , path_fp , client_levelp , cmdp , path_sizep);
  if (search_in_list(client_navn) == NULL){
    get_current_dir(path_fp); 
    client_level_fp = 0;
    list_of_dir(files);
    add_to_list(client_navn , client_level_fp , cmd_fp , path_fp , files , temp);
  }
  get_info_from_list(client_navn , client_levell , cmdl , path_fl , files , temp);

  if (client_level_fl == 111 || client_level_fl == 222) client_level_fl = 0;

  if (client_level_fl == 0) {
    if ( cmd_fp == 1 ){
      client_level_fp = 0;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp == 2 ){
      client_level_fp = 0;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp == 3 ){
      client_level_fp = 1;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp == 4 ){
      client_level_fp = 2;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp == 5 ){
      client_level_fp = 2;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp == 15 ){
      client_level_fp = 0;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp == 65 ){
      client_level_fp = 0;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
      //remove_from_list(client_navn);
    }
  }
  else if (client_level_fl == 1) {
    if ( cmd_fp == 8 ){// 8 ..
      //..mappe opp
      client_level_fp = 0;
      to_parent_directory(path_fl);
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp == 9){//9 /
      // / ny men
      client_level_fp = 4;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp == 10){//10 ' '
      // ' 'mappe ned
      client_level_fp = 3;
      get_directory_files(path_fl , temp);
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp == 15){
      client_level_fp = 1;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
      //[?] this
    }
    else if(cmd_fp == 65){
      //[q]
      client_level_fp = 111;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
  }
  else if (client_level_fl == 2){
    if(cmd_fp == 65){
      //[q]
      client_level_fp = 111;
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp > 0 && cmd_fp < 10) {
      char valgete_fil[STRING_SIZE];
      client_level_fp = 111;

      finn_file_form_path( cmd_fp , path_fl , valgete_fil);
      reggular_or_directory(path_fl, valgete_fil , temp);
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
      //velg en av filene
    }
  }
  else if (client_level_fl == 3){
    if(cmd_fp == 65){
      client_level_fp = 222;
      //mo fikses
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
    else if(cmd_fp > 0 && cmd_fp < 10) {
      char valgete_fil[STRING_SIZE];
      client_level_fp = 222;
      finn_dir_form_path( cmd_fp , path_fl , valgete_fil);
      to_related_directory( path_fl, valgete_fil);
      update_dir(files, path_fl);
      printf("path_fl=%s\n",path_fl);
      printf("files=%s\n",files);
      add_to_list(client_navn , client_level_fp , cmd_fp , path_fl , files , temp);
    }
  }
  Free_pakke(pakke);
}

void hent_fra_pakke(PAKKE_PEKER pakke , char *path , int *client_level , int *cmd , int *path_size){
  char * temp_path = Hent_PATH_fra_pakke( pakke);
  strcat( path , temp_path); // kopierer
  *client_level = Hent_CLIENT_LEVEL_fra_pakke( pakke);
  *cmd = Hent_COMMAND_fra_pakke( pakke);
  *path_size = Hent_PATH_SIZE_fra_pakke( pakke);
}

void sett_in_pakke(PAKKE_PEKER pakke , int client_navn){

  int *client_levell , client_level_fl;
  int *cmdl , cmd_fl;
  int *path_sizel , path_size_fl;

  client_levell = &client_level_fl;          /* get address */
  cmdl = &cmd_fl;                            /* get address */
  path_sizel = &path_size_fl;                /* get address */

  char path_fl[STRING_SIZE];
  char files[STRING_SIZE];
  char temp[STRING_SIZE];

  memset (path_fl , 0 , sizeof(path_fl));
  memset (files , 0 , sizeof(files));
  memset (temp , 0 , sizeof(temp));

  get_info_from_list(client_navn , client_levell , cmdl , path_fl , files , temp);
 

  *path_sizel = strlen(path_fl);
  Sett_CLIENT_i_pakke( pakke , client_navn);
  Sett_CLIENT_LEVEL_i_pakke( pakke , client_level_fl);
  Sett_COMMAND_i_pakke( pakke , cmd_fl);
  Sett_PATH_SIZE_i_pakke( pakke , path_size_fl);

  if (client_level_fl == 0){
    if(cmd_fl == 1){
      //her
      Sett_PATH_i_pakke( pakke , files);

    }
    else if(cmd_fl == 65){
      remove_from_list(client_navn);
    }
    else {
      Sett_PATH_i_pakke( pakke , path_fl);
    }
  }
  else if (client_level_fl == 1){

  }
  else if (client_level_fl == 2){
    if(cmd_fl == 4){
      Sett_PATH_i_pakke( pakke , files);
      //her
    }
    else if(cmd_fl == 5){
      Sett_PATH_i_pakke( pakke , files);
      //her
    }
    else {
      Sett_PATH_i_pakke( pakke , path_fl);
    }
  }
  else if (client_level_fl == 111){
    Sett_PATH_i_pakke( pakke , temp);
  }
  else if (client_level_fl == 222){
    Sett_PATH_i_pakke( pakke , temp);
  }
  else if (client_level_fl == 3){
    if(cmd_fl == 9){
      Sett_PATH_i_pakke( pakke , files);
      //her
    }
     else if(cmd_fl == 10){
       Sett_PATH_i_pakke( pakke , temp);
     }
  }
  else {
    Sett_PATH_i_pakke( pakke , path_fl);
  }
  print_list();
}

void list_of_dir(char *filelist){
  char str1[STRING_SIZE];
  struct dirent *ent;
  const char * d_name;
  DIR *dir;
  memset (filelist , 0 , sizeof(filelist));
  memset (str1 , 0 , sizeof(str1));
  get_current_dir(str1);
  dir = opendir(str1);
  while ((ent = readdir(dir))!= NULL){
    d_name = ent->d_name;
    if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
      strcat(filelist, d_name);
      strcat(filelist, " ");
    }
  }
  closedir (dir);
}
void update_dir(char *filelist , char *path_fl){
  char str1[STRING_SIZE];
  struct dirent *ent;
  const char * d_name;
  DIR *dir;
  memset (str1 , 0 , sizeof(str1));
  memset (filelist , 0 , sizeof(filelist));
  dir = opendir(path_fl);
  while ((ent = readdir(dir))!= NULL){
    d_name = ent->d_name;
    if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
      strcat(filelist, d_name);
      strcat(filelist, " ");
    }
  }
  closedir (dir);
}
void get_current_dir(char *str1){
  char* cwd;
  char buff[STRING_SIZE];
  memset (buff , 0 , sizeof(buff));
  cwd = getcwd( buff, sizeof(buff));
  if( cwd != NULL ) {
  }
  strcpy(str1 , cwd );
}

void finn_file_form_path(int cmd , char *client_path , char *valgete_fil){

  struct dirent *ent;
  const char * d_name;
  DIR *dir;
  int n = 0;
  char filelist[30][256];
  memset (valgete_fil , 0 , sizeof(valgete_fil));
  memset (filelist , 0 , sizeof(filelist));
  dir = opendir(client_path);
  cmd -= 1;
  while ((ent = readdir(dir))!= NULL){
    d_name = ent->d_name;
    if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
      strcpy(filelist[n], d_name);
      n++;
    }
  }
  closedir(dir);
  strcpy(valgete_fil , filelist[cmd]);
}
void reggular_or_directory(char *path_fl, char *valgete_fil, char *temp) {
  struct stat status;
  char str[2*STRING_SIZE];
  memset (str , 0 , sizeof(str));
  memset (temp , 0 , sizeof(temp));
  strcpy(str , path_fl);
  strcat(str , "/");
  strcat(str , valgete_fil);
  stat(str,&status);
  if (S_ISREG(status.st_mode)){
    strcpy(temp , valgete_fil);
    strcat(temp , " is reggular file.");
  }
  if (S_ISDIR(status.st_mode)){
    strcpy(temp , valgete_fil);
    strcat(temp , " is directory.");
  }
  if (S_ISLNK(status.st_mode)){
    strcpy(temp , valgete_fil);
    strcat(temp , " is link.");
  }
}
void to_parent_directory(char *path_fl){
  char str[STRING_SIZE];
  char *temp = malloc(STRING_SIZE);
  char *temp1[STRING_SIZE];

  memset (temp , 0 , sizeof(temp1));
  memset (str , 0 , sizeof(str));
  memset (temp1 , 0 , sizeof(temp1));

  int i = 0 , x , y = 0;
  i = strlen(path_fl);
  get_current_dir(str);

  for(x = i; x >= 0; x--){
    if (path_fl[x] ==  '/' ){
      y++;
    }
    if(y != 0){
      strncpy(temp, path_fl,x);
      break;
    }    
  }
  if (strcmp(str , temp) <= 0) {
    strcpy(path_fl , temp);
    printf("her \n");
  } else {
    printf("eller aldri \n");
  }
  //free(temp);
}
void to_related_directory(char *path_fl, char *valgete_fil){
  strcat(path_fl , "/");
  strcat(path_fl , valgete_fil);
}

void get_directory_files(char *path_fl , char *temp) {

  struct dirent *ent;
  struct stat status;
  const char * d_name;
  DIR *dir;
  memset (temp , 0 , sizeof(temp));

  dir = opendir(path_fl);
  while ((ent = readdir(dir))!= NULL){
    d_name = ent->d_name;
    if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
      stat(d_name , &status);
      if (S_ISDIR(status.st_mode)){
	strcat(temp , d_name);
	strcat(temp , " ");
      }
    }
  }
  printf("hahahah  %s\n", temp);
  closedir(dir);
}

void finn_dir_form_path(int cmd , char *client_path , char *valgete_fil){

  struct dirent *ent;
  struct stat status;
  const char * d_name;
  DIR *dir;
  int n = 0;
  char filelist[30][256];
  memset (valgete_fil , 0 , sizeof(valgete_fil));
  memset (filelist , 0 , sizeof(filelist));
  dir = opendir(client_path);
  cmd -= 1;
  while ((ent = readdir(dir))!= NULL){
    d_name = ent->d_name;
    if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
      stat(d_name , &status);
      if (S_ISDIR(status.st_mode)){
	strcpy(filelist[n], d_name);
	n++;
      }
    }
  }
  closedir(dir);
  strcpy(valgete_fil , filelist[cmd]);
}
