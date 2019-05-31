#define _POSIX_C_SOURCE 200809L 
#include "abb.h"
#include "cola.h"
#include <stdio.h>
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
    nodo_t* raiz;
    abb_destruir_dato_t destruir_dato;
    abb_comparar_clave_t comparar_clave;
    size_t cantidad;
};

struct abb_iter{
    cola_t* cola;
};
/*  #############################################################################
    ##                       FUNCIONES AUXILIARES                            ####
    #############################################################################
*/
    
/*
*pre:el arbol fue creado
*post: devuelve donde tiene que estar la clave buscada, en caso que la clave no este, 
* el puntero va a apuntar a la direccion donde se supone que deberia estar (el contenido es un puntero NULL)
*/
nodo_t** busqueda_ptr( nodo_t** ptr_nodo, abb_comparar_clave_t comparar, const char* clave ){
    if ( !(*ptr_nodo) ) return ptr_nodo;
    
    int comparacion = comparar( clave, (*ptr_nodo)->clave ); 

    if ( !comparacion ) return ptr_nodo;    
    
    if ( comparacion < 0 ) {
        
        return busqueda_ptr( &((*ptr_nodo)->izq) ,comparar ,clave );
    }
    else {
     
        return busqueda_ptr( &((*ptr_nodo)->der), comparar ,clave );
    }
}

/*
*pre: el nodo fue creado
*post: destruye el nodo pasado contenido en el puntero(se pasa su direccion de memoria).
*/
void* nodo_destruir( nodo_t** nodo_borrar ){
    free( (*nodo_borrar)->clave );
    void* dato = (*nodo_borrar)->dato;
    free(*nodo_borrar);
    *nodo_borrar = NULL;
    return dato;
}
/*
*pre:el arbol fue creado
*post: destruye todas las hojas del arbol(destruye el arbol).
*/
void destruir_hojas(nodo_t* raiz ,abb_destruir_dato_t destruir_dato){
    if(!raiz) return;
    if(raiz->izq) destruir_hojas(raiz->izq,destruir_dato);
    if(raiz->der) destruir_hojas(raiz->der,destruir_dato);
    free(raiz->clave);                         //---|   
    if(destruir_dato) destruir_dato(raiz->dato); //-|-> modoficar
    free(raiz);                                  //-|
}
/*
*post: crea un nodo inicializando sus campos, en caso de no crearse, devuelve NULL.
*/
nodo_t* nodo_crear( const char *clave, void *dato ){
    nodo_t* nodo = malloc( sizeof(nodo_t) );
    if( !nodo ) return NULL;
    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->clave = strdup( clave );
    nodo->dato = dato;
    return nodo;
}
/*
*
*post: se intercambiaron la clave y el dato de ambas estructuras. 
*/
void swap( nodo_t* nodo_a , nodo_t *nodo_b ){
    char* clave_aux = nodo_b->clave;
    void* dato_aux = nodo_b->dato; 
    nodo_b->clave = nodo_a->clave;
    nodo_b->dato = nodo_a->dato;
    nodo_a->clave = clave_aux;
    nodo_a->dato = dato_aux;
}
/*
*
*post: devolvio el mayor de sus menores del nodo pasado por parametro.
*/

nodo_t** obtener_mayor_de_menores(nodo_t** nodo){
    if(!(*nodo)->der) return nodo;    
    return obtener_mayor_de_menores(&(*nodo)->der);
}

/*
*pre:el arbol fue creado
*post: devuelve el dato asociado al nodo pasado por parametro, dentro del puntero.
*/
void* borrar_nodo_con_un_hijo(nodo_t** nodo_borrar){
    nodo_t* nodo_hijo = ((*nodo_borrar)->izq)? (*nodo_borrar)->izq : (*nodo_borrar)->der;
    void* dato = nodo_destruir(nodo_borrar);
    *nodo_borrar = nodo_hijo;
    return dato;
}
/*
*post: borro la clave y el dato haciendo todos los cambios correspondientes
*/
void* borrar_nodo_con_dos_hijos(nodo_t** nodo_a_borrar){
    nodo_t** nodo_swap = obtener_mayor_de_menores( &(*nodo_a_borrar)->izq) ;  //aca obtiene el nodo reemplazante

    swap(*nodo_swap,*nodo_a_borrar);
    if((*nodo_swap)->izq) return borrar_nodo_con_un_hijo(nodo_swap);
    return nodo_destruir(nodo_swap); 
}
/*
*pre: el arbol esta creado, 
*post: recorre el abb  aplicando la funcion VISITAR hasta que esta se vuelva FALSE,
* sino lo que haga visitar se applica a todo el abb.
*/
void recorrido_in_order(nodo_t* raiz, bool visitar(const char *, void *, void *), void *extra, bool* continuar){
    if(!raiz) return;   
    recorrido_in_order(raiz->izq ,visitar ,extra, continuar);

    if(*continuar){
        *continuar = visitar(raiz->clave ,raiz->dato ,extra);
    }
    if(*continuar) {
            recorrido_in_order(raiz->der ,visitar ,extra, continuar);
    }
    
}   
/*
*post: borro la clave y el dato haciendo todos los cambios correspondientes
*/
bool guardar_en_cola(const char* clave ,void* dato ,void* extra){ 
    dato = dato;        // si no pedantic dice que la variable no se utiliza.
    
    cola_encolar((cola_t*)extra ,strdup(clave));
    return true;
}
    
/*
    #############################################################################
    ##                          LAS PRIMITIVAS                               ####
    #############################################################################
*/

abb_t* abb_crear( abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato ){//YA ESTA VISTO
    abb_t* arbol = malloc( sizeof(abb_t) );
    if(!arbol) return NULL;
    arbol->raiz = NULL;
    arbol->destruir_dato = destruir_dato;
    arbol->comparar_clave = cmp;    
    arbol->cantidad = INICIO;
    return arbol;
}
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if ( !arbol ) return false;
    nodo_t** ptr_aux = busqueda_ptr(&(arbol->raiz),arbol->comparar_clave,clave);
    if( !(*ptr_aux) ){
        *ptr_aux = nodo_crear( clave , dato );
        arbol->cantidad += 1;
    }
    else{
        if( arbol->destruir_dato ) arbol->destruir_dato( (*ptr_aux)->dato );    
        (*ptr_aux)->dato = dato; 
    } 
    return true;    //YA ESTA VISTO
}
void *abb_obtener(const abb_t *arbol, const char *clave){       //YA ESTA VISTO
    if( !arbol ) return NULL;
    nodo_t* raiz_aux = arbol->raiz; 
    nodo_t** ptr_aux = busqueda_ptr(&(raiz_aux) ,arbol->comparar_clave ,clave); 
    return (*ptr_aux)? (*ptr_aux)->dato : NULL;
}
bool abb_pertenece(const abb_t *arbol, const char *clave){ //YA ESTA VISTO

    nodo_t* raiz_aux = arbol->raiz; 
    nodo_t** ptr_aux = busqueda_ptr(&(raiz_aux) ,arbol->comparar_clave ,clave); 
    return *ptr_aux;
}
size_t abb_cantidad( abb_t* arbol ){
    if( !arbol ) return 0;
    return arbol->cantidad; //YA ESTA VISTO
}
void *abb_borrar( abb_t *arbol , const char *clave){
    if ( !arbol ) return NULL;
    nodo_t** nodo_borrar = busqueda_ptr( &(arbol->raiz) , arbol->comparar_clave ,clave );
    if( !(*nodo_borrar) ) return NULL;
    void* dato_a_borrar = NULL;  
    
    if( !(*nodo_borrar)->izq && !(*nodo_borrar)->der )  dato_a_borrar = nodo_destruir( nodo_borrar );

    else if((*nodo_borrar)->izq && (*nodo_borrar)->der) dato_a_borrar =  borrar_nodo_con_dos_hijos(nodo_borrar);

    else dato_a_borrar = borrar_nodo_con_un_hijo(nodo_borrar);
    
    arbol->cantidad -= 1;
    return dato_a_borrar;
}
void abb_destruir( abb_t *arbol ){ //YA ESTA VISTO
    destruir_hojas( arbol->raiz,arbol->destruir_dato );
    free( arbol );
}   


void abb_in_order( abb_t *arbol, bool visitar(const char *, void *, void *), void *extra ){
    bool continuar = true;
    recorrido_in_order( arbol->raiz ,visitar ,extra, &continuar );    
}



abb_iter_t *abb_iter_in_crear( const abb_t *arbol ){
    cola_t* cola = cola_crear();
    if(!cola) return NULL; // estoy en duda si hay que verificar si se creo, sino, no se puede guardar
    bool corte = true;
    recorrido_in_order( arbol->raiz , guardar_en_cola , cola, &corte);
    abb_iter_t* iter_abb = malloc( sizeof(abb_iter_t) );
    if( !iter_abb ) {       // creo que si no se crea el iterador, la cola se destruye.
        cola_destruir( cola, free ); //para destruir las claves guardadas.
        return NULL;
    } 
    iter_abb->cola = cola;
    return iter_abb;
}
bool abb_iter_in_avanzar( abb_iter_t *iter ){
    if( cola_esta_vacia(iter->cola) ) return false;
    free( cola_desencolar(iter->cola) );
    return true;
}

const char *abb_iter_in_ver_actual( const abb_iter_t *iter ){
    return cola_ver_primero( iter->cola );
}
bool abb_iter_in_al_final( const abb_iter_t *iter ){
    return ( cola_esta_vacia(iter->cola) );
}
void abb_iter_in_destruir( abb_iter_t* iter ){
    cola_destruir( iter->cola ,free );
    free(iter);
}
