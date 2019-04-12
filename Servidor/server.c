//
//  main.c
//  ServidorSpace
//
//  Created by Edgar Chaves on 4/5/19.
//  Copyright © 2019 Edgar Chaves. All rights reserved.
//
// https: //stackoverflow.com/questions/40297504/client-server-chat-application-in-c.

#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>//srand() y rand()



int main()
{
    char buf[100];
    char inv[155][3];
    char bufParar[] = {"0"};
    char bufCrearAlien[] = {"1"};
    char bufEliminarAlien[] = {"2"};

    int k;
    socklen_t len;
    int sock_desc,temp_sock_desc;
    struct sockaddr_in server,client;

    memset(&server,0,sizeof(server));
    memset(&client,0,sizeof(client));

    sock_desc = socket(AF_INET,SOCK_STREAM,0);
    if(sock_desc == -1)
    {
        printf("Error en la creación del socket.");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = 3000; //3002

    k = bind(sock_desc,(struct sockaddr*)&server,sizeof(server));
    if(k == -1)
    {
        printf("Error conectando...");
        exit(1);
    }

    k = listen(sock_desc,20);
    if(k == -1)
    {
        printf("Error escuchando...");
        exit(1);
    }

    len = sizeof(client);//VERY IMPORTANT
    temp_sock_desc=accept(sock_desc,(struct sockaddr*)&client,&len);
    if(temp_sock_desc == -1)
    {
        printf("Error temporal en la creación del socket...");
        exit(1);
    }

    while(1)
    {
        k = recv(temp_sock_desc,buf,100,0);
        printf(" El valor de k es : %i", k);
        if(k == -1)
        {
            printf("Error recibiendo el mensaje...");

            exit(1);
        }
        printf("Mensaje obtenido desde el cliente : %s",buf);
        printf("\nEnter data to be send to client: ");

        fgets(buf,100,stdin);

        //Comandos para poder controlar al cliente:
        char result[100];

        if(strncmp(buf, "parar", 5) == 0){
            printf("Parar");
            send (temp_sock_desc, bufParar , 100, 0);
            break;

        }

        if(strncmp(buf, "crear", 5) == 0){
            printf("Se envio alien\n");
            char result[100];

            send (temp_sock_desc, inv , 100, 0);

        }

        if(strncmp(buf, "eliminar", 8) == 0){
            printf("Se envio eliminar Alien");
            send (temp_sock_desc, bufEliminarAlien, 100, 0);

        }

        if(strncmp(buf,"end",3) == 0)
            break;


        //k = send(temp_sock_desc,buf,100,0);

        if(k == -1)
        {
            printf("Error en el envío de mensajes...");
            exit(1);
        }
    }
    close(temp_sock_desc);

    exit(0);
    return 0;
}
