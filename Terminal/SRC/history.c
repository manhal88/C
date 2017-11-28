#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif

void print_h(int teller);
int decimal_binary(int n);
void add_bit(int tall);
int sjekk_bit( int tall);
void free_memory();
void print_bitmap(int *arr);/////////
void longlong64_to_arr(long long bitmap, int *b);///////////


// struct for history og bitmap-en min.
typedef struct h_liste {
  char history[64][8];//64 blokk med 8 byte hver
  long long bitmap;//64
}h_liste;

// struct for list
typedef struct meta_data{
  int data_index[15];
  int length;
  struct meta_data *next;
}meta_data;

h_liste * memory;
meta_data * minlist;

//lage plass til min lister
void lag_plass(){
  memory = malloc(sizeof(struct h_liste));
  memset (memory , 0 , sizeof(struct h_liste));
  minlist = malloc(sizeof(struct meta_data));
  memset (minlist , 0 , sizeof(struct meta_data));
}
/* sett_in metode tar in en line fra input metode(body.c) setter in 
kommandoene bokstav for bokstav i datablokkene. den sjekker bitma-pen 
min om ledig(i oppgaven sta at vi skal finne en tilfeldig plass i 
bitmap-en men jeg tokk den samme posisjon til datablokkene til 
bitmap-en) og adder 1 i denne plassen i bitma-pen. den kaller ogsa pa
print bitmap metode
 */

void sett_in(char *line){
  int x = 0;
  int y = 0;
  int z = 0;
  int a = 0;
 
  a = strlen(line);
  for(x = 0; x < 64 ; x++){
    if(memory->history[x][0] == '\0'){
      for(y = 0; y < 8 ; y++){
	if(z < a){
	  if (sjekk_bit(x) == 0 ){
	    add_bit(x);
	  }
	  memory->history[x][y] = line[z];
	  z++;
	} else {
	  memory->history[x][y] = 0;
	}
      }
    }
  }
  int bit_arr[64];
  longlong64_to_arr(memory->bitmap, bit_arr);
  print_bitmap(bit_arr);
}
/*
print_h metode tar inn teller som parameter og printer ut history
hvis teller er storre en 10 so printer ut siste 10
 */
void print_h(int teller){
  int x = 0;
  int y = 0;
  int z = teller;
  int a = teller;
  if( z > 10){
    z = 10;
  }
  DEBUG_PRINT(("History of the last %d commands:\n", z));
  DEBUG_PRINT(("\t%d: ", z));
  for(x = 0; x < 64 ; x++){
    if ( a <= 10) {
      if(z != 0 || z > 0){
	if(memory->history[x][0] != '\0'){
	  for(y = 0; y < 8 ; y++){
	    if(z > 0){
	      DEBUG_PRINT(("%c" , memory->history[x][y]));
	      if(z > 1){
		if(memory->history[x][y] == '\n'){
		  z--;
		  DEBUG_PRINT(("\t%d: ", z));
		}
	      }
	    }
	  }
	}
      }
    }
    a--;
  }
  DEBUG_PRINT((" \n"));
}
/*
sjekk_bit metode tar immot en plasstil en bit og sjekker
 om denne plassen er ledig eller opptatt i bitmap-en min
retunere 0 hvis det er ledig og 1 hvis ikke
 */

int sjekk_bit( int tall){
  uint64_t enbit = 0;
  enbit = enbit | 0x01;
  enbit = enbit << tall;
  enbit = memory->bitmap & enbit;
  return enbit;
}
/*
add_bit metode adder 1 i (x) posisjon som er (tall)
 */
void add_bit(int tall){
  uint64_t enbit = 0;
  enbit = enbit | 0x01;
  enbit = enbit << tall;
  memory->bitmap = memory->bitmap | enbit;
}

//longlong64_to_arr metode som lager en koppy av min bitmap i en 
//array hved hjelp av print_bitmap
void longlong64_to_arr(long long bitmap, int *bit_arr){
  int length = 64;
  int i;
  for(i = length - 1; i >= 0; i--){
    *(bit_arr+i) = (bitmap >> i) & 1;
  }
}
//print_bitmap metode som printer ut en array som er en koppy av 
//min bitmao
void print_bitmap(int *bit_arr){
  int length = 64;
  int i;
  DEBUG_PRINT(("DEBUG - BITMAP\n"));
  DEBUG_PRINT(("\n"));
  for(i = 0; i < length; i++) {
    if(i == 32){
      DEBUG_PRINT(("\n"));
    }
    DEBUG_PRINT(("%d" , *(bit_arr + i)));
  }
  DEBUG_PRINT(("\n\n"));
}
/* 
hjelpe metode til a printe ut decimal to binary
 */
int decimal_binary(int n) {  /* Function to convert decimal to binary.*/

  int rem, i=1, binary=0;
  while (n!=0) {
      rem=n%2;
      n/=2;
      binary+=rem*i;
      i*=10;
    }
  return binary;
}

//free
void free_memory(){
  free(memory);
  free(minlist);
}
