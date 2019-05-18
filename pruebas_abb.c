#include "abb.h"
#include "testing.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void pruebas(){
    abb_t* arbol = abb_crear(strcmp,NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba hash insertar clave1", abb_guardar(arbol, clave1, valor1));
    print_test("Prueba hash insertar clave1", abb_guardar(arbol, clave2, valor2));
    print_test("Prueba hash insertar clave1", abb_guardar(arbol, clave3, valor3));

    print_test("Prueba hash la cantidad de elementos es 1", abb_cantidad(arbol) == 3);
    print_test("Prueba hash obtener clave1 es valor1", abb_obtener(arbol, clave1) == valor1);
    print_test("Prueba hash obtener clave1 es valor1", abb_obtener(arbol, clave2) == valor2);
    print_test("Prueba hash obtener clave1 es valor1", abb_obtener(arbol, clave3) == valor3);
    //print_test("Prueba hash pertenece clave1, es true", hash_pertenece(hash, clave1));
    //print_test("Prueba hash borrar clave1, es valor1", hash_borrar(hash, clave1) == valor1);
    //print_test("Prueba hash la cantidad de elementos es 0", hash_cantidad(hash) == 0);
}
    

int main(){
    pruebas();   
return 0;
}
