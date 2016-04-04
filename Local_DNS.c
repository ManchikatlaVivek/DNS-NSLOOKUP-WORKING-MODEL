#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>

#define ip "127.0.0.1"
#define portno 6666
#define rootportno 7777
int qmsg[1];
struct dnsrecord
{
	char name[100];
	char value[16];
};
struct localtldcache{
char lext[100];
char ltldip[16];
int 	ltldport[1];
};
char ipaddress[16];
int i,flag=0;
int r,sent;
void main()
{
struct localtldcache cache1;
	qmsg[0]=0;
//==============================================
	int newfd;	
	struct sockaddr_in server,client;
	int ser_len = sizeof(server);
	char buffer[512];
	int len_buffer = 512;
	struct dnsrecord cache;
	char ch;
	int temp;
char a1[100];

//==============================================
	struct sockaddr_in rootserver,rootclient;
	int rootfd;
	int extension[1];
	int rootserver_length = sizeof(rootserver);

//==============================================================
int tldfd;
struct sockaddr_in tldserver;
int tldserver_length = sizeof(tldserver);
int authportnumber[1];

//==========================================================
	if( (newfd = socket(AF_INET,SOCK_DGRAM,0) ) == -1 )
	{
		perror("socket");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(portno);
	server.sin_addr.s_addr = inet_addr(ip);
	memset( &(server.sin_zero),'\0',8);

				struct sockaddr_in clientserver;
				clientserver.sin_family = AF_INET;
				clientserver.sin_port = htons(12222);
				clientserver.sin_addr.s_addr = inet_addr(ip);
				memset( &(clientserver.sin_zero),'\0',8 );

	

	if( (bind(newfd,(struct sockaddr *)&server,sizeof(server) ) ) ==-1 )
	{
		perror("bind");
		exit(-1);
	}

	for(i=0;i<512;i++)
	{
		buffer[i]='\0';
	}
	r = recvfrom(newfd,buffer,len_buffer,0,(struct sockaddr *)&client,&ser_len  );
	buffer[r]='\0';
	printf("Buffer Message is : %s",buffer);
	printf("\nBytes received is %d\n",r);
	for(i=9;i<r;i++)
	{	
		buffer[i-9]=buffer[i]; 
		buffer[i]='\0';
	}
	printf("Domain name:");
	for(i=0;i<r-9;i++)
	{
		printf("%c",buffer[i] );
	}
	printf("\n");
	// to extract the extension and store it in the com[] array;
	strcpy(a1,buffer);
	int ttl = strlen(buffer);
	a1[ttl]='\0';
	
						int j1=0;
							
							
							char ch1;
							int i1,k1;
								j1 = strlen(buffer);
								a1[j1]='\0';
								puts(a1);
								char com1[10];
								char moc1[10];
								int r11 = strlen(a1);
								for(i1=r11-1,j1=0;i1>=0;i1--)
								{
									if(a1[i1]== '.')
										break;
									else
									{
										moc1[j1]= a1[i1];
										j1++;
									}
								}
								for(i1=0;i1<j1;i1++)
									{
										com1[i1]=moc1[j1-1-i1];
									}
									com1[j1]='\0';
									puts(com1);	
						
	FILE *fp;
	
	fp = fopen("localcache.txt","r");
	if( (ch= fgetc(fp)) == EOF){
		printf("Nothing Present in Cache\n");
	}
	else
	{	

		//============Checking in the Cache Memory of Local Server================
		// printf("--------\t---------\nHostname\tIpaddress\n--------\t---------\n");
		rewind(fp);
		while(!feof(fp))
			{
				fscanf(fp,"%s %s",cache.name,cache.value);
				// printf("%s\t%s\n",cache.name,cache.value);
				
				temp = strcmp(cache.name,buffer);
				if(temp == 0)
				{
					// printf("%s\t%s\n",cache.name,cache.value);
					strcpy(ipaddress,cache.value);
					flag = 1;
				}
			}

			if(flag == 1) //it is present in the local server cache..
			{
				int lr = 16;
				printf("IP ADDRESS is :%s\n",ipaddress);
				int r1 = sendto(newfd,ipaddress,lr,0,(struct sockaddr *)&clientserver,ser_len );
				printf("%d bytes sent\n",r1);
				qmsg[0]=0;
				r1 = sendto(newfd,qmsg,sizeof(int),0,(struct sockaddr *)&clientserver,ser_len );
				printf("%d bytes sent\n",r1);
				
				close(newfd);
				exit(-1);
			}

	}
			if(flag == 0)// NOt present in the local server cache...
			{
				printf("Not Present in Cache Memory\n...............\nsending Request to Root Server\n.....................\n");
				fclose(fp);
				
				// check the tld cache of the local server.
				
					FILE *fpp;
					int cacheflag=0;
					fpp = fopen("localtldcache.txt","r");
					if(!fpp)
					{
						printf("Error opening the localtldcache file\n");
										
					}	
					char chh;
					if( (chh = getc(fpp)) == EOF)
					{
						printf("LocalTldCache contents are empty....\n");
					}
					else
					{
					printf("localtld cache contents\n");
						while(!feof(fpp))
						{
								fscanf(fpp,"%s %s %d",cache1.lext,cache1.ltldip,cache1.ltldport);
								printf("%s %s %d\n",cache1.lext,cache1.ltldip,cache1.ltldport[0]);
								int comp;
								comp = strcmp(com1,cache1.lext);
								if(comp == 0)
								{
								 cacheflag=1;
								 break;
								}
						}
						
					}		
					fclose(fpp);
					if(cacheflag == 1) // tld ip and port is present in the local-tld-server cache...
					{
					extension[0]=cache1.ltldport[0];
					char tldip[16];
					strcpy(tldip,cache1.ltldip);
					int tkt;
					tkt = strlen(cache1.ltldip);
					tldip[tkt]='\0';
							
								if( (tldfd = socket(AF_INET,SOCK_DGRAM,0) )== -1 )
									{
										perror("socket");
										exit(-1);
									}

				tldserver.sin_family = AF_INET;
				tldserver.sin_port = htons(cache1.ltldport[0]);
				tldserver.sin_addr.s_addr = inet_addr(tldip);
				memset( &(tldserver.sin_zero),'\0',8 );
				printf("%s\n",buffer );
				
				if( (sent = sendto(tldfd,buffer,strlen(buffer),0,(struct sockaddr *)&tldserver,sizeof(tldserver) ) ) ==-1)
				{
					perror("send");
					exit(-1);
				}
				printf("%d bytes sent\n", sent);

				if( (r = recvfrom(tldfd,authportnumber,sizeof(int),0, (struct sockaddr *)&tldserver, &tldserver_length)) ==-1)
				{
					perror("recvfrom");
					printf("No entry found in the Tld server\n");
					exit(-1);
				}
				printf("%d port number\n", authportnumber[0]);
				printf("%d bytes received\n",r );
				char authportip[16];
				if( (r = recvfrom(tldfd,authportip,sizeof(authportip),0, (struct sockaddr *)&tldserver, &tldserver_length)) ==-1)
				{
					perror("recvfrom");
					printf("No entry found in the Tld server\n");
					exit(-1);
				}
				authportip[r]='\0';
				printf("%s port IP adddre\n", authportip);
				printf("%d bytes received\n",r );


				if(authportnumber[0]==4567)
				{
					char ipbuffer1[]="1.0.0.0";
					int r2 = sendto(newfd,ipbuffer1,strlen(ipbuffer1),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes received\n",r2);
					qmsg[0]=qmsg[0]+3;
					int r1 = sendto(newfd,qmsg,sizeof(int),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes sent\n",r1);
				
					close(newfd);
					exit(-1);
				}
				qmsg[0]=qmsg[0]+3;
								printf("Number of query messages between tld and local server is %d\n",qmsg[0]);
				close(tldfd);
	/////////////////////////////////////////================////////////////////////////////////////
	
	
	int authfd;
				struct sockaddr_in authserver;
				char ipbuffer[16];
				int authserver_length = sizeof(authserver);
				if( (authfd = socket(AF_INET,SOCK_DGRAM,0) )== -1 )
				{
					perror("socket");
					exit(-1);
				}

				authserver.sin_family = AF_INET;
				authserver.sin_port = htons(authportnumber[0]);
				authserver.sin_addr.s_addr = inet_addr(authportip);
				memset( &(authserver.sin_zero),'\0',8 );
				printf("%s\n",buffer );
				
				if( (sent = sendto(authfd,buffer,strlen(buffer),0,(struct sockaddr *)&authserver,sizeof(authserver) ) ) ==-1)
				{
					perror("send");
					exit(-1);
				}
				printf("%d bytes sent\n", sent);
				int bufflen = 512;
				if( (r = recvfrom(authfd,ipbuffer,bufflen,0, (struct sockaddr *)&authserver, &authserver_length)) == -1)
				{
					perror("recvfrom");
					printf("No Entry Found in the Authoritative Server\n");
					exit(-1);
				}
				ipbuffer[r]='\0';
				char errip[]="1.0.0.0";
				if((strcmp(errip,ipbuffer)) == 0)
				{
					int r1 = sendto(newfd,errip,strlen(errip),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes received\n",r1);
					qmsg[0]=qmsg[0]+2;
					r1 = sendto(newfd,qmsg,sizeof(int),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes sent\n",r1);
				
					close(newfd);
				exit(-1);
				}
				
				printf("%s IP ADDRESS\n", ipbuffer);
				printf("%d bytes received\n",r );
				qmsg[0]=qmsg[0]+2;
								printf("Number of query messages between author and local server is %d\n",qmsg[0]);
				close(authfd);
				
				int r1 = sendto(newfd,ipbuffer,r,0,(struct sockaddr *)&clientserver,ser_len );
				printf("%d bytes received\n",r1);
				
					r1 = sendto(newfd,qmsg,sizeof(int),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes sent\n",r1);
					
				close(newfd);
				FILE *fp;
				fp = fopen("localcache.txt","a");
				fprintf(fp," %s %s",buffer,ipbuffer);
				fclose(fp);

	
	
	
	/////////////////////////////==========================////////////////////////////////////
					
					}
					else // not there in the localtldcache
					{
					 
			
				//================================Done checking in Cache Memory=======================
				//================================Contacting Root Server ==============================

				
			if( (rootfd = socket(AF_INET,SOCK_DGRAM,0) ) == -1 )
				{
					perror("socket");
					exit(-1);
				}


				rootserver.sin_family= AF_INET;
				rootserver.sin_port= htons(rootportno);
				rootserver.sin_addr.s_addr = inet_addr(ip);
				memset( &(rootserver.sin_zero),'\0',8);

				if( (sent = sendto(rootfd,buffer,(r-9),0, (struct sockaddr *)&rootserver, sizeof(rootserver) ) ) == -1)
				{
					perror("send");
					exit(-1);
				}
				printf("%d bytes sent\n",sent );

				if( (r = recvfrom(rootfd,extension,sizeof(int),0, (struct sockaddr *)&rootclient,&rootserver_length)) == -1)
				{
					perror("recvfrom");
					printf("No Entry for the Domain in the Root Server\n");
					exit(-1);
				}
			
				printf("extension is : %d\n",extension[0] );
				printf("%d bytes received\n", r );
				char tldip[16];
				if( (r = recvfrom(rootfd,tldip,sizeof(tldip),0, (struct sockaddr *)&rootclient,&rootserver_length)) == -1)
				{
					perror("recvfrom");
					printf("No Entry for the Domain in the Root Server\n");
					exit(-1);
				}
				tldip[r]='\0';
				printf("tld ip is : %s\n",tldip );
				printf("%d bytes received\n", r );


				if(extension[0]==4567)
				{	
					char ipbuffer1[]="1.0.0.0";
					int r2 = sendto(newfd,ipbuffer1,strlen(ipbuffer1),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes received\n",r2);
					qmsg[0]=qmsg[0]+3; //1 send and 2 recv between local and root server.
					int r1 = sendto(newfd,qmsg,sizeof(int),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes sent\n",r1);
				
					close(newfd);
					exit(-1);
				}	
				
				FILE *fpp;
				fpp = fopen("localtldcache.txt","a");
				fprintf(fpp," %s %s %d",com1,tldip,extension[0]); //extension,ip,portnumber;
				fclose(fpp);
				qmsg[0]=qmsg[0]+3;
				printf("Number of query messages between root and local server is %d\n",qmsg[0]);
				close(rootfd);
		//========================================================================================
				//===============portnumber of tld is in extension[0]=============================

				//=================================================TLD SERVER==========================

				if( (tldfd = socket(AF_INET,SOCK_DGRAM,0) )== -1 )
				{
					perror("socket");
					exit(-1);
				}

				tldserver.sin_family = AF_INET;
				tldserver.sin_port = htons(extension[0]);
				tldserver.sin_addr.s_addr = inet_addr(tldip);
				memset( &(tldserver.sin_zero),'\0',8 );
				printf("%s\n",buffer );
				
				if( (sent = sendto(tldfd,buffer,strlen(buffer),0,(struct sockaddr *)&tldserver,sizeof(tldserver) ) ) ==-1)
				{
					perror("send");
					exit(-1);
				}
				printf("%d bytes sent\n", sent);

				if( (r = recvfrom(tldfd,authportnumber,sizeof(int),0, (struct sockaddr *)&tldserver, &tldserver_length)) ==-1)
				{
					perror("recvfrom");
					printf("No entry found in the Tld server\n");
					exit(-1);
				}
				printf("%d port number\n", authportnumber[0]);
				printf("%d bytes received\n",r );
				char authportip[16];
				if( (r = recvfrom(tldfd,authportip,sizeof(authportip),0, (struct sockaddr *)&tldserver, &tldserver_length)) ==-1)
				{
					perror("recvfrom");
					printf("No entry found in the Tld server\n");
					exit(-1);
				}
				authportip[r]='\0';
				printf("%s port IP adddre\n", authportip);
				printf("%d bytes received\n",r );


				if(authportnumber[0]==4567)
				{
					char ipbuffer1[]="1.0.0.0";
					int r2 = sendto(newfd,ipbuffer1,strlen(ipbuffer1),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes received\n",r2);
					qmsg[0]=qmsg[0]+3;
					int r1 = sendto(newfd,qmsg,sizeof(int),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes sent\n",r1);
				
					close(newfd);
					exit(-1);
				}
				qmsg[0]=qmsg[0]+3;
								printf("Number of query messages between tld and local server is %d\n",qmsg[0]);
				close(tldfd);

	//================================================================================================================
				// ==================AUTH SERVER=========================
				int authfd;
				struct sockaddr_in authserver;
				char ipbuffer[16];
				int authserver_length = sizeof(authserver);
				if( (authfd = socket(AF_INET,SOCK_DGRAM,0) )== -1 )
				{
					perror("socket");
					exit(-1);
				}

				authserver.sin_family = AF_INET;
				authserver.sin_port = htons(authportnumber[0]);
				authserver.sin_addr.s_addr = inet_addr(authportip);
				memset( &(authserver.sin_zero),'\0',8 );
				printf("%s\n",buffer );
				
				if( (sent = sendto(authfd,buffer,strlen(buffer),0,(struct sockaddr *)&authserver,sizeof(authserver) ) ) ==-1)
				{
					perror("send");
					exit(-1);
				}
				printf("%d bytes sent\n", sent);
				int bufflen = 512;
				if( (r = recvfrom(authfd,ipbuffer,bufflen,0, (struct sockaddr *)&authserver, &authserver_length)) == -1)
				{
					perror("recvfrom");
					printf("No Entry Found in the Authoritative Server\n");
					exit(-1);
				}
				ipbuffer[r]='\0';
				char errip[]="1.0.0.0";
				if((strcmp(errip,ipbuffer)) == 0)
				{
					int r1 = sendto(newfd,errip,strlen(errip),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes received\n",r1);
					qmsg[0]=qmsg[0]+2;
					r1 = sendto(newfd,qmsg,sizeof(int),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes sent\n",r1);
				
					close(newfd);
				exit(-1);
				}
				
				printf("%s IP ADDRESS\n", ipbuffer);
				printf("%d bytes received\n",r );
				qmsg[0]=qmsg[0]+2;
								printf("Number of query messages between author and local server is %d\n",qmsg[0]);
				close(authfd);
				
				int r1 = sendto(newfd,ipbuffer,r,0,(struct sockaddr *)&clientserver,ser_len );
				printf("%d bytes received\n",r1);
				
					r1 = sendto(newfd,qmsg,sizeof(int),0,(struct sockaddr *)&clientserver,ser_len );
					printf("%d bytes sent\n",r1);
					
				close(newfd);
				FILE *fp;
				fp = fopen("localcache.txt","a");
				fprintf(fp," %s %s",buffer,ipbuffer);
				fclose(fp);

			}	
			
			
			}
			
			
			
	}
		
		




