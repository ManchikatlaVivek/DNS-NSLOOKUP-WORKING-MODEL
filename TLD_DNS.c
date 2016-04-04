#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
struct tldrecord{
	char domain[100];
	char ipadd[16];
	int authport;
	
};

#define ip "127.0.0.1"
#define iproot "127.0.0.23"
#define tldport 10000
#define localportno 6666

void main()
{
	int tldfd;
	int i,ctr=0,k;
	struct sockaddr_in tldserver,tldclient;
	int len_tldserver = sizeof(tldserver);
	int r,sent;
	int len_tldclient = sizeof(tldclient);
	int dotposition;
	char buffer[512];
	int len_buffer = 512;
	struct tldrecord record;
	char bufferdomain[100],ch;
	int authdnsportnumber[1];
	int temp,flag=0;
	char authdnsportip[16];
	if( (tldfd = socket(AF_INET,SOCK_DGRAM,0) ) == -1)
	{
		perror("socket");
		exit(-1);
	}

	
	tldserver.sin_family = AF_INET;
	tldserver.sin_port = htons(tldport);
	tldserver.sin_addr.s_addr = inet_addr(iproot);
	memset( &(tldserver.sin_zero),'\0',8 );

	tldclient.sin_family = AF_INET;
	tldclient.sin_port = htons(localportno);
	tldclient.sin_addr.s_addr = inet_addr(ip);
	memset( &(tldclient.sin_zero),'\0',8 );
	

	if( (bind(tldfd, (struct sockaddr *)&tldserver, len_tldserver) ) == -1 )
	{
		perror("bind");
		exit(-1);
	}

	r = recvfrom(tldfd,buffer,len_buffer,0, (struct sockaddr *)&tldclient, &len_tldclient );
	buffer[r]='\0';

	printf("%s\n",buffer );



	 k=strlen(buffer);
	 int tt=k;
	for(i=k-1;i>=0;i--)
	{
		if(buffer[i] == '.')
			ctr++;
		if(ctr == 2)
			break;
	}
	// printf("%d position \n",i );
	// the index of second is in i
	dotposition = i;
	
	printf("Received Domain Name : ");
	k=dotposition+1;
	for(i=k;i<tt;i++)
	{
		bufferdomain[i-k]=buffer[i];
		buffer[i]='\0';
	}
	bufferdomain[tt-k]='\0';
	puts(bufferdomain);

	FILE *fp ;
	fp = fopen("tldcom.txt","r");
	if( (ch = getc(fp) ) == EOF )
	{
		printf("No Entries in .Com \n");
	}
	else
	{
		rewind(fp);
		while(!feof(fp))
		{
			fscanf(fp,"%s %s %d",record.domain,record.ipadd,&record.authport);
			// printf("%s %d\n",record.domain,record.authport);
			temp = strcmp(record.domain,bufferdomain);
			if(temp == 0)
			{
				flag = 1;
				authdnsportnumber[0]=record.authport;
				strcpy(authdnsportip,record.ipadd);
				printf("%d authport \n",authdnsportnumber[0] );
				printf("%s authportip \n",authdnsportip);

			}

		}
		if(flag == 0)
		{
			printf("Authorative entry not found for the domain\n");
			authdnsportnumber[0]=4567;
			sent = sendto(tldfd,authdnsportnumber,sizeof(int),0,(struct sockaddr *)&tldclient,len_tldclient );
		printf("%d bytes sent\n",sent);
		char authdnsportip1[]="1.0.0.0";
		sent = sendto(tldfd,authdnsportip1,sizeof(authdnsportip1),0,(struct sockaddr *)&tldclient,len_tldclient );
		printf("%d bytes sent\n",sent);
		fclose(fp);
			fclose(fp);
			exit(-1);
		}

		sent = sendto(tldfd,authdnsportnumber,sizeof(int),0,(struct sockaddr *)&tldclient,len_tldclient );
		printf("%d bytes sent\n",sent);

		sent = sendto(tldfd,authdnsportip,sizeof(authdnsportip),0,(struct sockaddr *)&tldclient,len_tldclient );
		printf("%d bytes sent\n",sent);
		fclose(fp);
	}
	close(tldfd);




}