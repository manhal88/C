C = SRC/
O = Out/
all_c = $(C)main.c $(C)body.c $(C)safefork.c $(C)history.c
all_o =  $(O)main.o $(O)body.o $(O)safefork.o $(O)history.o
all_file = main.o body.o safefork.o history.o
CFLAGS = -g

gcc:	$(all_c) 
	gcc $(all_c) -c -g -DDEBUG
	mv $(all_file) $(O)

all:	$(all_o)	 
	gcc $(all_o) -g -o  prog
