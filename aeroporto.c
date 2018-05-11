#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
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
	sem_init(&airport->pistas_livres, 0 , airport->n_pistas);
	sem_init(&airport->portoes_livres, 0 , airport->n_portoes);
	airport->esteiras_livres = malloc(sizeof(sem_t)*airport->n_esteiras);
	for(int i = 0; i < airport->n_esteiras; i++) {
		sem_init(&airport->esteiras_livres[i], 0 , airport->n_max_avioes_esteira);
	}
	airport->avioes_aproximando = criar_fila();
	airport->avioes_prioridade = criar_fila();
	airport->avioes_no_aeroporto = 0;
	pthread_mutex_init(&airport->avioes_mutex, NULL);
	return airport;
}

// Ta certo
void aproximacao_aeroporto (aeroporto_t* aeroporto, aviao_t* aviao) {
	pthread_mutex_lock(&aeroporto->avioes_mutex);
	aeroporto->avioes_no_aeroporto++;
	pthread_mutex_unlock(&aeroporto->avioes_mutex);
	pthread_mutex_lock(&aviao->estado);
	printf("Aproximacao do avião de id %i com %i de combustivel\n",(int)aviao->id, (int)aviao->combustivel);
	if(aviao->combustivel <= 1) {
		inserir(aeroporto->avioes_prioridade, aviao);
	} else {
		inserir(aeroporto->avioes_aproximando, aviao);
	}
}

// Ta certo
void pousar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
	sem_wait(&aeroporto->pistas_livres); // Aloca pista
	usleep(aeroporto->t_pouso_decolagem);
	sem_post(&aeroporto->pistas_livres);
	printf("Aviao %i pousado com sucesso! Esperando portão\n", (int) aviao->id);
	sem_wait(&aeroporto->portoes_livres);
	pthread_mutex_unlock(&aviao->estado);
}

void acoplar_portao (aeroporto_t* aeroporto, aviao_t* aviao) {
	pthread_mutex_lock(&aviao->estado);
	printf("Aviao %i acoplado\n", (int) aviao->id);
	pthread_mutex_unlock(&aviao->estado);
}

void transportar_bagagens (aeroporto_t* aeroporto, aviao_t* aviao) {
	printf("Avioes no aeroporto 1 %i\n", (int)aeroporto->avioes_no_aeroporto);
	pthread_mutex_lock(&aviao->estado);
	printf("Avioes esteiras %i\n", (int)aeroporto->n_esteiras);
	int i = 0;

	for(i = 0; i < aeroporto->n_esteiras; i++) {

		int valor;
		sem_getvalue(&aeroporto->esteiras_livres[i], &valor);
		printf("Avioes esteiras %i\n", valor);
		if(i < aeroporto->n_esteiras - 1){
			if(sem_trywait(&aeroporto->esteiras_livres[i]) == 0)
				break;
		} else {
			sem_wait(&aeroporto->esteiras_livres[i]);
			break;
		}
	}
	usleep(aeroporto->t_remover_bagagens);
	adicionar_bagagens_esteira(aeroporto, aviao);
	usleep(aeroporto->t_inserir_bagagens);
	printf("Transporte de Aviao %i\n", (int) aviao->id);
	sem_post(&aeroporto->esteiras_livres[i]);
	pthread_mutex_unlock(&aviao->estado);
}

void adicionar_bagagens_esteira (aeroporto_t* aeroporto, aviao_t* aviao) {
	usleep(aeroporto->t_bagagens_esteira);
	printf("Bagagem adicionada para aviao %i\n", (int) aviao->id);
}

void decolar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
	pthread_mutex_lock(&aviao->estado);
	printf("Aviao %i decolando\n", (int) aviao->id);
	sem_post(&aeroporto->portoes_livres);
	//Aloca pista
	sem_wait(&aeroporto->pistas_livres);
	usleep(aeroporto->t_pouso_decolagem);
	// Desaloca pista
	free(aviao);
	printf("Aviao %i no ar\n", (int) aviao->id);
	pthread_mutex_lock(&aeroporto->avioes_mutex);
	aeroporto->avioes_no_aeroporto--;
	pthread_mutex_unlock(&aeroporto->avioes_mutex);
	sem_post(&aeroporto->pistas_livres);
	pthread_mutex_unlock(&aviao->estado);
}

int finalizar_aeroporto (aeroporto_t* aeroporto) {
	sem_destroy(&aeroporto->pistas_livres);
	sem_destroy(&aeroporto->portoes_livres);
	pthread_mutex_destroy(&aeroporto->avioes_mutex);
	for(int i = 0; i < aeroporto->n_esteiras; i++) {
		sem_destroy(&aeroporto->esteiras_livres[i]);
	}
	free(aeroporto->avioes_aproximando);
	free(aeroporto->avioes_prioridade);
	free(aeroporto->esteiras_livres);
	free(aeroporto);

	return 0;
}
