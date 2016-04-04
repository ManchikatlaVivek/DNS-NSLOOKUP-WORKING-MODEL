#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>

struct rootrecord{
	char dextension[20];
	int tldport;
	char ipadd[16];
};

#define ip "127.0.0.1"
#define portno 7777
#define clientportno 6666
void main()
{
	struct sockaddr_in rootserver,rootclient;
	int rootfd,r,i,j;
	char buffer[512],ch;
	int len_buffer = 512;
	int len_rootserver = sizeof(rootserver);
	char extension[10];
	char extensioncopy[10];
	int temp,flag =0;
	int resultport[1];
	int sent;
	char resultip[16];
	struct rootrecord record;	

	if( (rootfd = socket(AF_INET,SOCK_DGRAM,0) ) == -1)
	{
		perror("socket");
		exit(-1);
	}


	rootclient.sin_family = AF_INET;
	rootclient.sin_port = htons(clientportno);
	rootclient.sin_addr.s_addr = inet_addr(ip);
	memset( &(rootclient.sin_zero),'\0',8 );
	

	rootserver.sin_family = AF_INET;
	rootserver.sin_port = htons(portno);
	rootserver.sin_addr.s_addr = inet_addr(ip);
	memset( &(rootserver.sin_zero),'\0',8 );


	if( (bind(rootfd, (struct sockaddr *)&rootserver, len_rootserver) ) == -1 )
	{
		perror("bind");
		exit(-1);
	}

	r = recvfrom(rootfd,buffer,len_buffer,0,(struct sockaddr *)&rootclient, &len_rootserver );
	buffer[r]='\0';
	printf("Hostname from localserver: %s\n",buffer);
	printf("%d bytes received\n",r);

	for(i=r-1,j=0;i>=0;i--)
	{
		if(buffer[i]== '.')
			break;
		else
		{
			extensioncopy[j]= buffer[i];
			j++;
		}
	}
	for(i=0;i<j;i++)
	{
		extension[i]=extensioncopy[j-1-i];
	}
	extension[j]='\0';
	
	printf("Extension is :%s\n",extension);
	FILE *fp;

	fp = fopen("rootdnsfile.txt","r");
	if( (ch = getc(fp)) == EOF )
	{
		printf("File is Empty Add the tld portnumbers into it\n");
	}
	else
	{
		rewind(fp);
		while( !(feof(fp)) )
		{
			fscanf(fp,"%s %s %d",record.dextension,record.ipadd,&record.tldport);
			// printf("%s %d\n",record.dextension,record.tldport );
			// temp = strcmp(record.dextension,extension);
			temp=0;
			for(i=0;i<j;i++)
			{
				if( record.dextension[i] == extension[i] )
					temp++;
			}
			if(temp == j)
			{
				flag =1;
				resultport[0] = record.tldport;
				strcpy(resultip,record.ipadd);
				printf("%d\n",resultport[0]);
				printf("%s\n",resultip);
			}
		}
		if(flag == 0)
		{
			printf("No TLD dns server found for the input...!\n");
			resultport[0]=4567;
			sent = sendto(rootfd,resultport,sizeof(int),0, (struct sockaddr *)&rootclient, sizeof(rootclient));
			printf("%d bytes sent\n", sent);
			char resultip1[]="1.0.0.0";
			sent = sendto(rootfd,resultip1,sizeof(resultip1),0, (struct sockaddr *)&rootclient, sizeof(rootclient));
			printf("%d bytes sent\n", sent);
			fclose(fp);
			exit(-1);
		}
	}
	fclose(fp);

	sent = sendto(rootfd,resultport,sizeof(int),0, (struct sockaddr *)&rootclient, sizeof(rootclient));
	printf("%d bytes sent\n", sent);

	sent = sendto(rootfd,resultip,sizeof(resultip),0, (struct sockaddr *)&rootclient, sizeof(rootclient));
	printf("%d bytes sent\n", sent);

	close(rootfd);



}
