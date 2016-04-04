#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
struct alphabetarecord{
	char name[200];
	char value[16];
};

#define ip "127.0.0.1"
#define authip "127.0.0.57"
#define authportno 20001
#define localportno 6666

void main()
{
	int authfd;
	int sent,r;
	char buffer[512],ch;
	int len_buffer = 512;
	int temp,flag=0;
	struct alphabetarecord record;
	char ipbuffer[16];
	if( (authfd = socket(PF_INET,SOCK_DGRAM,0) ) == -1)
	{
		perror("socket");
		exit(-1);
	}

	struct sockaddr_in authserver,locserver;

	authserver.sin_family = AF_INET;
	authserver.sin_port = htons(authportno);
	authserver.sin_addr.s_addr = inet_addr(authip);
	memset( &(authserver.sin_zero),'\0',8 );
	int len_authserver = sizeof(authserver);

	locserver.sin_family = AF_INET;
	locserver.sin_port = htons(localportno);
	locserver.sin_addr.s_addr = inet_addr(ip);
	memset( &(locserver.sin_zero),'\0',8 );
	int len_locserver = sizeof(locserver);

	if( (bind(authfd, (struct sockaddr *)&authserver, len_authserver) ) == -1 )
	{
		perror("bind");
		exit(-1);
	}

	r = recvfrom(authfd,buffer,len_buffer,0, (struct sockaddr *)&locserver,&len_locserver );
	buffer[r]='\0';

	printf("%s",buffer);
	printf("%d bytes received\n", r);

	FILE *fp;
	fp = fopen("authin.txt","r");
	if((ch = getc(fp)) == EOF)
	{
		printf("No Entry in the Authorative ALPHABETA server\n");
	}
	else
	{
		rewind(fp);
		while(!feof(fp))
		{

			fscanf(fp,"%s %s",record.name,record.value);
			 printf("%s %s\n",record.name,record.value );
			temp = strcmp(record.name,buffer);
			if(temp == 0)
			{
				
				strcpy(ipbuffer,record.value);
				puts(record.value);
				flag =1;
				// printf("%d flagged value\n", flag);
			}
		}
	if(flag == 0)
	{
		printf("No Entry Found for the input\n");
		char ipbuffer1[]="1.0.0.0";
		r = sendto (authfd,ipbuffer1,strlen(ipbuffer1),0,(struct sockaddr *)&locserver, len_locserver );
		printf("%d bytes sent\n",r);
		fclose(fp);
		exit(-1);
	}

	
	int ll = strlen(ipbuffer);
	r = sendto (authfd,ipbuffer,ll,0,(struct sockaddr *)&locserver, len_locserver );
	printf("%d bytes sent\n",r);
	}
	fclose(fp);
	

}