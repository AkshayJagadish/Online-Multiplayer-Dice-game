#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include<time.h>

void child(int, char *);

int getRandomInteger(){
    srand(time(NULL));
    return (rand() % 6 + 1);
}

int main(int argc, char *argv[]){  
  int sd, client, portNumber;
  struct sockaddr_in servAdd;     // server socket address
    struct sockaddr_in clientAdd; // client socket address
    socklen_t slen = sizeof(clientAdd);
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
    client = accept(sd, (struct sockaddr *) &clientAdd, &slen);
    printf("\nGot a new client\n\nServicing Client.....\n");

      if(!fork()) {
//          printf("IP address is: %s\n", inet_ntoa(clientAdd.sin_addr));
          child(client, inet_ntoa(clientAdd.sin_addr));
      }

    close(client);
  }
}

void child(int sd, char *clientIP){
	char message[255];
    
    int serverScore = 0;
    int clientScore = 0;
    int serverDice = 0;
    int clientDice = 0;
    char c;
    
    fprintf(stderr, "Game on, you can play your dice.\n");

	while(1){
        
        while(1) {
            fprintf(stderr, "\nHit Enter to roll dice: ");
            c = getchar();
            
            if(c == '\n') {
                // double return pressed!
                break;
            }
        }
        
        serverDice = getRandomInteger();
        serverScore += serverDice;
        
        // (server score, client score)
        snprintf(message, sizeof(message), "%d,%d", serverScore, serverDice);
        
		write(sd, message, strlen(message));
        
        if (serverScore >= 10) {
            fprintf(stderr, "\nClient's IP: %s\n\nServer Dice rolled: %d\nClient Dice rolled: %d\nClient Score: %d\nMy Score is: %d\n\n", clientIP, serverDice, clientDice, clientScore, serverScore);
            fprintf(stderr, "I won with score %d\n\n", serverScore);
            fprintf(stderr, "\n\nWaiting for another connection......\n");
            sleep(1);
            exit(0);
        }
        
        memset(message, 0, strlen(message));
		if(!read(sd, message, 255)){
			close(sd);
			fprintf(stderr, "Bye, my client is dead, wait for a new one\n");
            fprintf(stderr, "\nWaiting for another connection......\n");
			exit(0);
		}
        
        char *token = strtok(message, ",");
        clientScore = atoi(message);
        token = strtok(NULL, ",");
        clientDice = atoi(token);
        
        if (clientScore >= 10) {
            fprintf(stderr, "\nClient's IP: %s\n\nServer Dice rolled: %d\nClient Dice rolled: %d\nClient Score: %d\nServer Score is: %d\n\n", clientIP, serverDice, clientDice, clientScore, serverScore);
            fprintf(stderr, "Client %s won the game with score %d\n", clientIP, clientScore);
            sleep(1);
            exit(0);
        }
        
        fprintf(stderr, "\nClient's IP: %s\n\nServer Dice rolled: %d\nClient Dice rolled: %d\nClient Score: %d\nMy Score is: %d\n\n", clientIP, serverDice, clientDice, clientScore, serverScore);
        
        sleep(1);
	}
}
