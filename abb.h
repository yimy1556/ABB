#ifndef _ABB_H
#define _ABB_H
#include <stddef.h>
#include <stdbool.h>

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);


bool abb_guardar(abb_t *arbol, const char *clave, void *dato);


void *abb_borrar(abb_t *arbol, const char *clave);


void *abb_obtener(const abb_t *arbol, const char *clave);


bool abb_pertenece(const abb_t *arbol, const char *clave);


size_t abb_cantidad(abb_t *arbol);


void abb_destruir(abb_t *arbol);

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

#endif // _ABB_H
