#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h> //This library is used for read,write and close using sockfd

void error(char* msg){
    perror(msg);
    exit(1);
}

int main(int argc,char* argv[]){
    if(argc<2){
        error("PORT NO not defined");
    }

    int sockfd = socket(AF_INET,SOCK_STREAM,0);//This is server file Descriptor
    if(sockfd < 0){
        error("Error opening socket");
    }

    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    //Clear the serv_addr
    bzero((char*) &serv_addr,sizeof(serv_addr));

    //Assigning Values to sockaddr_in structure of server(serv_addr)
    int portno;
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //Binding after assining values to sockaddr_in
    if(bind(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0){
        error("Error could not bind");
    }

    //Listen
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    //New SockFd after lisening for accepting the connection
    int newSockFd = accept(sockfd,(struct sockaddr*) &cli_addr,&clilen);//This is client File Descriptor

    if(newSockFd<0){
        error("Error in accepting the incoming connection");
    }

    int n;//dummy variable n for read and write
    //LOOP
    while(1){
        bzero(buffer,255);
        n = read(newSockFd,buffer,255);
        if(n<0){
            error("Error on reading");
        }

        printf("Client Message: %s\n",buffer);

        bzero(buffer,255);
        fgets(buffer,255,stdin);

        n = write(newSockFd,buffer,strlen(buffer));
        if(n<0){
            error("Error while writing to client");
        }

        int i = strncmp("Bye",buffer,3);
        if(i==0){
            break;
        }
    }

    //close the newsockfd and sockfd
    close(newSockFd);
    close(sockfd);
    return 0;
}
