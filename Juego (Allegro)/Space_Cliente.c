#include <stdio.h>
#include "../Estructura.c"

int main () {
    char asd[2][10];
     for (int i = 0; i < 2 ; i++){
       for (int j = 0; j < 10; j++) {

         asd[i][j] = i+j;
       }
     }
json_object *array = json_object_new_array();
for (int i = 0; i < 2 ; i++){
  for (int j = 0; j < 10; j++) {
    json_object *jobj = json_object_new_object();
    json_object_object_add(jobj,"posx",json_object_new_int(i));
    json_object_object_add(jobj,"posy",json_object_new_int(j));
    json_object_object_add(jobj,"estado",json_object_new_int(1));
    json_object_array_add(array,jobj);
  }
}
printf("1: %s\n", json_object_get_string(array));
array = setPosY(array,0,100);
printf("2: %s\n", json_object_get_string(array));
}
