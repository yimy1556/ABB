#ifndef _ABB_H
#define _ABB_H
#include <stddef.h>
#include <stdbool.h>

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/*
*crea la estructura del abb 
*
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato); /// ya esta hecho (Y)

/*
*pre:el arbol fue creado
*post: se guardo en el arbol el dato asociado a la clave devuelve TRUE, si no se puede guardar devuelve false.
*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/*
*pre:el arbol fue creado
*post: devuelve el valor correspondiente a la clave borrada,si la clave no pertenece devuelve NULL.
*/
void *abb_borrar(abb_t *arbol, const char *clave);

/*
*pre:el arbol fue creado
*post: devuelve el valor correspondiente a la clave ,si la clave no pertenece devuelve NULL.
*/
void *abb_obtener(const abb_t *arbol, const char *clave); //ya hecho(Y)

/*
*pre:el arbol fue creado
*post: devuelve TRUE si la clave pertenece al arbol,si no pertenece devuelve FALSE.
*/
bool abb_pertenece(const abb_t *arbol, const char *clave);

/*
*pre:el arbol fue creado
*post: devuelve la cantidad de elementos que contiene el arbol.
*/
size_t abb_cantidad(abb_t *arbol);	// ya hecho (Y)

/*
*pre:el arbol fue creado
*post: destruye el abb en su totalidad.
*/
void abb_destruir(abb_t *arbol);

/*
*pre:el arbol fue creado
*post: destruye el abb en su totalidad.
*/
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

<<<<<<< HEAD
/*
*pre:el arbol fue creado
*post: crea el iterador para el abb
*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);
/*
*pre:el iterador fue creado
*post: avanza al siguiente elemento del arbol
*/
bool abb_iter_in_avanzar(abb_iter_t *iter);
/*
*pre:el iterador fue creado
*post: devuelve el elemento actual donde se encuentra parado el iterador, si no hay arbol devuelve NULL.
*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
/*
*pre:el iterador fue creado
*post: si el iterador esta al final devuelve TRUE, en caso contrario devuelve FALse
*/
bool abb_iter_in_al_final(const abb_iter_t *iter);
/*
*pre:el iterador fue creado
*post: destruye el abb en su totalidad.
*/
void abb_iter_in_destruir(abb_iter_t* iter);












#endif // _ABB_H
