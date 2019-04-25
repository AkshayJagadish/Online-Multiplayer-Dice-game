#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include<time.h>
#include <ifaddrs.h>

int getRandomInteger(){
	  srand(time(NULL));
	    return(rand()%6 + 1);
}

int main(int argc, char *argv[]){
	//system("C:\\Windows\\System32\\ipconfig");
	  char message[255]="message from akshay";
	    int server, portNumber;
	      struct sockaddr_in servAdd;     // server socket address
	        
	       if(argc != 3){
		           printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
			       exit(0);
			         }

	         if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			      fprintf(stderr, "Cannot create socket\n");
			           exit(1);
				     }

		   servAdd.sin_family = AF_INET;
		     sscanf(argv[2], "%d", &portNumber);
		       servAdd.sin_port = htons((uint16_t)portNumber);

		         if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
				   fprintf(stderr, " inet_pton() has failed\n");
				     exit(2);
			 }

			   
			  if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
				    fprintf(stderr, "connect() has failed, exiting\n");
				      exit(3);
				       }
				int serverScore=0;
				int clientScore=0;
				int rand;
				int serverDice;
				int no;
				char c;
			   	while(1){
				  //scanf("%d",&no);
			
				   sleep(3);
				   rand=getRandomInteger();
				   clientScore+=rand;
				   
								
					

				     if (read(server, message, 255)<0){
					         fprintf(stderr, "read() error\n");
						     exit(3); 
						      }
				  while(1)
				  {
					  fprintf(stderr,"\nHit Enter to play the dice");
					  c=getchar();

					  if(c=='\n')
						  break;
				  }

				     //char *token= strtok(message,",");
				     char *token=strtok(message,",");
				     serverScore=atoi(message);
				     token=strtok(NULL,",");
				     serverDice=atoi(token);
				     printf("\nClient has rolled: %d\n",rand);
				     printf("\nServer has rolled: %d\n",serverDice);
				     if(serverScore>=10){
 					     printf("\nServer score: %d \nClient score: %d\n",serverScore,clientScore);
					     printf("\nServer Won the Game with score %d\n",serverScore);
					     sleep(1);
					     exit(0);
				     }	
				      	//sleep(3);
				        //fprintf(stderr, "Enter the $ sign to quit or a message for the server\n");
					//fgets(message, 254, stdin);
					     if(message[0] == '$'){
						         close(server);
							     exit(0);
						
			      		     }
					    
						
						memset(message,0,strlen(message));
						
					     snprintf(message,sizeof(message),"%d,%d",clientScore,rand);  
					     write(server, message, strlen(message));
					   if(clientScore>=10){
				   		printf("\nServer score: %d \nClient score: %d\n",serverScore,clientScore);
					        printf("\nClient Won the Game with score %d\n",clientScore);
					        sleep(1);
					        exit(0);
				     } 
				     printf("\nServer score: %d \nClient score: %d\n",serverScore,clientScore);
				     //printf("\n-------------------------------------------------------");
					       
			   }
}


