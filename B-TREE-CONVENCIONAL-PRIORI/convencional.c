#include "convencional.h"

/* Função que aloca um nova árvore */
BTREE_Conv *criaArvoreBConvencional() {
    BTREE_Conv *arv = malloc(sizeof (BTREE_Conv)); //Raiz
    arv->folha = TRUE; //O nó sempre começa sendo folha
    arv->numChaves = 0; //O contador de chaves sempre começa com zero
    arv->filhos[0] = NULL; //filhos começam vazios

    return arv;
}

/* Função que realiza o split de um nó cheio */
BTREE_Conv *splitConvencional(BTREE_Conv *arv, int *chaveMeio) {
    int i, meio;
    BTREE_Conv *NovofilhoDireita = criaArvoreBConvencional(); //Nó que será alocado á direita do novo pai

    meio = (arv->numChaves / 2) - 1; //Chave intermediária

    NovofilhoDireita->numChaves = arv->numChaves - meio - 1; //Novo número de chaves do novo filho
    arv->numChaves = meio; //Número de chaves é a mediana
    *chaveMeio = arv->chaves[meio]; //A chave do meio é alocada
    NovofilhoDireita->filhos[0] = arv->filhos[meio + 1]; //atualiza o ponteiro do novo nó

    for (i = 0; i < NovofilhoDireita->numChaves; ++i) {
        NovofilhoDireita->chaves[i] = arv->chaves[meio + 1 + i]; //O nó da direita recebe as chaves acima do meio
        NovofilhoDireita->filhos[i + 1] = arv->filhos[meio + 1 + i + 1]; //Os ponteiros vão sendo atualizados
    }

    return NovofilhoDireita;
}

/* Insere internamente na árvore realizando o split caso necessário,
 * Aqui tratamos o caso em que o no raiz da árvore fica com informações excedentes */
BTREE_Conv *inserirNaArvoreConvencional(BTREE_Conv *arv, int chave) {
    int chaveMeio;
    BTREE_Conv *filhoDireita, *NovoPai;

    inserirConvencional(arv, chave);

    if (arv->numChaves == M) { //Se encher o nó, faz a divisão
        filhoDireita = splitConvencional(arv, &chaveMeio); //O split retorna o nó com os elementos abaixo do meio
        NovoPai = criaArvoreBConvencional(); //É criado um novo nó que será o novo pai

        NovoPai->chaves[0] = chaveMeio; //Pai recebe a chave do meio
        NovoPai->filhos[0] = arv; //Filho á esquerda
        NovoPai->filhos[1] = filhoDireita; //Filho á direita
        NovoPai->numChaves = 1; //Número de chaves do pai

        return NovoPai;
    }

    return arv;
}

/* Insere na árvore realizando o split caso necessário,
 * se o nó for folha basta inserir na árvore,
 * se não for folha, usamos a recursão para achar a folha correta
 * e por fim verificamos se a raiz da sub-árvore tem informações excedentes */
void inserirConvencional(BTREE_Conv *arv, int chave) {
    int pos, chaveMeio, j;
    BTREE_Conv *filhoDireita;

    buscaEmNoConvencional(arv, chave, &pos); //Localiza a posição para inserir um nó

    if (arv->filhos[0] == NULL) { //Se o nó for folha

        /* INSERE A DIREITA */

        for (j = arv->numChaves; j > pos; --j) {
            arv->chaves[j] = arv->chaves[j - 1]; //Shift no vetor todo
            arv->filhos[j + 1] = arv->filhos [j]; //Atualiza os ponteiros dos filhos
        }

        arv->chaves[pos] = chave; //A nova chave é colocada na posição correta
        arv->filhos[pos + 1] = NULL; //atualiza filho do nó
        arv->numChaves++;
    } else {
        inserirConvencional(arv->filhos[pos], chave); //procura por uma folha

        if (arv->filhos[pos]->numChaves == M) { //Se encher o nó, faz a divisão
            filhoDireita = splitConvencional(arv->filhos[pos], &chaveMeio); //O split retorna o nó com os elementos acima do meio

            /* INSERE A DIREITA */

            for (j = arv->numChaves; j > pos; --j) {
                arv->chaves[j] = arv->chaves[j - 1]; //Shift no vetor todo
                arv->filhos[j + 1] = arv->filhos [j]; //Atualiza os ponteiros dos filhos
            }

            arv->chaves[pos] = chaveMeio; //A nova chave é colocada na posição correta
            arv->filhos[pos + 1] = filhoDireita; //Atualiza filho do nó
            arv->numChaves++;
        }
    }
}

/* Função que imprime a árvore em níveis */
void imprimeConvencional(BTREE_Conv *arv, int nivel) {
    long i;

    if (arv == NULL) return;

    printf("Nivel %d : ", nivel);

    for (i = 0; i < arv->numChaves; i++)
        printf("%d ", (int) arv->chaves[i]);

    putchar('\n');
    nivel++;

    for (i = 0; i <= arv->numChaves; i++)
        imprimeConvencional(arv->filhos[i], nivel); // Desce a recursão para os filhos
}

/* Função que localiza a posição em que será inserido o nó */
int buscaEmNoConvencional(BTREE_Conv *arv, int chave, int *pos) {
    for ((*pos) = 0; (*pos) < arv->numChaves; ++(*pos)) // Posição zero até máximo de chaves (a->n) já preenchidas
        if (chave == arv->chaves[*pos]) //Se o elemento a ser inserido já existir no vetor
            return 1; //Caso a chave exista
        else if (chave < arv->chaves[*pos]) //Se o elemento for menor que o procurado
            break;

    return 0; //Caso a chave não exista
}

/* Remove uma chave */
int removerChaveConvencional(BTREE_Conv *arv, int chave) {
    int i, pos;
    BTREE_Conv *predecessor;

    if (arv->filhos[0] == NULL) {
        if (buscaEmNoConvencional(arv, chave, &pos)) { //localiza a exata posição da chave a ser removida

            /* DELETAR Á ESQUERDA
             * A chaves maiores que a chave a ser deletadas são movidas para a
             * esquerda e a chave a ser deletada é colocada na ultima posição do nó */
            for (i = pos; i < arv->numChaves - 1; ++i) {
                arv->chaves[i] = arv->chaves[ i + 1]; //As chaves são movidas
                arv->filhos[i] = arv->filhos[ i + 1]; //Os filhos são atualizados
            }

            arv->filhos[i] = arv->filhos[i + 1];
            arv->numChaves--;
        } else {
            return 0;
        }
    } else {
        if (buscaEmNoConvencional(arv, chave, &pos)) { //localiza a exata posição da chave a ser removida

            /* LOCALIZAR MÁXIMO */

            predecessor = arv->filhos[pos];
            while (predecessor->filhos[0] != NULL) //Enquanto a chave não for folha
                predecessor = predecessor->filhos[predecessor->numChaves]; //Buscamos o valor máximo dela (PREDECESSOR)

            /* troca predecessor com a chave a ser removida e
             * chama a função para remover a chave */
            arv->chaves[pos] = predecessor->chaves[predecessor->numChaves - 1];
            predecessor->chaves[predecessor->numChaves - 1] = chave;
        }

        if (!removerChaveConvencional(arv->filhos[pos], chave))
            return 0;

        /* Se o nó tiver menos ue a quantidade minima de chaves
         * são feitos os reajustes */
        if (arv->filhos[pos]->numChaves < ((M / 2) - 1))
            RedistribuiOuConcatenaConvencional(arv, pos);
    }

    return 1;
}

/* Faz a concatenação ou redistribuição*/
void RedistribuiOuConcatenaConvencional(BTREE_Conv* arv, int pos) {
    int i, j;
    BTREE_Conv *esq, *dir;

    if (pos == arv->numChaves)
        pos--;

    esq = arv->filhos[pos]; //Recebe filho á esquerda
    dir = arv->filhos[pos + 1]; //Recebe filho á direita

    if (esq->numChaves + dir->numChaves + 1 >= M) { //Caso haja um estouro

        /* REDISTRIBUIÇÃO */

        while ((esq->numChaves < M / 2) == TRUE) { //Enquanto o valor for menor que o esperado

            /* ROTAÇÃO A DIREITA */

            /* Faz o shift nos elementos da esquerda */
            for (j = esq->numChaves; j > esq->numChaves; --j) {
                esq->chaves[j] = esq->chaves[j - 1];
                esq->filhos[j + 1] = esq->filhos[j];
            }

            /* Adiciona uma chave da direita na esquerda */
            esq->chaves[esq->numChaves] = arv->chaves[pos];
            esq->filhos[esq->numChaves + 1] = dir->filhos[0];
            esq->numChaves++;

            arv->chaves[pos] = dir->chaves[0];

            /* DELETAR Á ESQUERDA */

            /* Faz o shift nos elementos da esquerda */
            for (j = 0; j < dir->numChaves - 1; ++j) {
                dir->chaves[j] = dir->chaves[ j + 1];
                dir->filhos[j] = dir->filhos[ j + 1];
            }

            dir->filhos[j] = dir->filhos[j + 1];
            dir->numChaves--; //O nó passa a ter uma posição a menos
        }

        while ((dir->numChaves < (M / 2)) == TRUE) { //enquanto o valor for menor que o minímo

            /* ROTAÇÃO Á ESQUERDA */

            /* Faz o shift nos elementos da direita */
            for (j = dir->numChaves; j > 0; --j) {
                dir->chaves[j] = dir->chaves[j - 1];
                dir->filhos[j + 1] = dir->filhos[j];
            }

            //Os filhos e chaves são atualizados
            dir->filhos[1] = dir->filhos[0];
            dir->chaves[0] = arv->chaves[pos];
            dir->filhos[0] = esq->filhos[esq->numChaves];
            dir->numChaves++;

            arv->chaves[pos] = esq->chaves[esq->numChaves - 1];

            /* DELETAR DA DIREITA */

            /* Faz o shift nos elementos da esquerda */
            for (j = esq->numChaves - 1; j < esq->numChaves - 1; j++) {
                esq->chaves[j] = esq->chaves[ j + 1]; //As chaves são movidas
                esq->filhos[j + 1] = esq->filhos[ j + 2];
            }

            esq->numChaves--; //O nó passa a ter uma posição a menos
        }
    } else {
        /* CONCATENAÇÃO */

        /* ROTACAO A DIREITA */

        /* Faz o shift nos elementos da esquerda */
        for (j = esq->numChaves; j > esq->numChaves; --j) {
            esq->chaves[j] = esq->chaves[j - 1];
            esq->filhos[j + 1] = esq->filhos[j];
        }

        /* Adiciona uma chave da esquerda na direita e move os filhos */
        esq->chaves[esq->numChaves] = arv->chaves[pos];
        esq->filhos[esq->numChaves + 1] = dir->filhos[0];
        esq->numChaves++;

        for (i = 0; i < dir->numChaves; ++i) {

            /* Faz o shift nos elementos da esquerda */
            for (j = esq->numChaves; j > esq->numChaves; --j) {
                esq->chaves[j] = esq->chaves[j - 1];
                esq->filhos[j + 1] = esq->filhos[j];
            }

            /* Adiciona uma chave da esquerda na direita e move os filhos */
            esq->chaves[esq->numChaves] = dir->chaves[i];
            esq->filhos[esq->numChaves + 1] = dir->filhos[i + 1];
            esq->numChaves++;

        }

        free(dir); // libera o nó da direita

        /* DELETAR DA DIREITA */

        /* Faz o shift nos elementos do pai  que ficou menor em uma unidade */
        for (j = pos; j < arv->numChaves - 1; ++j) {
            arv->chaves[j] = arv->chaves[ j + 1];
            arv->filhos[j + 1] = arv->filhos[ j + 2];
        }

        arv->numChaves--;
    }
}