#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif


extern pid_t safefork(void);
int input(char *line , char *envp[] , int teller);
int start(char *user , int teller , char *envp[]);
int check_exit(char *param);
void check_h(char *param , int teller);
int check_reg(const char *path);
int check_dir(const char *path);
void check_prog(char *param[] , char *envp[] , int  teller);
void free_list(char *param[], int z);




/* start metode blir kalt fra main og som har en evig for loop som
 holder programmet kontinuerlig og leser linjer fra terminalen */
//start parameter er (user fra main, teller pa antal kommandoer, envp som blir brukt i execve)
int start(char *user , int teller , char *envp[]) {
  char line[120];// = {'\0'};

  for (;;) {
    DEBUG_PRINT(("[%s@ifish] >%d ", user , teller));
    if (fgets(line, 120, stdin) == NULL){
      DEBUG_PRINT(("\nProgrammet her avsluttet med: Ctrl^d\n"));
      free_memory();//free_memory er en metode som ligger i history.c og freeer alt
      exit(1);
    }
    DEBUG_PRINT(("Du skrev: %s", line));
    teller = input(line , envp , teller);
  }
  return 0;
}
/* input metode som tar line fra start og legger den i param, setter 
line in i history, kaller pa sjekk for (exit), kaller pa sjekk for (h), 
kaller pa (check_prog og den siste skekker om komandoen er execverbar 
eller ikke) og til slutt free-er listene mine
*/
// input parameter er (line fra start metode,  envp som blir brukt i execve , teller pa antal kommandoer )
int input(char *line , char *envp[], int teller) {
  int x;// teller som = strlen(line)
  int y = 0;//teller som = strlen(temp)
  int z = 0;//teller som = strlen(param[x])
  int a = 0;//teller som = strlen(path_arr)
  char mellom[] = "";
  char *param[21];
  char temp[20];
  memset (temp , 0 , sizeof(temp));
  memset (param , 0 , sizeof(param));
  if (line[0] == '\n'){
    DEBUG_PRINT(("Du skrev ingenting\n"));
  } else {
    for(x = 0; x <= strlen(line) ; x++){// i denne for-loop kopierer fra (line) til (temp) bokstav for bokstav
      if (!isspace(line[x]) && line[x] != '\0') {
	temp[y] = line[x];
	y++;
      }
      if (isspace(line[x]) && line[x] != '\0' && line[x] != '@') {
	if(strcmp(temp, mellom) != 0 ){ //sjekke om det ikke er to mellomrom etter varandre
	  if(z == 0){
	    if (check_exit(temp) == 1) {//sjekke om farste param er exit eller quit
	      free_memory();
	      exit(0);
	    }
	  }
	  param[z] = malloc(strlen(temp) + 1);
	  strcpy(param[z] ,temp);
	  y = 0;
	  z++;
	  memset (temp , 0 , sizeof(temp));
	}
      }
    }
    sett_in(line);// sett in linjen i history
    check_h(param[0] , teller);//sjekke farste param er h
    check_prog(param , envp , z);//sjekke param om det er et program eller ikke
    teller++;
  }
  free_list(param , z+1);
  return teller;
}
/* check_prog tar hele param in som parameter og bruker den i execve, i denne metoden
finner jeg pathen, splitter den, limer farste komando fra param og finner om den er execverbar
i tillegg lager barn procces 
 */
//check_prog parameter er (param fra input metode, envp som blir brukt i execve, 
//z som er storrelse til param for a free param)
void check_prog(char *param[] , char *envp[] , int z){

  int x = 0;
  int y = 0;
  char *path;
  char *direct = 0;
  char str2[5] = "/"; // bruker den mellom pathen og kommandoen
  char *path_arr[20]; //jeg legger alle pathene i

  path = getenv("PATH");
  path = strdup(path);
  direct = strtok(path , ":");
  while (direct != NULL) {
    path_arr[x] =  malloc(strlen(direct) + 1 + strlen(str2) + strlen(param[0]));
    strcpy(path_arr[x] , direct);
    x++;
    direct = strtok(NULL, ":");
  }
  free(path);
  int status;
  int barnid;
  barnid = safefork();
  if (barnid == 0){
    //DEBUG_PRINT(("dette er barn procces\n"));
    for (y = 0; y < x; y++){
      strcat(path_arr[y], str2);// kopierer "/"
      strcat(path_arr[y], param[0]);// kopierer param[0]
      if (access(path_arr[y], F_OK)!= -1){
	execve(path_arr[y], param, envp);
      } else {
      }
    }
    DEBUG_PRINT(("ifish: %s: command not found\n", param[0]));
    free_list(path_arr , x);
    free_list(param , z+1);
    free_memory();
    exit(0);
  } else {
    barnid = wait(&status);
    //DEBUG_PRINT(("dette er parent procces\n"));
  }
  free_list(path_arr , x);
}

/* check_exit metode som sjekker om det er en av to kommandoenen  exit eller quit
 */
//check_exit parameter er (param[0] fra input metode)
int check_exit(char *param ){//sjekke om
  char temp1[] = "exit";
  char temp2[] = "quit";

  if (strcmp(param, temp1) == 0 || strcmp(param, temp2) == 0) {
    DEBUG_PRINT(("Program slutt\n"));
    return 1;
  } else {
    return 0;
  }
}
/* check_h metode som sjekker om det er h kommando
 */
//check_h parameter er (param[0] fra input metode,  teller pa antal kommandoer(jeg 
//bruker teller som print-storrelse-loop i history listen min ( det er en dorlig mote))  
void check_h(char *param , int teller){//sjekke om
  char temp3[] = "h";

  if (strcmp(param, temp3) == 0 ) {

    print_h(teller);
  }
}

void free_list(char *param[], int z) {
  int y;
  for (y = 0; y < z; y++){
    free(param[y]);
  }
}
