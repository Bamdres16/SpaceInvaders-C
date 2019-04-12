//
//  main.c
//  ThreadServidorSpaceInvaders2
//
//  Created by Edgar Chaves on 4/11/19.
//  Copyright © 2019 Edgar Chaves. All rights reserved.
//


#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

#include "../Estructura.c"

int cliente = 0;
//the thread function
void *connection_handler(void *);


int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 3000 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0 )
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    c=sizeof(struct sockaddr_in);
    while(client_sock=accept(socket_desc,(struct sockaddr*)&client,(socklen_t*)&c)){
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( (pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0))
        {
            perror("could not create thread");
            return 1;
        }



        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}
/*
 This will handle connection for each client
 */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int n;

    char sendBuff[100], client_message[2000];

    while((n=recv(sock,client_message,2000,0))>0)
    {


        if (strlen(client_message) > 100){
          json_object *parsed = json_tokener_parse(client_message);

          printf("Pos en X: %s\n", json_object_get_string(parsed));

        }
        //send(sock,client_message,n,0);
    }

    close(sock);

    if(n==0)
    {
        puts("Client Disconnected");
    }
    else
    {
        perror("recv failed");
    }
    return 0;
}
