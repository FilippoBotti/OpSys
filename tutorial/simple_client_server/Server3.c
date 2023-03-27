#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "string.h"
#define LEN_MAX 20


typedef struct _RICHIESTA_MSG
{
	int req;
	char tipo[LEN_MAX + 1];
} RICHIESTA_MSG;

typedef struct _RISPOSTA_MSG
{
	int answ;
	char tipo[LEN_MAX + 1];
} RISPOSTA_MSG;


int main(int argc, char* argv[]){
	int sock, length, portno;
	struct sockaddr_in server, client;
	int pid,s,current_socket,rval,rval2,i;
	RICHIESTA_MSG request;
	RISPOSTA_MSG answer;

	//CONTROLLO GLI ARGOMENTI
	if(argc!=2){
	printf("Usare: %s <port_number> \n" , argv[0]);
	exit(-1);
	}

	int porta = 1000;

	//1.
	//CREA LA SOCKET STREAM
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0){
		perror("Opening stream socket");
		exit(1);
		//errore apertura socket, chiudo il programma
	}

	//socket correttamente aperta
	
	portno = atoi(argv[1]);
	
	//2.BINDING SERVER
	server.sin_family = AF_INET;
	//Uso la wildcard INADDR_ANY per accettare conessioni rievute da qualunque interfaccia di rete del sistema
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(portno);
	
	if(bind(sock,(struct sockaddr *)&server, sizeof(server))<0){
		perror("Binding error");	
		exit(1);
		//binding non corretto esco
	}

	//binding corretto

	//3. 
	length = sizeof(server);
	if(getsockname(sock,(struct sockaddr *)&server, &length)<0){
		perror("Getting socket name");	
		exit(1);
	}
	
	// ho tirato su il server
	printf("Socket port #%d\n" , ntohs(server.sin_port));
	
	//Pronto ad accettare connessioni
	
	//4.
	listen(sock,2);
	
	//5. effettivo programma
	int contatore = 0;
	while(1){
		//attesa di una connessione
		//.6
		current_socket = accept(sock,(struct sockaddr *)&client, (socklen_t *)&length);
		
		if(current_socket ==-1){
			perror("accept");
		}
		else
		{
			printf("Connection from %s, port %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		//server corrente
			pid = fork();
			if(pid==0){
				//processo figlio, gestisco richiesta client
				if(read(current_socket, &request, sizeof(request))<0){
					perror("messaggio non ricevuto");
					exit(-1);
				}
				//Esecuzione del servizio
				if(strncmp(request.tipo, "ADD", LEN_MAX) == 0){
					printf("Ricevuto: %s e %d", request.tipo, request.req);
					contatore += request.req;
					answer.answ = 10;
					strcpy(answer.tipo, "ADD_OK");
					if(write(current_socket, &answer, sizeof(answer))<0){
						perror("errore nella write");
						exit(-1);
					}
				}
				else {
					printf("Ricevuto errore");
					answer.answ = 0;
					strcpy(answer.tipo, "ERR");
					write(current_socket, &answer, sizeof(answer));
				}
				close(current_socket);
				exit(0);
			}
				
			else {
				if(pid ==-1) { 
					perror("fork error"); 
					exit(-1);
				}
				else{
					//ok il padre torna in accept
					close(current_socket);
				}
			}
		}	
	} 
	exit(0);
}















































