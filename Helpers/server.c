#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char **argv){
	//variabili
	int sock;
	struct sockaddr_in server;
	int mysock;
	char buff[1024];
	int rval;
	
	
	
	//creazione socket
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		perror("Sock error");
		exit(-1);
	}
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000);
	
	
	
	
	
		
	//call bind	
	if(bind(sock,(struct sockaddr *)&server,sizeof(server))){
		perror("Bind failed");
		exit(-1);
	}
	
	
	//listen
	listen(sock,5);
	
	
	//accept
	
	do {
		mysock = accept(sock, (struct sockaddr *) 0,0);
		if(mysock==-1){
			perror("accept failed");
		}
		else {
			memset(buff,0,sizeof(buff));
			if((rval = read(mysock,buff,sizeof(buff)))<0){
				perror("reading stream message error");
				exit(-1);
			}
			else if (rval ==0){
				printf("Ending connection\n");
			}
			else{
				printf("MSG: %s\n", buff);
			}
			printf("Got the message (rval = %d)\n", rval);
			close(mysock);
		}
	
	
	
	
	
	} while(1);
	
	
	
	
	return 0;
}
