#include "abb.h"
#include "testing.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.


/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/



static void prueba_crear_abb_vacio(){
    abb_t* abb = abb_crear(strcmp ,NULL);

    print_test("Prueba abb crear hash vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb , "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_iterar_abb_vacio(){
    abb_t* abb = abb_crear(strcmp,free);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_insertar(){
    abb_t* abb = abb_crear(strcmp ,NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    // Inserta 1 valor y luego lo borra 
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    //Inserta otros 2 valores y no los borra (se destruyen con el hash) 
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba hash insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba hash la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba hash obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba hash obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba hash pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

static void prueba_abb_reemplazar(){
    abb_t* abb = abb_crear(strcmp ,NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    //Inserta 2 valores y luego los reemplaza 
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    printf("%i\n",(int)abb_cantidad(abb));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    printf("%i\n",(int)abb_cantidad(abb));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir(){
    abb_t* abb = abb_crear(strcmp,free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    //Pide memoria para 4 valores
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    //Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza)
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

   // Se destruye el hash (se debe liberar lo que quedó dentro)
    abb_destruir(abb);
}

static void prueba_abb_borrar(){
    abb_t* abb = abb_crear(strcmp,free);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    //Inserta 3 valores y luego los borra 
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));

    //Al borrar cada elemento comprueba que ya no está pero los otros sí. 
    print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(abb, clave3));
    print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave1));
    print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);

    print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave2));
    print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
    print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_clave_vacia(){
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave = "", *valor = "";
    printf("_____prueba_abb_clave_vacia_____\n");
    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_valor_null(){
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave = "", *valor = NULL;

    //Inserta 1 valor y luego lo borra 
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static ssize_t buscar(const char* clave, char* claves[], size_t largo){
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}
bool imprimir_claves(const char* clave, void* dato, void* extra) {
    int* aux = (int*)extra;
    if (!extra ){
        printf("%s \n", clave );
        return true; 
    }else {
       
        if(10 > *aux){
            (*aux)++;
            printf("%s \n",clave );
            return true;
        }
    }
    return false;
}

    

static void prueba_abb_volumen(size_t largo, bool debug){
    printf("prueba_de volumen\n");
    abb_t* abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    /* Verifica que devuelva los valores correctos */
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb , claves[i]) == valores[i];
        if (!ok) break;
    }
  
    if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

       for (size_t i = 0; i < largo; i++) {
        ok = abb_borrar(abb , claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba abb borrar muchos elementos", ok);
    if (debug) print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);


    abb_destruir(abb);
    abb = abb_crear(strcmp, free);

    
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);
    abb_destruir(abb);

}

static void prueba_abb_iterar()
{   printf("_____prueba_abb_iterar_____\n");
    abb_t* abb = abb_crear(strcmp ,NULL);

    char *claves[] = {"2", "5", "3", };
    char *valores[] = {"guau", "miau", "mu"};

    //Inserta 3 valores 
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[2], valores[2]));
    print_test("Prueba abb insertar clave0", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[1], valores[1]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    const char *clave;
    ssize_t indice;

    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    //Primer valor 
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    printf(" clave 0 '%s' \n", claves[0] );
    printf(" clave 1 '%s' \n", claves[1] );
    printf(" clave 2 '%s' \n",claves[2] );
    printf(" ver actual es: '%s' \n", clave );
    print_test("Prueba abb iterador ver actual, ", !strcmp(abb_iter_in_ver_actual(iter), claves[0]));
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    //Segundo valor 
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    //Tercer valor 
    clave = abb_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    
    abb_iter_in_avanzar(iter);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    //Vuelve a tratar de avanzar, por las dudas 
    print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
    print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

void prueba_abb_borrar_dos_hijos(){

    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "1", *valor1 = "guau";
    char *clave2 = "2", *valor2 = "miau";
    char *clave3 = "3", *valor3 = "mu";
    char *clave4 = "4", *valor4 = "mee";
    char *clave5 = "5", *valor5 = "mi";
    char *clave6 = "6", *valor6 = "pio";
    char *clave7 = "7", *valor7 = "SEVEN";
    printf(" guardar 1, 2, 3 \n");
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave2, valor2));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave1, valor1));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave3, valor3));
   
    printf("borra 2 me devuelve el dato asociado\n");

    print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave2)== valor2 );

    abb_destruir(abb);

    abb= abb_crear( strcmp, NULL);
    printf(" guardarRE  1 2 4 3 5 6 7 \n");
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave1, valor1));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave2, valor2));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave4, valor4));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave3, valor3));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb, clave5, valor5));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave6, valor6));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave7, valor7));
    
    printf("AHORA BORRARE EL 1,2 Y 4\n");
    print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave1)== valor1 );
    print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave2)== valor2 );
    print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave4)== valor4 );

    abb_destruir(abb);
    abb= abb_crear( strcmp, NULL);
    printf(" guardarRE  1 2 4 3 5 6 7 \n");
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave1, valor1));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave2, valor2));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave4, valor4));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave3, valor3));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb, clave5, valor5));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave6, valor6));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave7, valor7));

    printf("AHORA BORRARE EL 1 Y 4\n");
    print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave1)== valor1 );
    //print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave2)== valor2 );
    print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave4)== valor4 );
    abb_destruir(abb);

    abb= abb_crear( strcmp, NULL);
    printf(" guardarRE  1 2 4 3 5 6 7 \n");
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave1, valor1));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave2, valor2));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave4, valor4));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave3, valor3));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb, clave5, valor5));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave6, valor6));
    print_test("Prueba abb iteración en volumen", abb_guardar(abb , clave7, valor7));

    printf("AHORA BORRARE EL 2, 1 Y 4\n");
    print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave2)== valor2 );
    print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave1)== valor1 );
    print_test("Prueba abb iteración en volumen", abb_borrar(abb , clave4)== valor4 );
    abb_destruir(abb);

}

void prueba_abb_iterar_volumen(size_t largo){
    printf("prueba_de volumen iterador\n");
    abb_t* abb = abb_crear(strcmp, free);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    print_test("Prueba abb almacenar muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

    
    for (size_t i = 0; i < largo; i++) {
        ok = abb_pertenece(abb, claves[i]);
        if (!ok) break;
        ok = abb_obtener(abb , claves[i]) == valores[i];
        if (!ok) break;
    }
   
    print_test("Prueba abb pertenece y obtener muchos elementos", ok);
    print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);
    int contador = 0;
    abb_in_order(abb, imprimir_claves , &contador );
    free(claves);
    abb_destruir(abb);
    
}
/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_abb_alumno()
{
    
    // Ejecuta todas las pruebas unitarias. 
    prueba_crear_abb_vacio();
    prueba_iterar_abb_vacio();
    prueba_abb_insertar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_borrar();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
    prueba_abb_volumen(500, true);
    prueba_abb_iterar();
    prueba_abb_iterar_volumen(5000);
    prueba_abb_borrar_dos_hijos();
}
