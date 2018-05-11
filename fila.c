#include <stdlib.h>
#include "fila.h"

/**
 * fila.c
 * Implementação das funções da fila.h
 * Descrições em fila.h
 **/

 elemento_t * aloca_elemento (aviao_t * dado) {
    elemento_t* elemento = malloc(sizeof(elemento_t));
    elemento->dado = dado;
    return elemento;
 }

 void desaloca_elemento (elemento_t * elemento) {
   free(elemento);
 }

fila_ordenada_t * criar_fila () {
    fila_ordenada_t* fila = malloc(sizeof(fila_ordenada_t));
    fila->n_elementos = 0;
    return fila;
}

void desaloca_fila (fila_ordenada_t * fila) {
  free(fila);
}

void inserir (fila_ordenada_t * fila, aviao_t * dado) {
  elemento_t* elemento = aloca_elemento(dado);
  if(fila->n_elementos == 0) {
    fila->primeiro = elemento;
    fila->ultimo = elemento;
  } else {
    fila->ultimo->proximo = elemento;
    elemento->anterior = fila->ultimo;
    fila->ultimo = elemento;
  }
  fila->n_elementos++;
}

aviao_t * remover (fila_ordenada_t * fila) {
  aviao_t* aviao;
  if(fila->n_elementos > 1) {
    elemento_t* t = fila->primeiro;
    aviao = t->dado;
    fila->primeiro = t->proximo;
    desaloca_elemento(t);
  } else {
    aviao = fila->primeiro->dado;
    fila->primeiro = NULL;
    fila->ultimo = NULL;
  }
  fila->n_elementos--;
  return aviao;
}
