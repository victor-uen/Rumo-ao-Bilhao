#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//==== CONSTANTES DO JOGO ====

// constantes do jogador
#define TAM_NOME 20

// constantes das perguntas
#define TAM_ENUNC 200
#define TAM_ALT 30
#define ALT 4


//==== STRUCTS ====

typedef struct Jogador{
    char nome[TAM_NOME];
    int vidas; //3
    int pulos; //3
    int cartas; //1
    int coringa; //1
    float score; //começa em 0.0 reais
    float mult; //começa em 1.0X
} Jogador;

typedef enum {
    HISTORIA,
    GEOGRAFIA,
    CIENCIA,
    ESPORTE,
    ENTRETENIMENTO
} Tema;

typedef struct Pergunta{ //Lista Circular Simples
    Tema tema;
    char enunciado[TAM_ENUNC];
    char alternativa[ALT][TAM_ALT];
    int resposta;
    int tempo; //15 ou 30 segundos
    struct Pergunta *prox;
} Pergunta;

typedef struct Ranking { //Lista Encadeada Simples
    char nome[TAM_NOME];
    float pontuacao;
    struct Ranking *prox;
} Ranking;