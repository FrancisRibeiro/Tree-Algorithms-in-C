#include "common.h"

/* Estrutura para a B-tree a Priori */
typedef struct NODE_P {
    int numChaves; //Número de chaves
    int folha; //Folha
    int chaves[M - 1]; //Vetor de chaves
    struct NODE_P *filhos[M]; //Vetor de filhos
} BTREE_Priori;

/* Cria a árvore com a ordem definida e aloca a raiz */
BTREE_Priori *criaArvoreBPriori();

/* Realiza o split no nó, considerando que o pai sempre terá espaço */
BTREE_Priori *splitPriori(BTREE_Priori *pai, int i, BTREE_Priori *filhoEsquerda);

/* insere uma chave em um nó não cheio, se necessário faz o split */
BTREE_Priori *inserirNaoCheio(BTREE_Priori *pai, int chave);

/* Verifica se a raiz da árvore precisa ser dividida;
 * Se necessário, chama o split para a raiz e aumenta a altura da árvore
 * chama a insereChave com a raiz da árvore */
BTREE_Priori *inserirPriori(BTREE_Priori *raiz, int chave);

/* Imprime a árvore em ordem */
void imprimePriori(BTREE_Priori *arv, int nivel);

/* Busca a posição de um no */
int buscaEmNoPriori(BTREE_Priori *arv, int chave, int *pos);

/* Remove uma chave */
int removerChavePriori(BTREE_Priori *arv, int chave);

/* Faz a concatenação ou redistribuição*/
void RedistribuiOuConcatenaPriori(BTREE_Priori *arv, int pos);


