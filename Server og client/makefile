CC = gcc
C = Client/
S = Server/

server = $(S)main_server.c $(S)server.c $(S)package.c $(S)package_beholder.c $(S)body.c 
client = $(C)main_client.c $(C)client.c $(C)package.c

server_file = $(S)main_server.o $(S)server.o $(S)package.c $(S)package_beholder.o $(S)body.o
client_file = $(C)main_client.o $(C)client.o $(C)package.o
CFLAGS = -c -g


all:	server client


server:	$(server_file)
	$(CC) $(server_file) -o server

client:	$(client_file)
	$(CC) $(client_file) -o client

server_file:	$(server)	
	$(CC) $(CFLAGS) $(server)

client_file:	$(client)
	$(CC) $(CFLAGS) $(client)

clean:	
	rm -rf $(S)*.o *~ server
	rm -rf $(C)*.o *~ client
