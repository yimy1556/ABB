#include "abb.h"
#include <stdlib.h>
#define INICIO 0

struct nodo{
    struct nodo* izq;
    struct nodo* der;
    char* clave;
    void* dato;
}nodo_t;

struct abb {
    struct nodo* raiz;
    abb_destruir_dato_t destruir_dato;
    abb_comparar_clave_t comparar_clave;
    size_t cantidad;
};

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;
    abb->raiz = NULL;
    abb->destruir_dato = destruir_dato;
    abb->comparar_clave = cmp;    
    abb->cantidad = INICIO;
    return abb;
}

