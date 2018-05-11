#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "aeroporto.h"

#define NOVO_AVIAO_MIN 30
#define NOVO_AVIAO_MAX 120
#define COMBUSTIVEL_MIN 1
#define COMBUSTIVEL_MAX 10
#define TEMPO_POUSO_DECOLAGEM 40
#define TEMPO_REMOVER_BAGAGENS 90
#define TEMPO_INSERIR_BAGAGENS 110
#define TEMPO_BAGAGENS_ESTEIRA 200
#define TEMPO_SIMULACAO 10000


typedef struct {
  aeroporto_t* aeroporto;
  aviao_t* aviao;
} thread_arg;

void* aviao_func(void*arg) {
  thread_arg* aeroviao = (thread_arg*)arg;
	aviao_t* aviao = aeroviao->aviao;
	aeroporto_t* aeroporto = aeroviao->aeroporto;
  // TODO Fazer a implementação da função da thread
  // Avião está voando e esperando liberar pista de voo
  aproximacao_aeroporto(aeroporto, aviao);
  // Avião pousou e está em processo de acoplagem
	acoplar_portao(aeroporto, aviao);
  // Avião está acoplado e esperando o transporte de bagagem terminar
	transportar_bagagens(aeroporto, aviao);
  // Avião está em processo de decolagem
	decolar_aviao(aeroporto, aviao);
  // Avião decolou e está no ar novamente
	free(aeroviao);
	return NULL;
}

int main (int argc, char** argv) {

	// Variáveis temporais (inicio t_)
	size_t t_novo_aviao_min, t_novo_aviao_max;
	size_t t_pouso_decolagem;
	size_t t_remover_bagagens, t_inserir_bagagens;
	size_t t_bagagens_esteira, t_simulacao;

	// Variáveis discretas (inicio n_)
	size_t n_pistas, n_portoes;
	size_t n_max_avioes_esteira, n_esteiras;
	size_t n_args;

	// Variáveis de prioridade (inicio p_)
	size_t p_combustivel_min, p_combustivel_max;

	if (argc == 5) { // Argumentos sem tempos de execução
		t_novo_aviao_min = NOVO_AVIAO_MIN;
		t_novo_aviao_max = NOVO_AVIAO_MAX;
		t_pouso_decolagem = TEMPO_POUSO_DECOLAGEM;
		t_remover_bagagens = TEMPO_REMOVER_BAGAGENS;
		t_inserir_bagagens = TEMPO_INSERIR_BAGAGENS;
		t_bagagens_esteira = TEMPO_BAGAGENS_ESTEIRA;
		t_simulacao = TEMPO_SIMULACAO;
		p_combustivel_min = COMBUSTIVEL_MIN;
		p_combustivel_max = COMBUSTIVEL_MAX;
		n_pistas = atoi(argv[1]);
		n_portoes = atoi(argv[2]);
		n_max_avioes_esteira = atoi(argv[3]);
		n_esteiras = atoi(argv[4]);

	} else if (argc == 14) { // Argumentos com tempos de execução
		int i = 0; // Este contador será incrementado antes de coletar um argumento
		t_novo_aviao_min = atoi(argv[++i]);
		t_novo_aviao_max = atoi(argv[++i]);
		p_combustivel_min = atoi(argv[++i]);
		p_combustivel_max = atoi(argv[++i]);
		t_pouso_decolagem = atoi(argv[++i]);
		n_pistas = atoi(argv[++i]);
		t_remover_bagagens = atoi(argv[++i]);
		t_inserir_bagagens = atoi(argv[++i]);
		n_portoes = atoi(argv[++i]);
		n_max_avioes_esteira = atoi(argv[++i]);
		t_bagagens_esteira = atoi(argv[++i]);
		n_esteiras = atoi(argv[++i]);
		t_simulacao = atoi(argv[++i]);

	} else { // Número incorreto de argumentos
		printf("Todas as entradas são inteiros positivos!!\nUso:\n");
		printf("./aeroporto  NOVO_AVIAO_MIN  NOVO_AVIAO_MAX\n");
		printf("COMBUSTIVEL_MIN COMBUSTIVEL_MAX\n");
		printf("TEMPO_POUSO_DECOLAGEM  NUMERO_PISTAS  TEMPO_REMOVER_BAGAGENS\n");
		printf("TEMPO_INSERIR_BAGAGENS  NUMERO_PORTOES  MAXIMO_AVIOES_ESTEIRA\n");
		printf("TEMPO_BAGAGENS_ESTEIRA  NUMERO_ESTEIRAS  TEMPO_SIMULACAO\n");
		printf("----------OU----------\n");
		printf("./airport  NUMERO_PISTAS  NUMERO_PORTOES  MAXIMO_AVIOES_ESTEIRA  NUMERO_ESTEIRAS\n");
		return 0;
	}

	// Impressão com os parâmetros selecionados para simulação
	printf("Simulação iniciada com tempo total: %lu\n", t_simulacao);
	printf("Tempo para criação de aviões: %lu - %lu\n", t_novo_aviao_min, t_novo_aviao_max);
	printf("Número de pistas de pouso: %lu\n", n_pistas);
	printf("Tempo de pouso e decolagem: %lu\n", t_pouso_decolagem);
	printf("Número de portões de embarque: %lu\n", n_portoes);
	printf("Tempo de inserção (%lu) e remoção (%lu) de bagagens\n", t_inserir_bagagens, t_remover_bagagens);
	printf("Número de esteiras: %lu, com %lu aviões por esteira\n", n_esteiras, n_max_avioes_esteira);
	printf("Tempo das bagagens nas esteiras: %lu\n", t_bagagens_esteira);

	// Inicialização do aeroporto
	n_args = 8;
	size_t args[8] = {n_pistas, n_portoes, n_esteiras,
				n_max_avioes_esteira,
				t_pouso_decolagem, t_remover_bagagens,
				t_inserir_bagagens, t_bagagens_esteira};

	aeroporto_t* meu_aeroporto = iniciar_aeroporto(args, n_args);
	// Descreve aqui sua simulação usando as funções definidas no arquivo "aeroporto.h"
	// Lembre-se de implementá-las num novo arquivo "aeroporto.c"
	int running = 1;
	int ticks = 0;
	srand(time(NULL));
	int gerarAviao = t_novo_aviao_min + (rand() % (t_novo_aviao_max - t_novo_aviao_min));
	int id = 0;
	while(running) {
		// Gerador de aviões
		if(gerarAviao-- <= 0 && ticks < t_simulacao) { // Gerar apenas enquanto tiver rodando a simuação
			gerarAviao = t_novo_aviao_min + (rand() % (t_novo_aviao_max - t_novo_aviao_min));
			int combustivel = p_combustivel_min + (rand()%(p_combustivel_max - p_combustivel_min));
			aviao_t* aviao = aloca_aviao(combustivel, id++);

			thread_arg* argd = malloc(sizeof(thread_arg));
			argd->aviao = aviao;
			argd->aeroporto = meu_aeroporto;
			pthread_create(&aviao->thread, NULL, aviao_func, &(*argd));
		}

		// Libera pouso de aviões
		if(meu_aeroporto->avioes_prioridade->n_elementos > 0) {

			int valor;
			sem_getvalue(&meu_aeroporto->pistas_livres, &valor);
			while(valor <= meu_aeroporto->n_pistas && meu_aeroporto->avioes_prioridade->n_elementos > 0) {
				aviao_t* next = remover (meu_aeroporto->avioes_prioridade);
				// Pista foi liberada e está em processo de pouso
				pousar_aviao(meu_aeroporto, next);
			}
		} else if(meu_aeroporto->avioes_aproximando->n_elementos > 0) {

			int valor;
			sem_getvalue(&meu_aeroporto->pistas_livres, &valor);
			while(valor < meu_aeroporto->n_pistas && meu_aeroporto->avioes_aproximando->n_elementos > 0) {
				aviao_t* next = remover (meu_aeroporto->avioes_aproximando);
				// Pista foi liberada e está em processo de pouso
				pousar_aviao(meu_aeroporto, next);
			}
		}
		if(ticks++ > t_simulacao && meu_aeroporto->avioes_no_aeroporto == 0)
			running = 0;
	}


	finalizar_aeroporto(meu_aeroporto);
	return 1;
}
