/*
argv[0] -> filename
argv[1] -> ip-address
argv[2] -> portno
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<netdb.h> //For -> struct hostent

void error(char* msg){
    perror(msg);
    exit(1);
}

int main(int argc,char* argv[]){
    int sockfd,n,portno;
    struct sockaddr_in serv_addr;
    struct hostent* server;//we need hostent at client side to store server-side device network related information

    char buffer[255];

    if(argc<3){
        error("Required Number of Arguments are not provided");
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0) error("Error Opening Socket at Client Side");

    //Obtaining the server socket
    server = gethostbyname(argv[1]);
    if(server==NULL){
        fprintf(stderr,"Error no such host exists");
    }

    //clearing out the serv_addr
    bzero((char*)&serv_addr,sizeof(serv_addr));

    //setting the values to serv_addr
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr,(char*) &serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);

    if(connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr))<0){
        error("Error while connecting");
    }

    while(1){
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));

        if(n<0){
            error("Error writing...");
        }

        bzero(buffer,255);
        n = read(sockfd,buffer,255);
        if(n<0){
            error("Error reading...");
        }

        printf("Server Message Received: %s\n",buffer);

        int i = strncmp("Bye",buffer,3);
        if(i==0){
            break;
        }
    }
    close(sockfd);
    return 0;
}