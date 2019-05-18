#include "cola.h"                                         
#include <stdlib.h>

typedef struct nodo{
    char* dato;
    struct nodo* siguiente_nodo;
}nodo_t;

struct cola{
    nodo_t* primer_nodo;
    nodo_t* ultimo_nodo;
};                                                                 
 
cola_t* cola_crear(void){
    cola_t* cola = malloc(sizeof(cola_t));
    if(!cola) return NULL; 
    cola->primer_nodo = NULL;
    cola->ultimo_nodo = NULL;
    return cola;
}


nodo_t* crear_nodo(char* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(!nodo) return NULL;
    nodo->dato = valor;
    nodo->siguiente_nodo = NULL;
    return nodo;
}

void cola_destruir(cola_t *cola,void destruir_dato(void*)){
    while(!cola_esta_vacia(cola)){
       if(destruir_dato) destruir_dato(cola_desencolar(cola));
       else{ 
           cola_desencolar(cola);
       } 
    }
    free(cola); 
}

bool cola_esta_vacia(const cola_t *cola){
    return !cola->primer_nodo;
}

bool cola_encolar(cola_t *cola, char* valor){
    nodo_t* nuevo_nodo = crear_nodo(valor);
    if (!nuevo_nodo) return false;
    if(!cola_esta_vacia(cola)) cola->ultimo_nodo->siguiente_nodo = nuevo_nodo;
    cola->ultimo_nodo = nuevo_nodo;
    if(cola_esta_vacia(cola)) cola->primer_nodo = nuevo_nodo;
    return  true;
}

char* cola_ver_primero(const cola_t *cola){
    if(cola_esta_vacia(cola)) return NULL;
    return cola->primer_nodo->dato;
}


char* cola_desencolar(cola_t *cola){
    if(cola_esta_vacia(cola)) return NULL;
    nodo_t* dirrecion_aux = cola->primer_nodo->siguiente_nodo;
    char* dato_nodo = cola_ver_primero(cola);
    free(cola->primer_nodo);
    cola->primer_nodo = dirrecion_aux;
    if (cola_esta_vacia(cola)) cola->ultimo_nodo = NULL;
    return dato_nodo;
}

