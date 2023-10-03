#define MYPORT "4950" /* port to be opened on server*/
#define SERVERPORT "4950" /* the port users will be connecting to */
#define MAXBUFLEN 550 /* get sockaddr, IPv4 or IPv6:*/


#ifndef _HEADER_H
#define _HEADER_H

void *get_in_addr(struct sockaddr *sa);
int *random_create();
int **create_table(int *arr);
void print_table(int **arr);
int verify_choice(int num,int *arr);
void strike_num(int num,int *arr);
int check_points(int **arr);
#endif