#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h> //srand() y rand()
#include <unistd.h> //usleep() y available
#include <time.h> //seed y random

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
#define FLASH "." //Puntos para cada ves que disparan al jugador se detecte mediante una pantalla de puntos
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
short int invasores[55][3];
const char *invasoresCalamar[] = {" o  o", "d0000b", " {  }", " [  ]"}; //Invasor tipo 1
const char *invasoresCancrejo[] = {" /00\\", "[WWWW]", "^    ^", " ^  ^"}; //Invasor tipo 2
const char *invasoresPulpo[] = {"  @@", "{0000}", "\\ \"\" /", "/ \"\" \\"}; //Invasor tipo 3
short int direccionInvasores = 1; //Este parámetro es para que se empiecen a mover a la derecha
short int numeroDeInvasores, filaDeInvasores, columnasDeInvasores; //Definicion de numero de invasores, filas y columnas

//**********************************************************************************

//                              Escudos
#define ANCHOBUNKERS 10
const char *bunker[] = {"h", "H"};
short int bunkers[144][3];



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
                if (invasores[j*columnasDeInvasores+i][2] != 0) {
                    bombaDisparada = TRUE;
                    posicionXdeBomba = invasores[j*columnasDeInvasores+i][0]+ANCHOINVASOR/2;
                    posicionYdeBomba = invasores[j*columnasDeInvasores+i][1]+ALTOINVASOR;
                    break;
                }
            }
            i++;
        }
    }

    //Si colisiona el borde de la pantalla, el indicador cambia, y cambia al contrario el sentido del movimiento del invasor (derecha-izquierda y viseversa)
    for (i = 0; i < 55; i++) {
        if (invasores[i][2]) {
            invasores[i][0] += direccionInvasores;

            if (invasores[i][0] + ANCHOINVASOR + 1 >= anchoPantalla || invasores[i][0] - 1 <= 0)
            flagInvertirDireccion = TRUE;
        }
    }

    //Si el flag se activa, cambia la dirección, mueve a todos los invasores hacia abajo y aumenta el intervalo de actualización
    if (flagInvertirDireccion == TRUE) {
        flagInvertirDireccion = FALSE;

        for (i = 0; i < 55; i++) {
            invasores[i][1] += INVADERDESCENDRATE;


            //Acá se comprueba si los invasores tocan la parte inferior de la pantalla
            if (invasores[i][2] != 0 && invasores[i][1]+ALTOINVASOR >= altoPantalla-ALTOJUGADOR)
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
