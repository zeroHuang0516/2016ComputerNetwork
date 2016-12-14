#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

typedef struct {
	char hostname[512];
	unsigned short port;
	int listen_fd;
}Server;

Server svr;
int t;
int maxfd;
int ret;
int conn_fd;
int maxClients =1024;
int client_socket[1024];
fd_set readset;
char buffer[1025];



int main(int argc, char* argv[])
{
	//initialize client_socket
	for(int i=0;i<maxClients;i++){
		client_socket[i]=0;
	}


	//create a socket
	gethostname(svr.hostname, sizeof(svr.hostname));
	svr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(svr.listen_fd<0){
		perror("socket");
		exit(1);
	}

	//set rebind sever
	int tmpaddr=1;
	if(setsockopt(svr.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmpaddr, sizeof(tmpaddr))<0){
		perror("setsockopt");
		exit(1);
	}

	//initialize the server
	struct sockaddr_in svraddr;
	int addrlen;
	bzero(&svraddr, sizeof(svraddr));
	svraddr.sin_family = AF_INET;
  	svr.port =(unsigned short) atoi(argv[1]);
	svraddr.sin_port = htons(svr.port);
	svraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind
	if(bind(svr.listen_fd, (struct sockaddr*)&svraddr, sizeof(svraddr))<0){
		perror("bind");
		exit(1);
	}

	//listens
	if(listen(svr.listen_fd, maxClients) < 0){
		perror("listen");
		exit(1);
	}

	addrlen = sizeof(svraddr);
	//printf("Waiting for connections ...\n");

	while(1)
	{
		//IO multiplexing
		FD_ZERO(&readset);
		FD_SET(svr.listen_fd, &readset);
		maxfd = svr.listen_fd;

		for(int i=0; i<maxClients;i++){
			if(client_socket[i]>0){
				FD_SET(client_socket[i], &readset);
			}
			//update maxfd
			if(client_socket[i] > maxfd){
				maxfd = client_socket[i];
			}
		} //end for

		ret = select(maxfd+1, &readset, NULL, NULL ,NULL);

		if(ret==-1){
      		printf("select() error\n");
      		return -1;
      	}
      	else if(ret==0){
      		continue;
      	}

      	if(FD_ISSET(svr.listen_fd, &readset)){
      		if((conn_fd=accept(svr.listen_fd, (struct sockaddr*)&svraddr, (socklen_t*)&addrlen))<0)
      		{
      			perror("accept");
      			exit(1);
      		}
      		//printf("New connection, socket fd is %d, ip is : %s, port : %d \n" , conn_fd , inet_ntoa(svraddr.sin_addr) , ntohs(svraddr.sin_port));
      		
      		
      		//add new connection to read_set
      		for(int i=0;i<maxClients;i++){
      			if(client_socket[i]==0){
      				client_socket[i] = conn_fd;
      				//printf("Adding to list of sockets as %d\n" , i);
                    break;
      			}
      		}
      	}

      	for(t=0; t<maxClients;t++){
      		if(FD_ISSET(client_socket[t], &readset)){
      			if ((read( client_socket[t] , buffer, 1024)) == 0)
                {
                    //disconnect
                    getpeername(client_socket[t] , (struct sockaddr*)&svraddr , (socklen_t*)&addrlen);
                    //printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(svraddr.sin_addr) , ntohs(svraddr.sin_port));
                      
                    //Close client socket
                    close( client_socket[t] );
                    //reuse in the future
                    client_socket[t] = 0;
                }

                else{
                	 printf("recv from %s:%d, seq = %s\n",inet_ntoa(svraddr.sin_addr),ntohs(svraddr.sin_port),buffer );
                	//printf("%s\n",buffer );
                	send(client_socket[t], buffer, strlen(buffer), 0);
                }
      		}
      	}
	}
	return 0;
}