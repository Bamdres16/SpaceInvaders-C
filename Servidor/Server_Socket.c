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
#include <json-c/json.h>
#include "../Estructura.c"
int k;
char buf[20000];
char bufParar[]         = {"0"};
char bufCrearAlien[]    = {"1"};
char bufEliminarAlien[] = {"2"};

void *manejadorConsola(void);

//the thread function
void *connection_handler(void *);

int socket_desc , client_sock , c , *new_sock;

int main()
{



    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    //temp_sock_desc = accept(socket_desc,(struct sockaddr*)&client,&len);

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
    listen(socket_desc , 4);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    c=sizeof(struct sockaddr_in);
    pthread_t threadComandos;
    pthread_create(&threadComandos, NULL, manejadorConsola , NULL);


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


        puts("Escribe un comando... \n ");
        pthread_join(threadComandos, NULL);


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

    char sendBuff[20000], client_message[20000];

   // while(n=recv(client_sock,client_message,2000,0) != 0) // probando con cliente_sock antes era sock
   while (1) // while tenia que ser infinito....
    {
        n = recv(client_sock,client_message,20000,0);

            if(strncmp(client_message, "Hola", 4) == 0){
                send (sock, "Hola", n, 0);
                printf("\n %s",client_message);
            }
            if(strncmp(client_message, "IZQ", 3)== 0){
                printf("\n %s",client_message);
            }
            if(strncmp(client_message, "DER", 3)== 0){
                printf("\n %s",client_message);
            }
            if(strncmp(client_message, "UP", 3)== 0){
              json_object *parsed = json_tokener_parse(client_message);


              printf("%d\n",getPosY(getIndex(parsed,0)));
            }
            if(strncmp(client_message, "HolaServer", 10)){
                printf("\n %s", client_message);
            }
            if(strncmp(client_message, "RecibidoCrear", 13) == 0){
                printf("\n %s", client_message);
            }

            if(strncmp(buf, "crear", 5 ) == 0){
                send(client_sock, "1" ,  100, 0  );
                printf("Se envio crear Alien en While.........");
                bzero(buf,20000);

            }

        bzero(client_message, 20000); // esto es lo que faltaba




    }


    //close(sock);


    /*
    if(n==0)
    {

        puts("Client Disconnected");
    }
    else
    {
        perror("recv failed");
    }
     */

    return 0;
}

void *manejadorConsola(){


    while (1) {
      printf("Escriba un comando..");
        fgets(buf, 20000, stdin);


        if(strncmp(buf,"end",3) == 0)
            break;
    }
    /*

    while(1)
    {

        puts("Escribe un comando...");

        fgets(buf,100,stdin);

    //Comandos para poder controlar al cliente:

        if(strncmp(buf, "parar", 5) == 0){
            printf("Parar");
            send (socket_desc, bufParar , 100, 0);
            break;

        }

        if(strncmp(buf, "crear", 5) == 0){
            //send(client_sock, bufCrearAlien, 100, 0);

            printf("Se mando crear... %s", bufCrearAlien);

        }

        if(strncmp(buf, "eliminar", 8) == 0){


            printf("Se envio eliminar Alien");


        }

        if(strncmp(buf, "0", 1) == 0){
            printf("Un alien ha sido eliminado desde el cliente...");
        }

        if(strncmp(buf,"end",3) == 0)
            break;





        if(k == -1)
            {
                printf("Error en el envío de mensajes...");
                exit(1);
        }
    }
     */
    return 0;
}
