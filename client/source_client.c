#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "header.h"



void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {

		return &(((struct sockaddr_in*)sa)->sin_addr);

	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
	
int * random_create()
{
	int *user_num;
	user_num=(int *)malloc(26*sizeof(int));
	int r,temp;
	for (int i=1;i<=25;i++)
	{
		user_num[i-1]=i;
	}
	for (int i=25-1;i>0;i--)
	{
		r=rand() %i;
		temp=user_num[i];
		user_num[i]=user_num[r];
		user_num[r]=temp;	
	}
	return user_num;
}

int **create_table(int *arr)
{
	int **table=(int**)malloc(5* sizeof(int *)) ;
	for(int i=0;i<5;i++)
	{
		table[i]=(int *)malloc(5* sizeof(int ));
	}
	int index=0;
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			table[i][j]=arr[index];
			index++;
		}
	}
	return table;
}

int verify_choice(int num,int *arr)
{
	if (num<1 || num>25)
	{
		return 0;
	}
	for(int i=0;i<25;i++)
	{
		if(num==arr[i])
		{
			return 1;
		}
	}
	return 0;
}

void print_table(int **arr)
{
	for(int i=0;i<5;i+=1)
	{
		printf("+----+----+----+----+----+\n");
		printf("| ");
		for(int j=0;j<5;j++)
		{
			if (arr[i][j]!=0)
			{
				printf("%02d | ",arr[i][j]);
			}
			else
			{
				printf("\033[1;31m");
				printf("%02d",arr[i][j]);
				printf("\033[0m");
				printf(" | ");
			}
		}
		printf("\n");
		if(i==4)
		{
			printf("+----+----+----+----+----+\n");
		}
	}

}

void strike_num(int num,int *arr)
{
	for(int i=0;i<25;i++)
	{
		if (num==arr[i])
		{
			arr[i]=0;
		}
	}
}

int check_points(int **arr)
{
	int points=0;
	for(int i=0;i<5;i++)
	{
		int flag=0;
		for(int j=0;j<5;j++)
		{
			if(arr[i][j]!=0)
			{
				flag=1;
				break;
			}
		}
		if (flag==0)
		{
			points+=1;
		}
	}

	for(int i=0;i<5;i++)
	{
		int flag=0;
		for(int j=0;j<5;j++)
		{
			if(arr[j][i]!=0)
			{
				flag=1;
				break;
			}
		}
		if (flag==0)
		{
			points+=1;
		}
	}
	if (arr[0][0]==0 && arr[1][1]==0 && arr[2][2]==0 && arr[3][3]==0 && arr[4][4]==0)
	{
		points+=1;
	}
	if (arr[0][4]==0 && arr[1][3]==0 && arr[2][2]==0 && arr[3][1]==0 && arr[4][0]==0)
	{
		points+=1;
	}
	return points;
}
