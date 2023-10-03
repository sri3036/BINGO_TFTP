#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <time.h>
#include "header.h"
#define INT 0
#define FLAG 1
#define DI 2
#define BYTES_ELIMINATE 4
#define BLOCK_SIZE 3
#define ACK_SIZE 10
#define PENTA 5



int main(void){

	//	while(1){  /*loop for accepting multiple clients consecutively*/

	int sockfd;

	struct addrinfo hints, *servinfo, *p;

	int rv;

	int numbytes;

	struct sockaddr_storage their_addr;

	char buf[MAXBUFLEN];

	socklen_t addr_len;

	char s[INET6_ADDRSTRLEN];
	
	int recv_choice;

	/*configuration of the server*/

	memset(&hints, INT, sizeof hints);

	hints.ai_family = AF_UNSPEC;/*Set to IPV4*/

	hints.ai_socktype = SOCK_DGRAM;/*For the UDP connection*/ 

	hints.ai_flags = AI_PASSIVE; /*Assign the client IP with same connection */

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {

		fprintf(stderr, "INFO : SERVER: getaddrinfo: %s\n", gai_strerror(rv));

		exit(EXIT_FAILURE);

	}
	/*loop through all the inputs and bind the socket*/
	for(p = servinfo; p != NULL; p = p->ai_next) {

		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {

			perror("INFO : SERVER: socket");

			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {

			close(sockfd);

			perror("INFO : SERVER: bind");

			continue;

		}

		break;
	}

	if (p == NULL) {

		fprintf(stderr, "INFO : SERVER: failed to bind socket\n");

		exit(EXIT_FAILURE);

	}

	freeaddrinfo(servinfo);

	printf("SERVER: waiting for the client....\n");
	
	addr_len = sizeof their_addr;
	
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , INT, (struct sockaddr *)&their_addr, &addr_len)) == -1) {

		perror("INFO : SERVER: recvfrom");

		exit(EXIT_FAILURE);

	}
	const char *ip=inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
	sleep(1);
	printf("SERVER: got request from %s\n", ip);
	sleep(1);
	printf("%s\n",buf);
	
	char t_msg [30]="Let's start the game";
	if((numbytes = sendto(sockfd, t_msg, strlen(t_msg), INT, (struct sockaddr *)&their_addr, addr_len)) == -1){

				perror("SERVER ACK: sendto");
				exit(EXIT_FAILURE);

	}
	sleep(1);
	printf("Num of bytes sent : %d\n",numbytes);
	sleep(1);
	printf("Creating table for gameplay\n");
	srand(time(NULL));
	int *sys_num=random_create();
	int **sys_table=create_table(sys_num);
	sleep(1);
	print_table(sys_table);
	sleep(1);
	printf("Please enter your choice b/w 1 to 25\n");
	while (true)
	{
		int choice=0;
		sleep(1);
		printf("Your choice : ");
		scanf("%d",&choice);
		int check = verify_choice(choice,sys_num);
		if (check == 1)
		{
			strike_num(choice,sys_num);
			sys_table=create_table(sys_num);
			int sys_check=check_points(sys_table);
			if(sys_check>4)
			{
				sleep(1);
				printf("\n---------YOU WON!!-----------\n");
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "%d", 100);
				if((numbytes = sendto(sockfd, buf, strlen(buf), INT, (struct sockaddr *)&their_addr, addr_len)) == -1){

				    perror("SERVER ACK: sendto");
				    exit(EXIT_FAILURE);
			    }
				break;
			}
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%d", choice);
			if((numbytes = sendto(sockfd, buf, strlen(buf), INT, (struct sockaddr *)&their_addr, addr_len)) == -1){

				perror("SERVER ACK: sendto");
				exit(EXIT_FAILURE);
			}
			sleep(1);
			print_table(sys_table);
			sleep(1);
			printf("\nYour choice is sent!....\n");
			
		    sleep(1);
			printf("Waiting for your opponent choice....\n");
			memset(buf, 0, sizeof(buf));
			if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , INT, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
				
				perror("INFO : SERVER: recvfrom");
				exit(EXIT_FAILURE);
			}
			sleep(1);
			printf("The received choice : %s\n",buf);
			int select=atoi(buf);
			if(select==200)
			{
				printf("\n------------SORRY YOU LOST--------------\n");
				break;
			}
			strike_num(select,sys_num);
			sys_table=create_table(sys_num);
			sleep(1);
			print_table(sys_table);
			int sys_check1=check_points(sys_table);
			if(sys_check1>4)
			{
				sleep(1);
				printf("\n---------YOU WON!!----------\n");
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "%d", 100);
				if((numbytes = sendto(sockfd, buf, strlen(buf), INT, (struct sockaddr *)&their_addr, addr_len)) == -1){

				    perror("SERVER ACK: sendto");
				    exit(EXIT_FAILURE);
			    }
				break;
			}
		}
	}
		
	close(sockfd);/*closing the connection*/
}
