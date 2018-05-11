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
  pthread_mutex_lock(&aviao->estado); // Avião voando
  pthread_create(&aviao->thread, NULL, aviao_func, &aviao);
  return aviao;
}

void* aviao_func(void*arg) {
  // TODO Fazer a implementação da função da thread
  // Avião está voando e esperando liberar pista de voo
  pthread_mutex_lock(&aviao->estado);
  // Pista foi liberada e está em processo de pouso
  pthread_mutex_lock(&aviao->estado);
  // Avião pousou e está em processo de acoplagem
  pthread_mutex_lock(&aviao->estado);
  // Avião está acoplado e esperando o transporte de bagagem terminar
  pthread_mutex_lock(&aviao->estado);
  // Avião está em processo de decolagem
  pthread_mutex_lock(&aviao->estado);
  // Avião decolou e está no ar novamente
}

void desaloca_aviao(aviao_t* aviao) {
  free(aviao);
}
