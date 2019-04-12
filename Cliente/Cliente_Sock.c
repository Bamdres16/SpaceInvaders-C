//
//  main.c
//  ThreadClienteSpaceInvaders
//
//  Created by Edgar Chaves on 4/11/19.
//  Copyright © 2019 Edgar Chaves. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h> //seed for random
#include<pthread.h>

#define MAX_SIZE 50

#include "../Estructura.c"







int sock_desc;
char client_message[2000];


//************************CONSTANTES Y VARIABLES*************************************
//                          Variables del juego


#define UPDATEINTERVAL 25000
short int anchoPantalla, altoPantalla; //Variables de la ventana
short int puntaje;// Variable Puntaje
short int vidasJugador; //Variable contador de vidad
int invaderUpdateInterval;
short int animationFlag = FALSE;
//**********************************************************************************

//                    Definiendo y dibujando al jugador

#define CANON "  A" //Dibujo del cañón del jugador
#define JUGADOR "_/A\\_" //Cuerpo del jugador
#define FLASH "." //puntito para cada ves que disparan al jugador se detecte mediante una pantalla de puntitos
#define ANCHOJUGADOR 6  //Dimensiones de la bala del jugador
#define ALTOJUGADOR 2
#define VELOCIDADJUGADOR 2  //Velocidad del movimiento del jugador
short int posicionXdeJugador; //Posición del jugador
short int flagDisparoJugador;
//**********************************************************************************

//                          Disparo del jugador


#define DISPARO "|"
#define VELOCIDADDISPARO 1 //Velocidad del disparo del jugador
short int posicionXdeDisparo, posicionYdelDisparo;
short int disparoTirado = FALSE; //Se define inicialmente que el tiro no sea disparado

//**********************************************************************************

//                            Disparo de Aliens
#define BOMBAINVASOR "0"
#define VELOCIDADBOMBA 1  //Velocidad del disparo de Aliens
short int posicionXdeBomba, posicionYdeBomba;
short int bombaDisparada = FALSE; //Se define inicialmente que el tiro no sea disparado

//**********************************************************************************

//                              Invasores
#define ANCHOINVASOR 6
#define ALTOINVASOR 3
#define INVADERINCREASESPEEDRATE 25000
#define INVADERDESCENDRATE 2

const char *invasoresCalamar[] = {" o  o", "d0000b", " {  }", " [  ]"}; //Invasor tipo 1
const char *invasoresCancrejo[] = {" /00\\", "[WWWW]", "^    ^", " ^  ^"}; //Invasor tipo 2
const char *invasoresPulpo[] = {"  @@", "{0000}", "\\ \"\" /", "/ \"\" \\"}; //Invasor tipo 3
short int direccionInvasores = 1; //Este parámetro es para que se empiecen a mover a la derecha
short int numeroDeInvasores, filaDeInvasores, columnasDeInvasores; //Definicion de numero de invasores, filas y columnas
json_object *invasores_json;

//**********************************************************************************


//                              Escudos
#define ANCHOBUNKERS 10
const char *bunker[] = {"h", "H"};
short int bunkers[144][3];
json_object *bunkers_json;

//**********************************************************************************


//**********************************************************************************

//                      Configuracion del juego y reset

void configuracionJuego(int configuracionIncial) {
    int i, x, fila, columna;

      invasores_json = json_object_new_array();
      bunkers_json = json_object_new_array();

    //Calcular los escudos, los invasores y numero de escudos dependiendo de las dimensiones de la pantalla
    int anchoSecciondeBloqueo, espacioEntreInvasores, numeroDeEscudos;

    //Configura las filas de invasores dependiendo de las dimensiones de la pantalla
    if (altoPantalla < 50)
        filaDeInvasores = altoPantalla/12;
    else
        filaDeInvasores = 5;

    //Configura las columnas de invasores y el numero de escudos dependiendo de las dimensiones de las pantallas
    if (anchoPantalla < 160) {
        columnasDeInvasores =  anchoPantalla/14;
        numeroDeEscudos = anchoPantalla/20; //Acá se define el número de escudos, 80/20 = 4 escudos
    }
    else {
        columnasDeInvasores = 14;
        numeroDeEscudos = 4;
    }

    //Se define el número de invasores, espacio entre invasores y ancho de los escudos
    numeroDeInvasores = filaDeInvasores * columnasDeInvasores;
    espacioEntreInvasores = (anchoPantalla-(columnasDeInvasores*ANCHOINVASOR*2))/2;
    anchoSecciondeBloqueo =  9;//anchoPantalla/((numeroDeEscudos*2-1)+1);  //espacio entre los bloques


    //Inicializando los Invasores
    x = 0;
    for (fila = 0; fila < filaDeInvasores; fila++) {
        for (columna = 0; columna < columnasDeInvasores; columna++) {

            json_object *jobj = json_object_new_object();
            json_object_object_add(jobj,"posx",json_object_new_int(espacioEntreInvasores + columna*ANCHOINVASOR*2));
            json_object_object_add(jobj,"posy",json_object_new_int(fila*ALTOINVASOR*2 + ALTOINVASOR));
            json_object_object_add(jobj,"estado",json_object_new_int(1));
            json_object_array_add(invasores_json,jobj);
            x++;
        }
    }

    //si se cumple la primer configuración del juego, las variables del juego y configuraciones se setean
    if (configuracionIncial == TRUE) {
        puntaje = 0;
        vidasJugador = 3;
        flagDisparoJugador = FALSE;
        invaderUpdateInterval = 250000;
        disparoTirado = FALSE;
        bombaDisparada = FALSE;

        //Inicializar Jugador
        posicionXdeJugador = anchoPantalla/2-ANCHOJUGADOR/2;

        //Inicializar escudos
        x = 0;
        for (i = 0; i < numeroDeEscudos; i++) { //Para todos los bloques
            for (fila = 0; fila < 3; fila++) { //para cada linea de los escudos
                for (columna = 0; columna < ANCHOBUNKERS; columna++) { //Para cada bloque de escudo
                    if (fila == 2 && columna == 2) //Solamente 4 escudos en la fila inferior
                        columna = ANCHOBUNKERS-2;
                    bunkers[x][0] = anchoSecciondeBloqueo + i*anchoSecciondeBloqueo*2 + columna; //setea X
                    bunkers[x][1] = altoPantalla-ALTOJUGADOR-4+fila; //setea Y
                    bunkers[x][2] = 2; //Acá se establece el estado del escudo

                    x++;
                }
            }
        }
    }

    //Subiendo cada nivel
    if (configuracionIncial == FALSE) {
        invaderUpdateInterval -= INVADERINCREASESPEEDRATE; //incrementando la velocidad por 1/20 del original
        vidasJugador++; //Aumenta las vidas del jugador
    }


}


//**********************************************************************************

//                      Configuraciones para los Invasores

void configuracionInvasores() {
    static short int flagInvertirDireccion;
    int i, j;


    //1 Entrada, 3 salidas, se lanza una bomba
    if (!(rand()%3)) {
        i = rand() % columnasDeInvasores + 1;

        //Se elige un invasor random y se suelta la bomba del invasor desde ahí, cuidando que no haya otro invasor ahí
        while (!bombaDisparada) {
            if (i >= columnasDeInvasores)
                i = 0;

            for (j = filaDeInvasores-1; j >= 0; j--) {
                if (getEstado(getIndex(invasores_json,j*columnasDeInvasores+i)) != 0) {
                    bombaDisparada = TRUE;
                    posicionXdeBomba = getPosX(getIndex(invasores_json,j*columnasDeInvasores+i))+ANCHOINVASOR/2;
                    posicionYdeBomba = getPosY(getIndex(invasores_json,j*columnasDeInvasores+i))+ALTOINVASOR;
                    break;
                }
            }
            i++;
        }
    }

    //Si colisiona el borde de la pantalla, el indicador cambia, y cambia al contrario el sentido del movimiento del invasor (derecha-izquierda y viseversa)
    for (i = 0; i < 55; i++) {
        if (getEstado(getIndex(invasores_json,i))) {
            invasores_json = setPosX(invasores_json,i,getPosX(getIndex(invasores_json,i))+direccionInvasores);
            if (getPosX(getIndex(invasores_json,i)) + ANCHOINVASOR + 1 >= anchoPantalla || getPosX(getIndex(invasores_json,i)) - 1 <= 0)
                flagInvertirDireccion = TRUE;
        }
    }

    //Si el flag se activa, cambia la dirección, mueve a todos los invasores hacia abajo y aumenta el intervalo de actualización
    if (flagInvertirDireccion == TRUE) {
        flagInvertirDireccion = FALSE;

        for (i = 0; i < 55; i++) {
            invasores_json = setPosY(invasores_json,i,getPosY(getIndex(invasores_json,i))+INVADERDESCENDRATE);

            //Acá se comprueba si los invasores tocan la parte inferior de la pantalla
            if (getEstado(getIndex(invasores_json,i)) != 0 && getPosY(getIndex(invasores_json,i))+ALTOINVASOR >= altoPantalla-ALTOJUGADOR)
                vidasJugador = 0;
        }

        //Invierte la direeción
        if (direccionInvasores > 0)
            direccionInvasores = -1;
        else
            direccionInvasores = 1;
    }

    if (animationFlag == TRUE)
        animationFlag = FALSE;
    else
        animationFlag = TRUE;
}

//**********************************************************************************

//                      Funciones de los proyectiles

void configuracionProyectil() {
    short int i;

    //El disparo se va fuera de la pantalla
    if (posicionYdelDisparo <= 0)
        disparoTirado = FALSE;

    if (posicionYdeBomba > altoPantalla)
        bombaDisparada = FALSE;

    //PRIMERO: Revisa si el tiro ha impactado algun invasor, verificando la coíncidencia de posición de ambos(Invasor y proyectil)
    if (disparoTirado) {

        for (i = 0; i < 55; i++) {
            if (getEstado(getIndex(invasores_json,i))) { //Si el invasor no ha sido tocado por un proyectil...
                if (getPosX(getIndex(invasores_json,i)) <= posicionXdeDisparo && getPosX(getIndex(invasores_json,i))+ANCHOINVASOR >= posicionXdeDisparo && getPosY(getIndex(invasores_json,i)) <= posicionYdelDisparo && getPosY(getIndex(invasores_json,i))+ALTOINVASOR >= posicionYdelDisparo) {

                    invasores_json = setFalse(invasores_json,i);

                    numeroDeInvasores--;
                    disparoTirado = FALSE;

                    //Brinda puntaje dependiendo de la posición de la fila del invasor golpeado
                    if (i/columnasDeInvasores < 1)
                        puntaje += 40;
                    else if (i/columnasDeInvasores < 3)
                        puntaje += 20;
                    else
                        puntaje += 10;

                    //Si mueren todos los invasores o si no hay invasores, se avanza de nivel
                    if (numeroDeInvasores == 0)
                        configuracionJuego(FALSE);
                }
            }
        }
    }

    //Informa al server:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //SEGUNDO: Verifica si la bomba de los invasores ha golpeado al jugador
    if (bombaDisparada) {
        if (posicionXdeBomba >= posicionXdeJugador && posicionXdeBomba <= posicionXdeJugador+ANCHOJUGADOR && posicionYdeBomba >= altoPantalla-3) {
            vidasJugador--;

            flagDisparoJugador = TRUE;
            bombaDisparada = FALSE;
        }
    }


    //TERCERO Y CUARTO: Verifica si el disparo del jugador o del invasor ha golpeado a los escudos, y se modifica el estado de los mismos
    if (posicionYdelDisparo >= altoPantalla-6 || posicionYdeBomba >= altoPantalla-6) {
        for (i = 0; i < 144; i++) {
            if (bunkers[i][2]) { //Si el escudo está arriba
                if (disparoTirado && posicionXdeDisparo == bunkers[i][0] && posicionYdelDisparo == bunkers[i][1]) {
                    bunkers[i][2]--;
                    disparoTirado = FALSE;
                }
                if (bombaDisparada && posicionXdeBomba == bunkers[i][0] && posicionYdeBomba == bunkers[i][1]) {
                    bunkers[i][2]--;
                    bombaDisparada = FALSE;
                }
            }
        }
    }

    //Informa al server:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //QUINTO: Se da el movimiento de los proyectiles del jugador y de los invasores
    posicionYdelDisparo-=VELOCIDADDISPARO;
    posicionYdeBomba+=VELOCIDADBOMBA;
}



//**********************************************************************************

//                              Mostrar Objetos

//Se ejecutan todos los objetos y se imprimen en pantalla
void displayObjects() {
    int i, j;

    //Esto genera una pantalla rápida de puntos, que aparece y desaparece, cada vez que el jugador ha sido impactado
    if (flagDisparoJugador) {
        clear();
        for (i = 0; i < altoPantalla; i++) {
            for (j = 0; j < anchoPantalla; j++) {
                mvprintw(i,j, FLASH);
            }
        }
        refresh();
        flagDisparoJugador = FALSE;
        usleep(UPDATEINTERVAL);
    }

    //Imprime a los invasores
    for (i = 0; i < filaDeInvasores; i++) {
        const char *(*currentInvader)[4];

        if (i == 0)
            currentInvader = &invasoresCalamar;
        else if (i > 2)                                                        //Aqui hice un cambio, era:  else if (i > 2)
            currentInvader = &invasoresPulpo;
        else
            currentInvader = &invasoresCancrejo;


        //Si el invasor no ha sido golpeado, se sigue mostrando en pantalla al invasor
        for (j = 0; j < columnasDeInvasores; j++) {
            if (getEstado(getIndex(invasores_json,i*columnasDeInvasores+j))) {

                mvprintw(getPosY(getIndex(invasores_json,i*columnasDeInvasores+j)), getPosX(getIndex(invasores_json,i*columnasDeInvasores+j)), (*currentInvader)[0]);
                mvprintw(getPosY(getIndex(invasores_json,i*columnasDeInvasores+j))+1, getPosX(getIndex(invasores_json,i*columnasDeInvasores+j)), (*currentInvader)[1]);
                mvprintw(getPosY(getIndex(invasores_json,i*columnasDeInvasores+j))+2, getPosX(getIndex(invasores_json,i*columnasDeInvasores+j)), (*currentInvader)[2+animationFlag]);
            }
        }
    }

    //Muestra a los escudos aún no destruídos
    for (i = 0; i < 144; i++) {
        if (bunkers[i][2])
            mvprintw(bunkers[i][1], bunkers[i][0], bunker[bunkers[i][2]-1]);
    }

    //Muestra al proyectil cuando se dispara
    if (disparoTirado)
        mvprintw(posicionYdelDisparo, posicionXdeDisparo, DISPARO);

    //Muestra el proyectil de los invasores cuando se dispara
    if (bombaDisparada)
        mvprintw(posicionYdeBomba, posicionXdeBomba, BOMBAINVASOR);

    //Muestra al jugador
    mvprintw(altoPantalla-3, posicionXdeJugador, CANON);
    mvprintw(altoPantalla-2, posicionXdeJugador, JUGADOR);

    //Informa al server:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //Imprime el puntaje y las vidas
    mvprintw(altoPantalla-1, 0, "Puntaje:%d", puntaje);
    mvprintw(altoPantalla-1, anchoPantalla-7, "Vidas:%d"," ", vidasJugador);
    //Informa al server:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
}


//**********************************************************************************

//                               Loop del juego

void loop() {
    int invaderTimer = 0;
    short int keyPressed; //Variable para detección de teclas

    while (vidasJugador) {
        usleep(UPDATEINTERVAL); //Actualiza la pantalla después de UPDATEINTERVAL milisegundos
        clear(); //Limpia la pantalla
        displayObjects(); //Coloca todos los objetos en la pantalla
        refresh(); //Actualiza


        keyPressed = getch(); //Obtiene la tecla pulsada actualmente

        //Handle keypresses
        switch(keyPressed) {
            case KEY_LEFT: //SI la tecla es a la izquierda, mueve a la izquierda al jugador
                if (posicionXdeJugador - VELOCIDADJUGADOR > 0){
                    posicionXdeJugador-=VELOCIDADJUGADOR;
                    send(sock_desc, "IZQ", 10, 0);
                }
                break;

            case KEY_RIGHT: //Si la tecla es a la derecha, mueve a la derecha al jugador
                if (posicionXdeJugador + ANCHOJUGADOR <= anchoPantalla){
                    posicionXdeJugador+=VELOCIDADJUGADOR;
                    send(sock_desc, "DER", 10, 0);
                }
                break;

            case KEY_UP: //Si la tecla es hacia arriba, dispara el proyectil
                if (!disparoTirado) {
                    posicionXdeDisparo = posicionXdeJugador+2;
                    posicionYdelDisparo = altoPantalla-3;
                    disparoTirado = TRUE;
                    send(sock_desc,json_object_get_string(invasores_json),strlen(json_object_get_string(invasores_json)),0);



                }

                break;

            case 113: //SI la tecla es Q, quita el programa
                vidasJugador = 0;
                break;
        }

        //Actualiza los timers
        invaderTimer += UPDATEINTERVAL;

        //Controla la pantalla

        if (disparoTirado || bombaDisparada)
            configuracionProyectil();

        //Si ya ha pasado algo de tiempo, empieza a mover a los invasores
        if (invaderTimer >= invaderUpdateInterval) {
            invaderTimer = 0;
            configuracionInvasores();
        }


    }
}

//**********************************************************************************


//                                  INICIO DEL JUEGO

void inicioDelJuego() {
    clear();
    mvprintw(altoPantalla/2-10, anchoPantalla/2-35, "=======================================================================");
    mvprintw(altoPantalla/2-9, anchoPantalla/2-35, " ___   ___   ___   ___  ___                  ___   __   ___   ___  ___");
    mvprintw(altoPantalla/2-8, anchoPantalla/2-35, "|___  |___| |___| |    |___     | |\\ | \\  / |___| |  \\ |___  |__/ |___");
    mvprintw(altoPantalla/2-7, anchoPantalla/2-35, " ___| |     |   | |___ |___     | | \\|  \\/  |   | |__/ |___  |  \\  ___|");
    mvprintw(altoPantalla/2-5, anchoPantalla/2-35, "=======================================================================");

    mvprintw(altoPantalla/2-3, anchoPantalla/2-35, invasoresPulpo[0]);
    mvprintw(altoPantalla/2-2, anchoPantalla/2-35, invasoresPulpo[1]);
    mvprintw(altoPantalla/2-1, anchoPantalla/2-35, invasoresPulpo[2]);
    mvprintw(altoPantalla/2+1, anchoPantalla/2-35, "Calamar");
    mvprintw(altoPantalla/2+2, anchoPantalla/2-36, "10 puntos");

    mvprintw(altoPantalla/2-3, anchoPantalla/2-3, invasoresCancrejo[0]);
    mvprintw(altoPantalla/2-2, anchoPantalla/2-3, invasoresCancrejo[1]);
    mvprintw(altoPantalla/2-1, anchoPantalla/2-3, invasoresCancrejo[2]);
    mvprintw(altoPantalla/2+1, anchoPantalla/2-2, "Pulpo");
    mvprintw(altoPantalla/2+2, anchoPantalla/2-4, "20 puntos");

    mvprintw(altoPantalla/2-3, anchoPantalla/2+29, invasoresCalamar[0]);
    mvprintw(altoPantalla/2-2, anchoPantalla/2+29, invasoresCalamar[1]);
    mvprintw(altoPantalla/2-1, anchoPantalla/2+29, invasoresCalamar[2]);
    mvprintw(altoPantalla/2+1, anchoPantalla/2+28, "Cangrejo");
    mvprintw(altoPantalla/2+2, anchoPantalla/2+28, "40 puntos");

    mvprintw(altoPantalla/2+4, anchoPantalla/2-17, "Presione cualquier tecla para jugar");
    mvprintw(altoPantalla/2+11, anchoPantalla/2-19, "Brayan Muñoz-Sebastián Alba-Edgar Cháves");
    refresh();
    nodelay(stdscr, FALSE);
    getch();
}

//**********************************************************************************

//                                  FIN DEL JUEGO

int finDelJuego() {
    clear();
    mvprintw(altoPantalla/2-3, anchoPantalla/2-24, "=================================================");
    mvprintw(altoPantalla/2-2, anchoPantalla/2-24, " ___   ___   _  _  ___      ___        ___   ___");
    mvprintw(altoPantalla/2-1, anchoPantalla/2-24, "| __  |___| | \\/ ||___     |   | \\  / |___  |__/");
    mvprintw(altoPantalla/2, anchoPantalla/2-24, "|___| |   | |    ||___     |___|  \\/  |___  |  \\");
    mvprintw(altoPantalla/2+2, anchoPantalla/2-24, "=================================================");

    mvprintw(altoPantalla/2+4, anchoPantalla/2-5, "Su Puntaje: %d", puntaje);
    mvprintw(altoPantalla/2+6, anchoPantalla/2-20, "Presione R para reiniciar, o Q para salir", puntaje);
    refresh();
    nodelay(stdscr, FALSE);
    return getch();
}

//**********************************************************************************

//                                  MAIN DEL JUEGO



void *inicioJuegoMain(){
    initscr(); //inicia el modo de cursores
    raw(); //desactiva el buffer de línea
    curs_set(0);  //oculta el cursor
    keypad(stdscr, TRUE); //inicializa el teclado
    srand(time(NULL)); //generador aleatorio
    getmaxyx(stdscr, altoPantalla, anchoPantalla); //obtiene las medidas de la pantalla



    int keyPressed;
    do {
        inicioDelJuego();
        nodelay(stdscr, TRUE);
        configuracionJuego(TRUE); //Activa las configuraciones del juego, por lo tanto incializa los elementos
        loop(); //Inicia el juego
        keyPressed = finDelJuego();
        mvprintw(0,0,"hello %d", keyPressed);
        refresh();
    } while (keyPressed == 114);


    endwin(); //fin del modo cursor
    return 0;
}

void *cliente(){

    struct sockaddr_in serv_addr;
    char sbuff[MAX_SIZE],rbuff[MAX_SIZE];

    if((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("Failed creating socket\n");

    bzero((char *) &serv_addr, sizeof (serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.0.5");
    serv_addr.sin_port = htons(3000);

    if (connect(sock_desc, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        printf("Failed to connect to server\n");
        //return -1;
    }

    //printf("Connected successfully - Please enter string\n");

    while(1)
    {
        send(sock_desc,sbuff,strlen(sbuff),0);

        if(recv(sock_desc,rbuff,MAX_SIZE,0)==0){
            printf("Error");
            exit(1);
          }
        else
            fputs(rbuff,stdout);

        bzero(rbuff,MAX_SIZE);//to clean buffer-->IMP otherwise previous word characters also came
    }


    close(sock_desc);
    return 0;


}


int main()
{

    pthread_t thread;
    pthread_t thread1;


    pthread_create(&thread1, NULL, cliente, NULL);
    pthread_create(&thread, NULL, inicioJuegoMain, NULL);

    pthread_join(thread, NULL);
    pthread_join(thread1, NULL);




    return 0;



}
