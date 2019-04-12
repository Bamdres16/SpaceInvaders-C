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
    char buf [100];
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
    client.sin_port = 3000; //3002

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






          int *n1;

          printf("Direccion en cliente: %s\n", buf);
          sscanf(buf, "%p", &n1);
          printf("El valor en esa direccion es: %d\n", *n1);


    }
    pclose(sock_desc);
    exit(0);
    return 0;
}
