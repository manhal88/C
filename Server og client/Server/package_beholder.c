#include <stddef.h>
#include <stdint.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <inttypes.h>
#define STRING_SIZE 64+64

struct pakke_beholder* search_in_list(int client_navn);
struct pakke_beholder* create_list(int client_navn , int client_level , int cmd , char *path , char *files , char *temp);

struct pakke_beholder {
  int client_navn;
  int level;
  int cmd;
  char path[STRING_SIZE];
  char files[STRING_SIZE];
  char temp[STRING_SIZE];
  struct pakke_beholder *next;
};

struct pakke_beholder *first = NULL;
struct pakke_beholder *current = NULL;


struct pakke_beholder* create_list(int client_navn , int client_level , int cmd , char *path , char *files , char *temp){
  struct pakke_beholder *beholder;
  beholder = (struct pakke_beholder*)malloc(sizeof(struct pakke_beholder));
  beholder->client_navn = client_navn;
  beholder->level = client_level;
  beholder->cmd = cmd;
  memset (beholder->path , 0 , sizeof(beholder->path));
  memset (beholder->files , 0 , sizeof(beholder->files));
  memset (beholder->temp , 0 , sizeof(beholder->temp));
  strcpy(beholder->path , path);
  strcpy(beholder->files , files);
  strcpy(beholder->temp , temp);
  beholder->next = NULL;
  first = current = beholder;
  return beholder;
}
struct pakke_beholder* add_to_list(int client_navn , int client_level , int cmd , char *path , char *files , char *temp){
  if(first == NULL){
    return (create_list(client_navn , client_level , cmd , path , files , temp));
  } else {
    struct pakke_beholder *beholder;
    beholder = search_in_list(client_navn);
    if (beholder == NULL){
      beholder = (struct pakke_beholder*)malloc(sizeof(struct pakke_beholder));
      beholder->client_navn = client_navn;
      beholder->level = client_level;
      beholder->cmd = cmd;
      memset (beholder->path , 0 , sizeof(beholder->path));
      memset (beholder->files , 0 , sizeof(beholder->files));
      memset (beholder->temp , 0 , sizeof(beholder->temp));
      strcpy(beholder->path , path);
      strcpy(beholder->files , files);
      strcpy(beholder->temp , temp);
      beholder->next = NULL;
      current = first;
      while(current->next != NULL){
	current = current->next;
      }
      current->next = beholder;
      return beholder;
    } else {
      beholder->client_navn = client_navn;
      beholder->level = client_level;
      beholder->cmd = cmd;
      memset (beholder->path , 0 , sizeof(beholder->path));
      memset (beholder->files , 0 , sizeof(beholder->files));
      memset (beholder->temp , 0 , sizeof(beholder->temp));

      strcpy(beholder->path , path);
      strcpy(beholder->files , files);
      strcpy(beholder->temp , temp);
    }
  }
}

struct pakke_beholder* search_in_list(int client_navn){
  struct pakke_beholder *beholder = first;
  while(beholder != NULL){
    if(beholder->client_navn == client_navn){
      return beholder;
    } else {
      beholder = beholder->next;
    }
  }
  return NULL;
}

void get_info_from_list(int client_navn , int *client_level , int *cmd , char *path , char *files , char *temp){
  struct pakke_beholder *beholder = NULL;
  beholder = search_in_list(client_navn);
  if (beholder != NULL){
    *client_level = beholder->level;
    *cmd = beholder->cmd;
    strcpy(path,beholder->path);
    strcpy(files,beholder->files);
    strcpy(temp,beholder->temp);
  }
}
void print_list(){
  struct pakke_beholder *beholder = first;
  printf("-------Printing list Start------- \n");
  while(beholder != NULL){
    printf("    [%d]-------------?----?\n",beholder->client_navn);
    printf("-     |      |     [%d]  [%d]\n", beholder->level , beholder->cmd);
    printf("-     |      | \n");
    printf("-     |      ---->[%s] \n",beholder->path);
    printf("-     |      | \n");
    printf("-     |      ---->[%s] \n",beholder->files);
    printf("-     |      | \n");
    printf("-     |      ---->[%s] \n",beholder->temp);
    printf("-     | \n");
    printf("-     ? \n");
    beholder = beholder->next;
  }
  printf("----[null]--------------------- \n");
  printf("-------Printing list End--------- \n");
  return;
}

int remove_from_list(int client_navn){
  struct pakke_beholder *beholder = NULL;
  struct pakke_beholder *temp = NULL;
  current = first;
  beholder = search_in_list(client_navn);
  if(beholder == NULL){
    return -1;
  }
  if(first == beholder){
    first= first->next;
    return 0;
  } else {
    while(current->next != beholder){
      current = current->next;
    }
    temp = current->next;
    temp->next = current->next;
    free(temp);
    temp = NULL;
    return 0;
  }
}

void free_list(){
  if(first->next == NULL){
    free(first);
    return;
  }
  current = first;
  while(current->next != NULL){
    current = current->next;
  }
  free(current);
  free_list();
}
