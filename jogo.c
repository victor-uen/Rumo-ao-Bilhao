#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ALTERNATIVAS 4
#define MAX_NOME 50

typedef struct {
    char enunciado[200];
    char alternativas[MAX_ALTERNATIVAS][100];
    int correta;
    int pulada; //começa em 0
    struct PerguntaFacil *prox;
} PerguntaFacil;

typedef struct {
    char enunciado[200];
    char alternativas[MAX_ALTERNATIVAS][100];
    int correta;
    int pulada; //começa em 0
    struct PerguntaMedia *prox;
} PerguntaMedia;

typedef struct {
    char enunciado[200];
    char alternativas[MAX_ALTERNATIVAS][100];
    int correta;
    int pulada; //começa em 0
    struct PerguntaDif *prox;
} PerguntaDif;

typedef struct {
    char nome[MAX_NOME];
    int vidas;
    int pontuacao;
    int pulos;
    int coringas;
    int multiplicador;
} Jogador;

// Carregar perguntas
int carregarPerguntas(const char* arquivo, Pergunta perguntas[]) {
    FILE* f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir o arquivo de perguntas.\n");
        return 0;
    }

    int i = 0;
    while (i < maxPerguntas) {
        if (!fgets(perguntas[i].enunciado, sizeof(perguntas[i].enunciado), f)) break;
        perguntas[i].enunciado[strcspn(perguntas[i].enunciado, "\n")] = 0; // remover \n

        for (int j = 0; j < MAX_ALTERNATIVAS; j++) {
            if (!fgets(perguntas[i].alternativas[j], sizeof(perguntas[i].alternativas[j]), f)) break;
            perguntas[i].alternativas[j][strcspn(perguntas[i].alternativas[j], "\n")] = 0;
        }

        char linha[20];
        if (!fgets(linha, sizeof(linha), f)) break;
        perguntas[i].correta = atoi(linha);

        if (!fgets(linha, sizeof(linha), f)) break;
        perguntas[i].nivel = atoi(linha);

        i++;
    }

    fclose(f);
    return i;
}

// Embaralhar perguntas
void embaralhar(Pergunta perguntas[], int total) {
    srand(time(NULL));
    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Pergunta tmp = perguntas[i];
        perguntas[i] = perguntas[j];
        perguntas[j] = tmp;
    }
}


// Criar jogador
Jogador criarJogador(const char* nome) {
    Jogador j;
    strcpy(j.nome, nome);
    j.vidas = 3;
    j.pontuacao = 0;
    j.pulos = 2;
    j.coringas = 1;
    j.multiplicador = 1;
    return j;
}

// Atualizar pontuação
void atualizarPontuacao(Jogador* j, int acerto) {
    if (acerto) {
        j->pontuacao += 100 * j->multiplicador;
        j->multiplicador++;
    } else {
        j->vidas--;
        j->multiplicador = 1;
    }
    printf("\nPontuação atual: %d\n", j->pontuacao);
}

// Jogar um nível
void jogarNivel(Jogador* jogador, Pergunta perguntas[], int total, int nivel) {
    printf("\n--- NÍVEL %d ---\n", nivel);
    int respondidas = 0;
    for (int i = 0; i < total && respondidas < 4; i++) {
        if (perguntas[i].nivel != nivel) continue;

        printf("\n%s\n", perguntas[i].enunciado);
        for (int j = 0; j < MAX_ALTERNATIVAS; j++) {
            printf("%d) %s\n", j + 1, perguntas[i].alternativas[j]);
        }

        int resposta;
        printf("Digite: \n1 para alternativa 1 \n2 para alternativa 2 \n3 para alternativa 3 \n4 para alternativa 4 \nP para pular (Pulos restantes: %d)\nD para ver uma dica \nC para uma ajuda coringa \n", jogador->pulos);
        scanf("%d", &resposta);

        if (resposta == 0 && jogador->pulos > 0) {
            jogador->pulos--;
            printf("Pulou! Pergunta será trocada.\n");
            i--; // repete uma outra pergunta do mesmo nível
            // Preciso alteração mudar Perguntas para ser uma lista circular simples. Quando o jogador usa o pulo, a pergunta atual é deletada e a lista ajustada. E o jogador responde uma pergunta de mesmo nível.
            continue;
        }

        if (resposta == perguntas[i].correta) {
            printf("Correta!\n");
            atualizarPontuacao(jogador, 1);
        } else {
            printf("Errada! A certa era %d\n", perguntas[i].correta);
            atualizarPontuacao(jogador, 0);
            if (jogador->vidas <= 0) {
                printf("Sem vidas restantes!\n");
                return;
            }
        }
        respondidas++;
    }
}

// Jogar partida completa
void jogarPartida(Jogador* jogador, Pergunta perguntas[], int total) {
    printf("\nBem vindo %s! Vejo que você está no Rumo ao Bilhão!\n", jogador->nome);
    for (int nivel = 1; nivel <= 3; nivel++) {
        jogarNivel(jogador, perguntas, total, nivel);
        if (jogador->vidas <= 0) break;
    }
    printf("\nFim da partida! Pontuação final: %d\n", jogador->pontuacao);
}


int main() {
    int menu = 9;
    while (menu != 0) {
        printf("1- Jogar\n\2- Ver Ranking\n3- Ver Instruções\n0- Sair\n");
        scanf("%d", &menu);

        if (menu == 1) {
            PerguntaFacil *headFacil = NULL;
            PerguntaMedia *headMedia = NULL;
            PerguntaDif *headDif = NULL;
            Pergunta perguntas[MAX_PERGUNTAS];
            int total = carregarPerguntas("C:\\Users\\lucar\\OneDrive\\Anexos\\teste jogo aed\\joguinho.txt", perguntas, MAX_PERGUNTAS);
            if (total == 0) return 1;

            embaralhar(perguntas, total);

            char nome[MAX_NOME];
            printf("Digite seu nome: ");
            scanf(" %[^\n]", nome);

            Jogador jogador = criarJogador(nome);
            jogarPartida(&jogador, perguntas, total);
        }
        else if (menu == 2) {
            //Ranking
        }
        else if (menu == 3) {
            //Ver Instruções
        }
    }
    printf("Obrigado por jogar\n");
    return 0;
}
