#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <termios.h>
#include <fcntl.h>


int main (int argc, char*argv[]){
	int telnetSocket;
	struct sockaddr_in server;
	unsigned char buf[21];
	int len, i;
	int port=23;

	FILE *pf;
	char ID[256];
	char PASS[256];
	char BOARD[256];
	char CONTENT[256];
	char P[256];
	char buffer[512];
	char* pch;
	char* c;
	char* color_f;
	char* color_l;

	telnetSocket = socket(AF_INET, SOCK_STREAM,0);
	if(telnetSocket==-1){
		perror("socket");
		return 1;
	}
	server.sin_addr.s_addr = inet_addr("140.112.172.11");
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//connect to ptt
	if(connect(telnetSocket,(struct sockaddr*)&server, sizeof(server))<0){
		perror("connect");
		return 1;
	}

	if(argc != 2){
    	fprintf(stderr, "Usage: project2 [filename]\n");
    	exit(1);
  	}
	pf= fopen(argv[1],"r");
	strcpy(buffer,"");
	
	printf("Connect...\n");

	if(fgets(buffer,512,pf)!=NULL){
		if((pch = strstr (buffer, "<ID>"))!=NULL){
		c=strstr(buffer, "</ID>");
		if(c!=NULL){
			strcpy( ID, "" );
			strncpy(ID,buffer+4,(size_t)(c-buffer-4));
			ID[(size_t)(c-buffer-4)]='\0';
		}
		}
	}

	strcpy(buffer,"");
	if(fgets(buffer,512,pf)!=NULL){
		if((pch = strstr (buffer, "<PASS>"))!=NULL){
		c=strstr(buffer, "</PASS>");
		if(c!=NULL){
			strcpy( PASS, "" );
			strncpy(PASS,buffer+6,(size_t)(c-buffer-6));
			PASS[(size_t)(c-buffer-6)]='\0';
			///PASS[(size_t)(c-buffer-5)]='\n';
			//printf("%s",PASS );
		}
	}
	}
	//puts("Connected...\n");
	//log in 
	printf("Log in...\n");
	write(telnetSocket, ID,strlen(ID));
	write(telnetSocket, "\r\n",2);
	write(telnetSocket, PASS,strlen(PASS));
	write(telnetSocket, "\r\n",2);
	write(telnetSocket, "\x003" ,1);
	sleep(5);
	write(telnetSocket, "\x003" ,1);
	write(telnetSocket,"y\r\n",3);
	sleep(10);
	write(telnetSocket, "\x003" ,1);
	sleep(2);
	printf("Log in...successful\n");

	

	while((fgets(buffer,512,pf)!=NULL)){
		
		if((pch = strstr (buffer, "<BOARD>"))!=NULL){
			if((c=strstr(buffer, "</BOARD>"))!=NULL){
				strncpy(BOARD,buffer+7,(size_t)(c-buffer-7));
				BOARD[(size_t)(c-buffer-7)]='\0';
				//printf("%s\n",BOARD );
			}
			write(telnetSocket,"s",1);
			write(telnetSocket, BOARD,strlen(BOARD));
			write(telnetSocket,"\r\n",2);
			write(telnetSocket, "\x003" ,1);
			write(telnetSocket, "\x003" ,1);
			printf("Enter board %s...\n",BOARD);
			sleep(1);

		}
		
		else if((pch = strstr (buffer, "<P>"))!=NULL){
			if((c=strstr(buffer, "</P>"))!=NULL){
				strncpy(P,buffer+3,(size_t)(c-buffer-3));
				P[(size_t)(c-buffer-3)]='\0';
				//printf("%s\n",P );
			}
			write(telnetSocket, "\x010" ,1);
    		write(telnetSocket, "\r\n",2);
    		write(telnetSocket, P,strlen(P));
    		write(telnetSocket,"\r\n",2);
    		printf("Enter title %s...\n",P);
		}
		else if((pch = strstr (buffer, "<CONTENT>"))!=NULL){
			printf("Enter content...\n");
		if((c=strstr(buffer, "</CONTENT>"))==NULL){
			if((color_f=strstr(buffer,"<COLOR_R>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_R>"))!=NULL){
					strncpy(CONTENT,buffer+9,color_f-buffer-9);
					CONTENT[color_f-buffer-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//printf("%s\n",CONTENT );
					strncpy(CONTENT,color_f+9,color_l-color_f-9);
					CONTENT[color_l-color_f-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1r\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
					CONTENT[strlen(CONTENT)]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1w\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_Y>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_Y>"))!=NULL){
					strncpy(CONTENT,buffer+9,color_f-buffer-9);
					CONTENT[color_f-buffer-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//printf("%s\n",CONTENT );
					strncpy(CONTENT,color_f+9,color_l-color_f-9);
					CONTENT[color_l-color_f-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1y\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
					CONTENT[strlen(CONTENT)]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1w\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_B>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_B>"))!=NULL){
					strncpy(CONTENT,buffer+9,color_f-buffer-9);
					CONTENT[color_f-buffer-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//printf("%s\n",CONTENT );
					strncpy(CONTENT,color_f+9,color_l-color_f-9);
					CONTENT[color_l-color_f-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1b\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
					CONTENT[strlen(CONTENT)]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1w\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_G>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_G>"))!=NULL){
					strncpy(CONTENT,buffer+9,color_f-buffer-9);
					CONTENT[color_f-buffer-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//printf("%s\n",CONTENT );
					strncpy(CONTENT,color_f+9,color_l-color_f-9);
					CONTENT[color_l-color_f-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1g\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
					CONTENT[strlen(CONTENT)]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1w\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_L>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_L>"))!=NULL){
					strncpy(CONTENT,buffer+9,color_f-buffer-9);
					CONTENT[color_f-buffer-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//printf("%s\n",CONTENT );
					strncpy(CONTENT,color_f+9,color_l-color_f-9);
					CONTENT[color_l-color_f-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1l\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
					CONTENT[strlen(CONTENT)]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1w\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_P>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_P>"))!=NULL){
					strncpy(CONTENT,buffer+9,color_f-buffer-9);
					CONTENT[color_f-buffer-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//printf("%s\n",CONTENT );
					strncpy(CONTENT,color_f+9,color_l-color_f-9);
					CONTENT[color_l-color_f-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1p\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
					CONTENT[strlen(CONTENT)]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1w\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_C>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_C>"))!=NULL){
					strncpy(CONTENT,buffer+9,color_f-buffer-9);
					CONTENT[color_f-buffer-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//printf("%s\n",CONTENT );
					strncpy(CONTENT,color_f+9,color_l-color_f-9);
					CONTENT[color_l-color_f-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1c\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
					CONTENT[strlen(CONTENT)]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1w\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_W>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_W>"))!=NULL){
					strncpy(CONTENT,buffer+9,color_f-buffer-9);
					CONTENT[color_f-buffer-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//printf("%s\n",CONTENT );
					strncpy(CONTENT,color_f+9,color_l-color_f-9);
					CONTENT[color_l-color_f-9]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1w\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
					CONTENT[strlen(CONTENT)]='\0';
					//printf("%s\n",CONTENT);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"\x016",1);
					write(telnetSocket,"\x003",1);
					write(telnetSocket,"1w\r\n",4);
					write(telnetSocket,CONTENT,strlen(CONTENT));
					//write(telnetSocket,"\r\n",2);
				}
			}
			else{
				strncpy(CONTENT,buffer+9,sizeof(buffer)-9);
				CONTENT[sizeof(buffer)-9]='\0';
				//printf("%s",CONTENT );
				write(telnetSocket,"\x016",1);
				write(telnetSocket,"\x003",1);
				write(telnetSocket,"1w\r\n",4);
				write(telnetSocket,CONTENT,strlen(CONTENT));
				//write(telnetSocket,"\r\n",2);
			}

			
			while(fgets(buffer,512,pf)!=NULL){
				if((pch = strstr (buffer, "</CONTENT>"))!=NULL){
					//printf("%s",CONTENT );
					if((color_f=strstr(buffer,"<COLOR_R>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_R>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1r\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,pch-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
							//printf("%s\n",CONTENT );
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_Y>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_Y>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1y\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,pch-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
							//printf("%s\n",CONTENT );
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_B>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_B>"))!=NULL){
						strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1b\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,pch-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
							//printf("%s\n",CONTENT );
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_G>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_G>"))!=NULL){
						strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1r\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,pch-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1g\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
							//printf("%s\n",CONTENT );
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_L>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_L>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1l\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,pch-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
							//printf("%s\n",CONTENT );
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_P>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_P>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1p\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,pch-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
							//printf("%s\n",CONTENT );
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_C>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_C>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1c\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,pch-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
							//printf("%s\n",CONTENT );
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_W>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_W>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,pch-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
							//printf("%s\n",CONTENT );
							//write(telnetSocket,"\r\n",2);
						}
					}
					else{
						strncpy(CONTENT,buffer,pch-buffer);
						CONTENT[sizeof(buffer)]='\0';
						//printf("%s",CONTENT );
						write(telnetSocket,"\x016",1);
						write(telnetSocket,"\x003",1);
						write(telnetSocket,"\x016",1);
						write(telnetSocket,"\x003",1);
						write(telnetSocket,"1w\r\n",4);
						write(telnetSocket,CONTENT,strlen(CONTENT));
						//write(telnetSocket,"\r\n",2);
					}



					//write(telnetSocket,CONTENT,strlen(CONTENT));
					write(telnetSocket, "\x018" ,1);
    				sleep(1);
    				write(telnetSocket,"s\r\n",3);
    				write(telnetSocket, "q",1);
    				write(telnetSocket, "q",1);
    				write(telnetSocket, "q",1);
    				write(telnetSocket, "q",1);
    				write(telnetSocket, "q",1);
    				write(telnetSocket, "q",1);
    				write(telnetSocket, "q",1);
    				printf("Save and post...\n");
					break;
				}

				else{
					if((color_f=strstr(buffer,"<COLOR_R>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_R>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1r\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_Y>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_Y>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1y\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_B>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_B>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1b\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_G>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_G>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1g\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_L>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_L>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1l\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_P>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_P>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1p\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_C>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_C>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1c\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
						}
					}
					else if((color_f=strstr(buffer,"<COLOR_W>"))!=NULL){
						if((color_l=strstr(buffer,"</COLOR_W>"))!=NULL){
							strncpy(CONTENT,buffer,color_f-buffer);
							CONTENT[color_f-buffer]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,sizeof(buffer)-(color_l-buffer)-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
						}
					}
					else{
						strncpy(CONTENT,buffer,sizeof(buffer));
						CONTENT[sizeof(buffer)]='\0';
						write(telnetSocket,"\x016",1);
						write(telnetSocket,"\x003",1);
						write(telnetSocket,"\x016",1);
						write(telnetSocket,"\x003",1);
						write(telnetSocket,"1w\r\n",4);
						write(telnetSocket,CONTENT,strlen(CONTENT));
						//write(telnetSocket,"\r\n",2);
						//printf("%s",CONTENT );
					}
				}
			}
		}
		else{
			if((color_f=strstr(buffer,"<COLOR_R>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_R>"))!=NULL){
							strncpy(CONTENT,buffer+9,color_f-buffer-9);
							CONTENT[color_f-buffer-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1r\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,c-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_Y>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_Y>"))!=NULL){
							strncpy(CONTENT,buffer+9,color_f-buffer-9);
							CONTENT[color_f-buffer-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1y\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,c-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_B>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_B>"))!=NULL){
							strncpy(CONTENT,buffer+9,color_f-buffer-9);
							CONTENT[color_f-buffer-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1b\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,c-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_G>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_G>"))!=NULL){
							strncpy(CONTENT,buffer+9,color_f-buffer-9);
							CONTENT[color_f-buffer-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1g\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,c-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_L>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_L>"))!=NULL){
							strncpy(CONTENT,buffer+9,color_f-buffer-9);
							CONTENT[color_f-buffer-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1l\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,c-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_P>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_P>"))!=NULL){
							strncpy(CONTENT,buffer+9,color_f-buffer-9);
							CONTENT[color_f-buffer-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1p\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,c-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_C>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_C>"))!=NULL){
							strncpy(CONTENT,buffer+9,color_f-buffer-9);
							CONTENT[color_f-buffer-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1c\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,c-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
				}
			}
			else if((color_f=strstr(buffer,"<COLOR_W>"))!=NULL){
				if((color_l=strstr(buffer,"</COLOR_W>"))!=NULL){
							strncpy(CONTENT,buffer+9,color_f-buffer-9);
							CONTENT[color_f-buffer-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//printf("%s\n",CONTENT );
							strncpy(CONTENT,color_f+9,color_l-color_f-9);
							CONTENT[color_l-color_f-9]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							strncpy(CONTENT,color_l+10,c-color_l-10);
							CONTENT[strlen(CONTENT)]='\0';
							//printf("%s\n",CONTENT);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"\x016",1);
							write(telnetSocket,"\x003",1);
							write(telnetSocket,"1w\r\n",4);
							write(telnetSocket,CONTENT,strlen(CONTENT));
							//write(telnetSocket,"\r\n",2);
				}
			}
			else{
				strncpy(CONTENT,buffer+9,(c-buffer-9));
				CONTENT[c-buffer-9]='\0';
				//printf("%s\n",CONTENT );
				write(telnetSocket,CONTENT,strlen(CONTENT));
				//write(telnetSocket,"\r\n",2);
			}
			
			write(telnetSocket, "\x018" ,1);
    		sleep(1);
    		write(telnetSocket,"s\r\n",3);
    		write(telnetSocket, "q",1);
    		write(telnetSocket, "q",1);
    		write(telnetSocket, "q",1);
    		write(telnetSocket, "q",1);
    		write(telnetSocket, "q",1);
    		write(telnetSocket, "q",1);
    		write(telnetSocket, "q",1);
    		printf("Save and post...\n");
		}
	}

		else if((pch = strstr (buffer, "<EXIT>"))!=NULL){
			printf("Exit...\n");
			write(telnetSocket, "g",1);
    		sleep(3);
    		write(telnetSocket, "\r\n",2);
    		write(telnetSocket,"y\r\n",3);
			break;
		}
	
	}


	close(telnetSocket);
	return 0;

}