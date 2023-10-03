#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include "header.h"



int main(int argc, char* argv[]){

	int sockfd;

	struct addrinfo hints;
	
	struct addrinfo *servinfo=NULL;

	struct addrinfo *res=NULL;

	int rv;

	int numbytes;

	char buf[MAXBUFLEN];

	char s[INET6_ADDRSTRLEN];

	struct sockaddr_storage their_addr;

	socklen_t addr_len;
	
	int sent_bytes;
	
	int recv_bytes;

	if(argc != 2)// CHECKS IF args ARE VALID 
	{

		fprintf(stderr,"WARNING : Please enter the ip\n");

		exit(1);

	}
	
 
	char *server = argv[1]; /* server address */

	/* CLIENT CONFIGURATION */

	memset(&hints, INT, sizeof hints);   

	hints.ai_family = AF_UNSPEC; /*Allow IPV4 or IPV6*/

	hints.ai_socktype = SOCK_DGRAM;/*Datagram socket (udp)*/
	

	if((rv = getaddrinfo(server, SERVERPORT, &hints, &servinfo)) != 0){

		fprintf(stderr, "INFO : CLIENT:  getaddrinfo: %s\n", gai_strerror(rv));

		exit(EXIT_FAILURE);

	}

	/* getaddrinfo() returns a list of address structures. Try each address until we successfully bind*/

	for(res = servinfo; res != NULL; res =res->ai_next) {

		if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){

			perror("INFO : CLIENT: socket");

			continue;

		}

		break;/*sucess*/

	}

	if(res == NULL){ /*No address succeeded*/

		fprintf(stderr, "INFO : CLIENT: failed to bind socket\n");

		exit(EXIT_FAILURE);

	}
	
	char message[30]=" Client is ready for play ";
		
	if((numbytes = sendto(sockfd, message, strlen(message), INT, res->ai_addr, res->ai_addrlen))==-1){


			perror("INFO : CLIENT: sendto");

			exit(EXIT_FAILURE);

	}
	printf("CLIENT: Number Bytes sent to server %d\n", numbytes);
	
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , INT, (struct sockaddr *)&their_addr, &addr_len)) == -1) {

				perror("INFO : CLIENT: recvfrom");

				exit(EXIT_FAILURE);

	}
	sleep(1);
	printf("%s\n",buf);
	sleep(1);
	printf("Creating table for gameplay\n");
	//srand(time(NULL));
	int *user_num=random_create();
	int **user_table=create_table(user_num);
	sleep(1);
	print_table(user_table);
	while (true)
	{
		sleep(1);
		printf("Waiting for opponent choice....\n");
		memset(buf, 0, sizeof(buf));
		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , INT, (struct sockaddr *)&their_addr, &addr_len)) == -1) {

				perror("INFO : CLIENT: recvfrom");

				exit(EXIT_FAILURE);
		}
		sleep(1);
	    int choice = atoi(buf);
		if(choice ==100)
		{
			sleep(1);
			printf("--------SORRY YOU LOST----------\n");
			break;
		}
		sleep(1);
		printf("The opponnent choice : %d\n",choice);
		
		strike_num(choice,user_num);
		user_table=create_table(user_num);
		sleep(1);
		print_table(user_table);
		int user_check=check_points(user_table);
		if(user_check>4)
		{
			sleep(1);
			printf("\n--------------YOU WON!!-------------\n");
			memset(buf, 0, sizeof(buf));
			sprintf(buf,"%d",200);
			if((numbytes = sendto(sockfd, buf, strlen(buf), INT, res->ai_addr, res->ai_addrlen))==-1){
				
				perror("INFO : CLIENT: sendto");
				exit(EXIT_FAILURE);
			}
			break;
		}
		while(true)
		{
			int select=0;
			printf("YOUR CHOICE : ");
			scanf("%d",&select);
			int check = verify_choice(select,user_num);
			if(check ==1)
			{
				strike_num(select,user_num);
				user_table=create_table(user_num);
				sleep(1);
				print_table(user_table);
				int user_check1=check_points(user_table);
				if(user_check1>4)
				{
					sleep(1);
					printf("\n------------YOU WON!!--------------\n");
					memset(buf, 0, sizeof(buf));
					sprintf(buf,"%d",200);
					if((numbytes = sendto(sockfd, buf, strlen(buf), INT, res->ai_addr, res->ai_addrlen))==-1){
				
						perror("INFO : CLIENT: sendto");
						exit(EXIT_FAILURE);
					}
				    exit(EXIT_SUCCESS);
				}
				memset(buf, 0, sizeof(buf));
				sprintf(buf,"%d",select);
				if((numbytes = sendto(sockfd, buf, strlen(buf), INT, res->ai_addr, res->ai_addrlen))==-1){
				
					perror("INFO : CLIENT: sendto");
					exit(EXIT_FAILURE);
				}
				sleep(1);
				printf("YOUR CHOICE IS SENT...\n");
				break;
			}
			else{
				sleep(1);
				printf("Please choose the correct number\n");
			}
		}
			
	}
		
	
	freeaddrinfo(servinfo);

	close(sockfd);
}
