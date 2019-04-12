#include<stdio.h>
#include <json-c/json.h>
/*
Varias funciones entre ellas:

*/

/* <<<<<<<<<<----------------------------->>>>>>>>>>
								Para los aliens
						posx, posy, estado, tipo
<<<<<<<<<<----------------------------->>>>>>>>>> */

// Retorna la posicion en X del Alien
int getPosX (json_object *alien){
	struct json_object *posX;
	json_object_object_get_ex(alien, "posx", &posX);
	return json_object_get_int(posX);
	free(posX);
}
// Retorna la posicion en Y del Alien
int getPosY (json_object *alien){
	struct json_object *posY;
	json_object_object_get_ex(alien, "posy", &posY);
	return json_object_get_int(posY);
	free(posY);
}
// Retorna si está vivo o muerto
int getEstado (json_object *alien){
	struct json_object *estado;
	json_object_object_get_ex(alien, "estado", &estado);
	return json_object_get_int(estado);
	free(estado);
}
// Retorna que tipo es, pulpo, cangrejo...
const char* getTipo (json_object *alien){
	struct json_object *tipo;
	json_object_object_get_ex(alien, "tipo", &tipo);
	return json_object_get_string(tipo);
	free(tipo);
}

/*<<<<<<<<<<----------------------------->>>>>>>>>>
								Funciones basicas
objeto por index, largo de un arreglo, valor de una key (json_object),
		valor de una key (entero), obtener un alien por fila y columna,
								leer un archivo json,

<<<<<<<<<<----------------------------->>>>>>>>>> */
// Retorna una posicion del arreglo
json_object* getIndex (json_object *objeto, size_t iterador){
	struct json_object *value;
	value = json_object_array_get_idx(objeto,iterador);
	return value;
	free(value);
}


// Retorna el largo del arreglo
int getLength (json_object *fila_arr){
	return json_object_array_length(fila_arr);
}

//Retorna el objeto de la Key
json_object* getValue (json_object *objeto,const char* key){
	struct json_object *value;
	json_object_object_get_ex(objeto, key, &value);
	return value;
	free(value);
}
// Retorna el valor entero de la Key
int getValue_Entero (json_object *objeto,const char* key){
	struct json_object *value;
	json_object_object_get_ex(objeto, key, &value);
	return json_object_get_int(value);
	free(value);
}
// Obtiene un alien de un json en una fila columna
json_object* getAlien (size_t fila, size_t columna, json_object* parser){
	struct json_object *aliens;
	struct json_object *fila_s;
	struct json_object *fila_arr;
	size_t n_filas,n_columnas;
	aliens = getValue(parser,"aliens");
	fila_s = getIndex(aliens,fila);
	fila_arr = getValue(fila_s,"fila");
	return getIndex (fila_arr,columna);
	free(aliens);
	free(fila_s);
	free(fila_arr);
}
// Obtiene un json de un archivo
json_object* getJson (const char* name){
	FILE *fp;
	char buffer[1024];
	fp = fopen(name,"r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);
	return json_tokener_parse(buffer);
	free(fp);
}

// Delete position of array
json_object *deleteIndex (json_object *array, size_t iterador){
	struct json_object *value = json_object_new_array();
	size_t len = getLength(array);
	for (size_t i = 0; i < len;i++){
		if (i == iterador){
			continue;
		} else {
			json_object_array_add(value,getIndex(array,i));
		}
	}
	return value;
}

json_object *setFalse (json_object *array, size_t iterador){
	json_object *value = getIndex(array,iterador);
	struct json_object *isSet = json_object_new_array();
	size_t len = getLength(array);
	for (size_t i = 0; i < len;i++){
		if (i == iterador){
			json_object *obj = json_object_new_object();
			json_object_object_add(obj,"posx",json_object_new_int(getPosX(value)));
			json_object_object_add(obj,"posy",json_object_new_int(getPosY(value)));
			json_object_object_add(obj,"estado",json_object_new_int(0));
			json_object_array_add(isSet,obj);

		} else{
			json_object *obj = json_object_new_object();
			json_object_object_add(obj,"posx",json_object_new_int(getPosX(getIndex(array,i))));
			json_object_object_add(obj,"posy",json_object_new_int(getPosY(getIndex(array,i))));
			json_object_object_add(obj,"estado",json_object_new_int(getEstado(getIndex(array,i))));
			json_object_array_add(isSet,obj);
		}
	}
	return isSet;
}

json_object *setPosX (json_object *array, size_t iterador, int valor){
	json_object *value = getIndex(array,iterador);
	struct json_object *isSet = json_object_new_array();
	size_t len = getLength(array);
	for (size_t i = 0; i < len;i++){
		if (i == iterador){
			json_object *obj = json_object_new_object();
			json_object_object_add(obj,"posx",json_object_new_int(valor));
			json_object_object_add(obj,"posy",json_object_new_int(getPosY(value)));
			json_object_object_add(obj,"estado",json_object_new_int(getEstado(value)));
			json_object_array_add(isSet,obj);

		} else{
			json_object *obj = json_object_new_object();
			json_object_object_add(obj,"posx",json_object_new_int(getPosX(getIndex(array,i))));
			json_object_object_add(obj,"posy",json_object_new_int(getPosY(getIndex(array,i))));
			json_object_object_add(obj,"estado",json_object_new_int(getEstado(getIndex(array,i))));
			json_object_array_add(isSet,obj);
		}
	}
	return isSet;
}

json_object *setPosY (json_object *array, size_t iterador, int valor){
	json_object *value = getIndex(array,iterador);
	struct json_object *isSet = json_object_new_array();
	size_t len = getLength(array);
	for (size_t i = 0; i < len;i++){
		if (i == iterador){
			json_object *obj = json_object_new_object();
			json_object_object_add(obj,"posx",json_object_new_int(getPosX(value)));
			json_object_object_add(obj,"posy",json_object_new_int(valor));
			json_object_object_add(obj,"estado",json_object_new_int(getEstado(value)));
			json_object_array_add(isSet,obj);

		} else{
			json_object *obj = json_object_new_object();
			json_object_object_add(obj,"posx",json_object_new_int(getPosX(getIndex(array,i))));
			json_object_object_add(obj,"posy",json_object_new_int(getPosY(getIndex(array,i))));
			json_object_object_add(obj,"estado",json_object_new_int(getEstado(getIndex(array,i))));
			json_object_array_add(isSet,obj);
		}
	}
	return isSet;
}

json_object *setEstado (json_object *array, size_t iterador, int valor){
	json_object *value = getIndex(array,iterador);
	struct json_object *isSet = json_object_new_array();
	size_t len = getLength(array);
	for (size_t i = 0; i < len;i++){
		if (i == iterador){
			json_object *obj = json_object_new_object();
			json_object_object_add(obj,"posx",json_object_new_int(getPosX(value)));
			json_object_object_add(obj,"posy",json_object_new_int(getPosY(value)));
			json_object_object_add(obj,"estado",json_object_new_int(valor));
			json_object_array_add(isSet,obj);

		} else{
			json_object *obj = json_object_new_object();
			json_object_object_add(obj,"posx",json_object_new_int(getPosX(getIndex(array,i))));
			json_object_object_add(obj,"posy",json_object_new_int(getPosY(getIndex(array,i))));
			json_object_object_add(obj,"estado",json_object_new_int(getEstado(getIndex(array,i))));
			json_object_array_add(isSet,obj);
		}
	}
	return isSet;
}
/*<<<<<<<<<<----------------------------->>>>>>>>>>
								Funciones para crear un objeto
Pone de un vez un objeto completo de la forma "tag" : valor, ya sea entero, string, array,
alien, crear un json completo, crear lista de aliens,

<<<<<<<<<<----------------objeto por index, largo de un arreglo, valor de una key (json_object),
		valor de una key (entero), obtener un alien por fila y columna,
								leer un archivo json, añadir un elemento a una lista
	------------>>>>>>>>>> */

//Crea un objeto entero
json_object *crearEntero (const char *key, int value){
	json_object *jobj = json_object_new_object();
	json_object *entero = json_object_new_int(value);
	json_object_object_add(jobj,key, entero);
	return jobj;
}
// Crea un objeto string
json_object *crearString (const char *key, const char *value){
	json_object *jobj = json_object_new_object();
	json_object *string = json_object_new_string(value);
	json_object_object_add(jobj,key, string);
	return jobj;
}
// Crea un objeto array
json_object *crearArray (const char *key, json_object *array){
	json_object *jobj = json_object_new_object();
	json_object_object_add(jobj,key, array);
	return jobj;
}
// Crea un alien
json_object *crearAlien (int posx, int posy, int estado, const char *tipo){
	json_object *jobj = json_object_new_object();
	json_object_object_add(jobj, "posx", json_object_new_int(posx));
	json_object_object_add(jobj, "posy", json_object_new_int(posy));
	json_object_object_add(jobj, "estado", json_object_new_int(estado));
	json_object_object_add(jobj, "tipo", json_object_new_string(tipo));
	return jobj;
}
// Genera un json completo
json_object *crearJson (int vidas, int velocidad, int puntos, json_object *aliens){ //Falta añadir bunker
	json_object *jobj = json_object_new_object();
	//Aliens es un array
	json_object_object_add(jobj, "vidas", json_object_new_int(vidas));
	json_object_object_add(jobj, "velocidad", json_object_new_int(velocidad));
	json_object_object_add(jobj, "puntos", json_object_new_int(puntos));
	json_object_object_add(jobj, "aliens", aliens);
	return jobj;
}
// Crea la lista completa de todos los aliens
json_object *crearAliens (json_object *fila1, json_object *fila2) {

	json_object *aliens_array = json_object_new_array();
	json_object_array_add(aliens_array,crearArray("fila", fila1));
	json_object_array_add(aliens_array,crearArray("fila", fila2));
	return aliens_array;
}
// Añade un alien a una fila
void addElemento(json_object *array, json_object *elemento){
	json_object_array_add(array,elemento);
}

/*int main(int argc, char **argv) {

	json_object *array1 = json_object_new_array();
	json_object *array2 = json_object_new_array();

	addElemento(array1, crearAlien(101,202,1,"calamar"));
	addElemento(array1, crearAlien(120,520,0,"pulpo"));
	addElemento(array2, crearAlien(200,10,1,"cangrejo"));
	addElemento(array2, crearAlien(200,10,1,"calamar"));

	json_object *json = crearJson(4,250000,240,crearAliens(array1,array2));

	struct json_object *alien;
	alien = getAlien(0,1,json);
	printf("%d\n", getValue_Entero(json,"puntos"));

}*/

/* Ejemplo de como leer un archivo y obtener un alien en especifico segun fila y columna
struct json_object *parsed_json;
struct json_object *alien;
parsed_json = getJson("archivo.json");
alien = getAlien(2,1,parsed_json); acá si no leo el archivo lo que paso es json object que ya tenga todo el json
printf("%d\n", getPosX(alien));
*/


/* Ejemplo de como crear un json completo
json_object *jobj = json_object_new_object();

json_object *array1 = json_object_new_array();
json_object *array2 = json_object_new_array();
json_object *array3 = json_object_new_array();

json_object *alien = crearAlien(101,202,1,"calamar");
json_object *alien1 = crearAlien(120,520,0,"pulpo");
json_object *alien2 = crearAlien(200,10,1,"cangrejo");

json_object_array_add(array1,alien);
json_object_array_add(array2,alien1);
json_object_array_add(array2,alien2);

json_object_array_add(array3,crearArray("fila", array1));
json_object_array_add(array3,crearArray("fila", array2));

json_object_object_add(jobj, "aliens", array3);
json_object_object_add(jobj, "vidas", json_object_new_int(10));

printf("%s\n", json_object_get_string(jobj));
*/
