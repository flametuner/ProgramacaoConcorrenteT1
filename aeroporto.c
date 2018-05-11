#include "aeroporto.h"

/**
 * aeroporto.c
 * Implementação das funções do aeroporto.h
 * Descrições em aeroporto.h
 **/

aeroporto_t* iniciar_aeroporto (size_t* args, size_t n_args) {
	aeroporto_t* airport = malloc(sizeof(aeroporto_t));
	int i = 0;
	airport->n_pistas = args[i++];
	airport->n_portoes = args[i++];
	airport->n_esteiras = args[i++];
	airport->n_max_avioes_esteira = args[i++];
	airport->t_pouso_decolagem = args[i++];
	airport->t_remover_bagagens = args[i++];
	airport->t_inserir_bagagens = args[i++];
	airport->t_bagagens_esteira = args[i++];
	free(args);
	return airport;
}

void aproximacao_aeroporto (aeroporto_t* aeroporto, aviao_t* aviao) {
	pthread_mutex_unlock(&aviao->estado);
	inserir(aeroporto->avioes_aproximando, aviao);
}

void pousar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {

}

void acoplar_portao (aeroporto_t* aeroporto, aviao_t* aviao) {

}

void transportar_bagagens (aeroporto_t* aeroporto, aviao_t* aviao) {

}

void adicionar_bagagens_esteira (aeroporto_t* aeroporto, aviao_t* aviao) {

}

void decolar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
	aeroporto->
}

int finalizar_aeroporto (aeroporto_t* aeroporto) {
	free(aeroporto);
	return 0;
}
