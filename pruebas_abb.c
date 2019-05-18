#include "abb.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.


/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_abb_vacio()
{
    abb_t* abb = abb_crear(strcmp ,NULL);

    print_test("Prueba abb crear hash vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb , "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_iterar_abb_vacio()
{
    abb_t* abb = abb_crear(strcmp,free);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

static void prueba_abb_insertar()
{
    abb_t* abb = abb_crear(strcmp ,NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el hash) */
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

static void prueba_abb_reemplazar()
{
    abb_t* abb = abb_crear(strcmp ,NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
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

static void prueba_abb_reemplazar_con_destruir()
{
    abb_t* abb = abb_crear(strcmp,free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
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

    /* Se destruye el hash (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}

static void prueba_abb_borrar()
{
    abb_t* abb = abb_crear(strcmp,free);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
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

static void prueba_abb_clave_vacia()
{
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

static void prueba_abb_valor_null()
{
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

/*static void prueba_hash_volumen(size_t largo, bool debug)
{printf("_____prueba_hash_volumen 214_____\n");
    hash_t* hash = hash_crear(NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

     Inserta 'largo' parejas en el hash 
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
        ok = hash_guardar(hash, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba hash almacenar muchos elementos", ok);
    if (debug) print_test("Prueba hash la cantidad de elementos es correcta", hash_cantidad(hash) == largo);

    Verifica que devuelva los valores correctos 
    size_t valor = 0;
    printf("_____aca hay error 236_____\n");
    for (size_t i = 0; i < largo; i++) {
        ok = hash_pertenece(hash, claves[i]);
        if (!ok) break;
        ok = hash_obtener(hash, claves[i]) == valores[i];
        if (!ok) break;
        valor = i;
    }
    printf("V%li#####\n",valor); 
    printf("L%li#####\n",largo);

    if (debug) print_test("Prueba hash pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba hash la cantidad de elementos es correcta", hash_cantidad(hash) == largo);

     Verifica que borre y devuelva los valores correctos 
    for (size_t i = 0; i < largo; i++) {
        ok = hash_borrar(hash, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba hash borrar muchos elementos", ok);
    if (debug) print_test("Prueba hash la cantidad de elementos es 0", hash_cantidad(hash) == 0);

     Destruye el hash y crea uno nuevo que sí libera 
    hash_destruir(hash);
    hash = hash_crear(free);

     Inserta 'largo' parejas en el hash 
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = hash_guardar(hash, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);

     Destruye el hash - debería liberar los enteros 
    hash_destruir(hash);

}

static ssize_t buscar(const char* clave, char* claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}


static void prueba_abb_iterar()
{printf("_____prueba_abb_iterar_____\n");
    abb_t* abb = abb_crear(strcmp ,NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    //Inserta 3 valores 
    print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_crear(abb);
    const char *clave;
    ssize_t indice;

    print_test("Prueba hash iterador esta al final, es false", !hash_iter_al_final(iter));

    //Primer valor 
    clave = abb_iter_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba hash iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba hash iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba hash iterador avanzar es true", hash_iter_avanzar(iter));
    print_test("Prueba hash iterador esta al final, es false", !hash_iter_al_final(iter));

     Segundo valor 
    clave = hash_iter_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba hash iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba hash iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba hash iterador avanzar es true", hash_iter_avanzar(iter));
    print_test("Prueba hash iterador esta al final, es false", !hash_iter_al_final(iter));

     Tercer valor 
    clave = hash_iter_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba hash iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba hash iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    printf(" hasta aca se llego linea 321 \n" );
    hash_iter_avanzar(iter);
    print_test("Prueba hash iterador esta al final, es true", hash_iter_al_final(iter));

     Vuelve a tratar de avanzar, por las dudas 
    print_test("Prueba hash iterador ver actual, es NULL", !hash_iter_ver_actual(iter));
    print_test("Prueba hash iterador avanzar es false", !hash_iter_avanzar(iter));
    print_test("Prueba hash iterador esta al final, es true", hash_iter_al_final(iter));

    hash_iter_destruir(iter);
    hash_destruir(hash);
}

static void prueba_hash_iterar_volumen(size_t largo)
{printf("_____prueba_de volumen _____\n");
    hash_t* hash = hash_crear(NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t valores[largo];

     Inserta 'largo' parejas en el hash 
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        sprintf(claves[i], "%08d", i);
        valores[i] = i;
        ok = hash_guardar(hash, claves[i], &valores[i]);
        if (!ok) break;
    }

    // Prueba de iteración sobre las claves almacenadas.
    hash_iter_t* iter = hash_iter_crear(hash);
    print_test("Prueba hash iterador esta al final, es false", !hash_iter_al_final(iter));

    ok = true;
    unsigned i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( hash_iter_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = hash_iter_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = hash_obtener(hash, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        *valor = largo;
        hash_iter_avanzar(iter);
    }
    print_test("Prueba hash iteración en volumen", ok);
    print_test("Prueba hash iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba hash iterador esta al final, es true", hash_iter_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( valores[i] != largo ) {
            ok = false;
            break;
        }
    }
    print_test("Prueba hash iteración en volumen, se cambiaron todo los elementos", ok);

    free(claves);
    hash_iter_destruir(iter);
    hash_destruir(hash);
}
*/
/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/


void pruebas_abb()
{
    /* Ejecuta todas las pruebas unitarias. */
    prueba_crear_abb_vacio();
    prueba_iterar_abb_vacio();
    prueba_abb_insertar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_borrar();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
//prueba_hash_volumen(12500, true);
  //  prueba_hash_iterar();
//    prueba_hash_iterar_volumen(5000);
}

/*void pruebas_volumen_catedra(size_t largo)
{
    prueba_hash_volumen(largo, false);
}*/

