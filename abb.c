#include "abb.h"
#include <stdlib.h>
#include <string.h>
#define INICIO 0

typedef struct nodo{
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

nodo_t* nodo_crear(const char *clave, void *dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;
    nodo->izq = NULL;
    nodo->der = NULL;
    strcpy(nodo->clave,clave);
    nodo->dato = dato;
    return nodo;
}

/*bool agregar_hoja(nodo_t* nodo ,nodo_t* nuevo_nodo ,int* cantidad){
    if(!nuevo_nodo) return false;
    nodo = nuevo_nodo
    *cantidad++;
    return true;
}*/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* arbol = malloc(sizeof(abb_t));
    if(!arbol) return NULL;
    arbol->raiz = NULL;
    arbol->destruir_dato = destruir_dato;
    arbol->comparar_clave = cmp;    
    arbol->cantidad = INICIO;
    return arbol;
}


nodo_t** busqueda_ptr(nodo_t** ptr_nodo,abb_comparar_clave_t comparar,char* clave){
    if (!(*ptr_nodo)) return ptr_nodo;
    
    int comparacion = comparar((*ptr_nodo)->clave,clave); 
    if (!comparacion) return ptr_nodo;    
    
    if (comparacion < 0) return busqueda_ptr(&((*ptr_nodo)->izq),comparar,clave);
    else return busqueda_ptr(&((*ptr_nodo)->der),comparar,clave);
}
