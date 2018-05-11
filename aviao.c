#include <stdlib.h>
#include "aviao.h"

/**
 * aviao.c
 * Implementação das funções do aviao.h
 * Descrições em aviao.h
 **/

aviao_t * aloca_aviao (size_t combustivel, size_t id) {
  aviao_t* aviao = malloc(sizeof(aviao_t));
  aviao->combustivel = combustivel;
  aviao->id = id;
  pthread_mutex_init(&aviao->estado, NULL);
  return aviao;
}

void desaloca_aviao(aviao_t* aviao) {
  pthread_mutex_destroy(&aviao->estado);
  free(aviao);
}
