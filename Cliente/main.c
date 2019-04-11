//
//  main.c
//  Cliente
//
//  Created by Edgar Chaves on 4/9/19.
//  Copyright © 2019 Edgar Chaves. All rights reserved.
//

#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>
#include<stdlib.h>
#include <arpa/inet.h>

int main(){

    //Mensajes predeterminados al servidor:--------------------------;
    char buf[100];
    char buf1[] = {'h','o','l','a'};
    char bufExitoCrearAlien[] = {"Exito al crear Alien"};
    char bufExitoEliminarAlien[] = {"Exito al eliminar Alien"};
    //---------------------------------------------------------------;
    int k;
    int sock_desc;
    struct sockaddr_in client;
    memset(&client,0,sizeof(client));
    sock_desc=socket(AF_INET,SOCK_STREAM,0);

    if(sock_desc == -1){
        printf("Error en la creación del socket.");
        exit(1);
    }

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("192.168.0.5"); // 192.168.100.26
    client.sin_port = 3005; //3002

    k = connect(sock_desc,(struct sockaddr*)&client,sizeof(client));
    if(k == -1){
        printf("Error conectando al servidor");
        exit(1);
    }

    while(1){
       // printf("\nEnter data to be send to server: ");
        //fgets(buf,100,stdin);
        if(strncmp(buf,"end",3) == 0)
            break;





        k = send(sock_desc,buf1,100,0);
        if(k == -1){
            printf("Error en el envío...");
            exit(1);
        }

        k = recv(sock_desc,buf,100,0);

        //Prueba
        printf("\n %i", strncmp(buf, "1", 1));

        if(strncmp(buf, "0", 1) == 0){
            break;
        }

        if (strncmp(buf, "1", 1) == 0){
            printf("Recibido crear");
            send(sock_desc,bufExitoCrearAlien,100,0);
        }

        if(strncmp(buf, "2", 1) == 0){
            printf("Recibido eliminar");
            send(sock_desc,bufExitoEliminarAlien,100,0);
        }

        if(k == -1){
            printf("Error en el recibo del mensaje...");
            exit(1);
        }
        printf(" \n Mensaje obtenido del servidor : %s",buf);
    }
    close(sock_desc);
    exit(0);
    return 0;
}
