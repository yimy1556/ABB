#define _POSIX_C_SOURCE 200809L 
#include "abb.h"
#include "cola.h"
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

typedef struct abb_iter{
    cola_t* cola;
}abb_iter_t;


nodo_t** busqueda_ptr(nodo_t** ptr_nodo,abb_comparar_clave_t comparar,const char* clave){
    if (!(*ptr_nodo)) return ptr_nodo;
    
    int comparacion = comparar((*ptr_nodo)->clave,clave); 
    if (!comparacion) return ptr_nodo;    
    
    if (comparacion < 0) return busqueda_ptr(&((*ptr_nodo)->izq),comparar,clave);
    else return busqueda_ptr(&((*ptr_nodo)->der),comparar,clave);
}

void* nodo_destruir(nodo_t** nodo_borrar){
    free((*nodo_borrar)->clave);
    void* dato = (*nodo_borrar)->dato;
    *nodo_borrar = NULL;
    return dato;
}

void destruir_hojas(nodo_t* raiz ,abb_destruir_dato_t destruir_dato){
    if(!raiz) return;
    if(raiz->izq) destruir_hojas(raiz->izq,destruir_dato);
    if(raiz->der) destruir_hojas(raiz->der,destruir_dato);
    free(raiz->clave);                         //---|   
    if(destruir_dato) destruir_dato(raiz->dato); //-|-> modoficar
    free(raiz);                                  //-|
}

nodo_t* nodo_crear(const char *clave, void *dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;
    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->clave = strdup(clave);
    nodo->dato = dato;
    return nodo;
}

void swap(nodo_t* nodo_a ,nodo_t *nodo_b){
    char* clave_aux = nodo_b->clave;
    void* dato_aux = nodo_b->dato; 
    nodo_b->clave = nodo_a->clave;
    nodo_b->dato = nodo_a->dato;
    nodo_a->clave = clave_aux;
    nodo_a->dato = dato_aux;
}

bool guardar_cola(const char* clave ,void* dato ,void* extra){ 
    dato = dato;
    cola_encolar((cola_t*)extra ,strdup(clave));
    return true;
}

nodo_t** recorrido_der_ultimo(nodo_t** nodo_der){
    if(!(*nodo_der)->der) return nodo_der;    
    return recorrido_der_ultimo(&(*nodo_der)->der);
}

void* borrar_nodo_con_un_hijo(nodo_t** nodo_borrar){
    nodo_t* nodo_hijo = ((*nodo_borrar)->izq)? (*nodo_borrar)->izq : (*nodo_borrar)->der;
    void* dato = nodo_destruir(nodo_borrar);
    *nodo_borrar = nodo_hijo;
    return dato;
}

void* borrar_nodo_con_dos_hijos(nodo_t** nodo_a_borrar){
    nodo_t** nodo_swap = recorrido_der_ultimo(&(*nodo_a_borrar)->izq);  
    swap(*nodo_swap,*nodo_a_borrar);
    if((*nodo_swap)->izq) return borrar_nodo_con_un_hijo(nodo_swap);
    return nodo_destruir(nodo_swap); 
}

/*#############################################################################*/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* arbol = malloc(sizeof(abb_t));
    if(!arbol) return NULL;
    arbol->raiz = NULL;
    arbol->destruir_dato = destruir_dato;
    arbol->comparar_clave = cmp;    
    arbol->cantidad = INICIO;
    return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if (!arbol) return false;
    nodo_t** ptr_aux = busqueda_ptr(&(arbol->raiz),arbol->comparar_clave,clave);
    if(!(*ptr_aux)){
        *ptr_aux = nodo_crear(clave ,dato);
        arbol->cantidad += 1;
    }
    else{
        if(arbol->destruir_dato) arbol->destruir_dato((*ptr_aux)->dato);    
        (*ptr_aux)->dato = dato; 
    } 
    return true;
}
void *abb_obtener(const abb_t *arbol, const char *clave){
    if(!arbol) return NULL;
    nodo_t* raiz_aux = arbol->raiz; 
    nodo_t** ptr_aux = busqueda_ptr(&(raiz_aux) ,arbol->comparar_clave ,clave); 
    return (*ptr_aux)? (*ptr_aux)->dato : NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    nodo_t* raiz_aux = arbol->raiz; 
    nodo_t** ptr_aux = busqueda_ptr(&(raiz_aux) ,arbol->comparar_clave ,clave); 
    return *ptr_aux;
}

size_t abb_cantidad(abb_t* arbol){
    if(!arbol) return 0;
    return arbol->cantidad;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if (!arbol) return NULL;
    nodo_t** nodo_borrar = busqueda_ptr(&(arbol->raiz) ,arbol->comparar_clave ,clave);
    if(!(*nodo_borrar)) return NULL;
    void* dato_a_borrar;  
    
    if(!(*nodo_borrar)->izq && !(*nodo_borrar)->der){ 
        dato_a_borrar = nodo_destruir(nodo_borrar);
    }
    else if((*nodo_borrar)->izq && (*nodo_borrar)->der){
        dato_a_borrar = borrar_nodo_con_dos_hijos(nodo_borrar);
    }
    else{
        dato_a_borrar = borrar_nodo_con_un_hijo(nodo_borrar);
    }
    arbol->cantidad -= 1;
    return dato_a_borrar;
}

void abb_destruir(abb_t *arbol){
    destruir_hojas(arbol->raiz,arbol->destruir_dato);
    free(arbol);
}

void recorrido_in_order(nodo_t* raiz, bool visitar(const char *, void *, void *), void *extra){
    if(!raiz) return;
    if(raiz->izq) recorrido_in_order(raiz->izq ,visitar ,extra);
    if(!visitar(raiz->clave ,raiz->dato ,extra)) return;
    if(raiz->der) recorrido_in_order(raiz->der ,visitar ,extra);
}
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    recorrido_in_order(arbol->raiz ,visitar ,extra);    
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    cola_t* cola = cola_crear();
    recorrido_in_order(arbol->raiz ,guardar_cola ,cola);
    abb_iter_t* iter_abb = malloc(sizeof(abb_iter_t));
    if(!iter_abb) return NULL;
    iter_abb->cola = cola;
    return iter_abb;    
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(cola_esta_vacia(iter->cola)) return false;
    free(cola_desencolar(iter->cola));
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    return cola_ver_primero(iter->cola);
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return (cola_esta_vacia(iter->cola));
}

void abb_iter_in_destruir(abb_iter_t* iter){
    cola_destruir(iter->cola ,free);
    free(iter);
}
