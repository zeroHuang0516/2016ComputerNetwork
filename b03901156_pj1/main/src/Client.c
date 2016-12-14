#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/time.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h> 
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>


unsigned short port;
char* ip[25];

void client(unsigned short port, char*ip[25],int total, double timeout){
	int sockfd;
        int seq=0;  //client's seq
        struct hostent *h;
        struct sockaddr_in svr_addr;
        char *tmp[512];
		int counter=0;
		clock_t start, finish;
		char buf[512];
		char msg[512];

		if ((h=gethostbyname(ip)) == NULL) {  
            perror("gethostbyname");
            exit(1);
        }
        
        //the socket which connects to server
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
            perror("socket");
            exit(1);
        }

        svr_addr.sin_port = htons(port);
        svr_addr.sin_family = AF_INET;
        svr_addr.sin_addr = *((struct in_addr *)h->h_addr);
        bzero(&(svr_addr.sin_zero), 8);//zero the rest of the struct 

        if (connect(sockfd, (struct sockaddr *)&svr_addr,sizeof(struct sockaddr)) <0) {
            perror("connect");
            exit(1);
        }

        //printf("New connection, ip is : %s, port is : %d\n", inet_ntoa(svr_addr.sin_addr),port);
        
	while (1) {
	  if(total!=0){
		if(counter==total){break;}

		sprintf(msg, "%d", seq);
		if (send(sockfd, msg, 25, 0) == -1){
            perror("send");
		    exit (1);
		}
		else{
			start=clock();
            if ((recv(sockfd, buf, 1024, 0)) < 0 ) {
                perror("recv");
                exit(1);
		    }

		      finish=clock();

		    if(((double)(finish - start)*1000 / CLOCKS_PER_SEC)<=timeout){
	    	      printf("recv from %s:%d, seq = %d, RTT = %lf msec\n",inet_ntoa(svr_addr.sin_addr) ,port,seq,((double)(finish - start)*1000.0 / CLOCKS_PER_SEC) );
      		        counter++;
			         seq++;
		    }
		    else{
			     printf("timeout when connect to %s:%d, seq = %d\n",inet_ntoa(svr_addr.sin_addr) ,port,seq);
			     counter++;
			     seq++;

		    }
		}
	  }
	  else{
	  	sprintf(msg, "%d", seq);
		if (send(sockfd, msg, 25, 0) == -1){
            perror("send");
		    exit (1);
		}
		else{
			start=clock();
            if ((recv(sockfd, buf, 1024, 0)) < 0 ) {
                perror("recv");
                exit(1);
		    }

		      finish=clock();

		    if(((double)(finish - start)*1000 / CLOCKS_PER_SEC)<=timeout){
	    	      printf("recv from %s: %d,seq=%d, RTT=%lf msec\n",inet_ntoa(svr_addr.sin_addr),port,seq,((double)(finish - start)*1000 / CLOCKS_PER_SEC) );
			         seq++;
		    }
		    else{
			     printf("timeout when connect to %s: %d, seq=%d\n",ip,port,seq);
			     seq++;

		    }
		}
	  }
	}//end while

        close(sockfd);
        //printf("Host disconnected , ip %s , port %d\n" , inet_ntoa(svr_addr.sin_addr) , port);
}

int main(int argc, char *argv[])
{
        char *tmp[512];
        int total = 0;//default
		double timeout=1000;//default
		int present=1;
		int count;

		if(argv[1][0] == '-' && argv[1][1]=='n'){
			total = atoi(argv[2]);
			present=3;
			if(argv[3][0] == '-' && argv[3][1]=='t'){
				timeout = atof(argv[4]);
				present=5;
			}
		}
		else if(argv[1][0] == '-' && argv[1][1]=='t'){
			timeout = atof(argv[2]);
			present=3;
			if(argv[3][0] == '-' && argv[3][1]=='n'){
				total = atoi(argv[4]);
				present=5;
			}
		}
	
		for(int k=present;k<argc;k++){
			for(int j=0;j<strlen(argv[k]);j++){
        		if(argv[k][j]==':'){
        			count=j;
        		}
        	}
        	strncpy(tmp, argv[k], count);
        	strcpy(ip, tmp);
        	tmp[0]=NULL;
        	strncpy(tmp, argv[k]+count+1, strlen(argv[k])-count);
        	port = (unsigned short)atoi(tmp);
        	client(port,ip,total,timeout);
		}

        return 0;
    }

