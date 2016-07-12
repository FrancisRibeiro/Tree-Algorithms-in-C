#include "common.h"

/* Estrutura para a B-tree Convencional */
typedef struct NODE_C {
    int numChaves; //Número de chaves
    int folha; //Folha
    int chaves[M]; //Vetor de chaves
    struct NODE_C *filhos[M + 1]; //Vetor de filhos
} BTREE_Conv;

/* Cria a árvore com a ordem definida e aloca a raiz*/
BTREE_Conv *criaArvoreBConvencional();

/* Realiza o split no nó, quando este estiver cheio*/
BTREE_Conv *splitConvencional(BTREE_Conv *arv, int *chaveMeio);

/* Insere uma chave em um nó verificando se é necessário fazer split*/
void inserirConvencional(BTREE_Conv *arv, int chave);

/* Insere o nó na árvore*/
BTREE_Conv *inserirNaArvoreConvencional(BTREE_Conv *arv, int chave);

/* Imprime a árvore em níveis*/
void imprimeConvencional(BTREE_Conv *arv, int nivel);

/* Função que localiza a posição em que será inserido o nó, ordenando-o*/
int buscaEmNoConvencional(BTREE_Conv *arv, int chave, int *pos);

/* Remove uma chave */
int removerChaveConvencional(BTREE_Conv *arv, int chave);

/* Faz a concatenação ou redistribuição*/
void RedistribuiOuConcatenaConvencional(BTREE_Conv *arv, int pos);