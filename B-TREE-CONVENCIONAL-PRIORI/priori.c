#include "priori.h"

/* Aloca um nó para a árvore */
BTREE_Priori *criaArvoreBPriori() {
    BTREE_Priori *x = malloc(sizeof (BTREE_Priori));
    x->folha = TRUE; //O nó sempre começa sendo folha
    x->numChaves = 0; //O contador de chaves sempre começa com zero
    x->filhos[0] = NULL; //filhos começam vazios

    return x;
}

/* Realiza o split no nó, considerando que o pai sempre terá espaço */
BTREE_Priori *splitPriori(BTREE_Priori *pai, int i, BTREE_Priori *filhoEsquerda) {
    int j;
    BTREE_Priori *filhoDireita;

    filhoDireita = criaArvoreBPriori(); //Alocando espaço para o filho direito

    filhoDireita->folha = filhoEsquerda->folha;
    filhoDireita->numChaves = (int) (M / 2) - 1; //Número de elementos á direta do meio

    //Passando os elementos á direita do meio para o novo nó
    for (j = 0; j < (int) (M / 2) - 1; j++)
        filhoDireita->chaves[j] = filhoEsquerda->chaves[j + (int) (M / 2)];

    //Caso não seja folha, temos ponteiros os filhos para passar para à esquerda
    if (filhoEsquerda->folha == FALSE)
        for (j = 0; j < (int) (M / 2); j++)
            filhoDireita->filhos[j] = filhoEsquerda->filhos[j + (int) (M / 2)]; //O novo nó recebe os ponteiros

    filhoEsquerda->numChaves = (int) (M / 2) - 1; //O número de elementos do irmão a direita é atualizado

    //Passando os ponteiros corretos para o pai
    for (j = pai->numChaves; j >= i + 1; j--)
        pai->filhos[j + 1] = pai->filhos[j];

    pai->filhos[i + 1] = filhoDireita; // Filho á esquerda é alocado a seu pai

    //As chaves do pai são atualizadas
    for (j = pai->numChaves - 1; j >= i; j--)
        pai->chaves[j + 1] = pai->chaves[j];

    pai->chaves[i] = filhoEsquerda->chaves[(int) (M / 2) - 1];

    pai->numChaves++;

    return pai;
}

/* Insere uma chave em um nó não cheio, se necessário faz o split */
BTREE_Priori *inserirNaoCheio(BTREE_Priori *pai, int chave) {
    int i = pai->numChaves - 1;

    if (pai->folha == TRUE) { //Verifica se o nó é folha
        while (i >= 0 && pai->chaves[i] > chave) { //Procura a posição correta para inserir
            pai->chaves[i + 1] = pai->chaves[i]; //Move o último elemento uma posição a frente(SHIFT))
            i--;
        }

        pai->chaves[i + 1] = chave; //insere na posição correta
        pai->numChaves++;
    } else {
        while (i >= 0 && pai->chaves[i] > chave)//Procura a posição correta para inserir
            i--;
        i++;

        if (pai->filhos[i]->numChaves == M - 1) { //Veririca se o nó interno esta cheio
            pai = splitPriori(pai, i, pai->filhos[i]); //Se estiver divide

            if (chave > pai->chaves[i])
                i++;
        }

        //caso não esteja cheio faz recursão, mas agora analisando o proximo filho
        pai->filhos[i] = inserirNaoCheio(pai->filhos[i], chave);
    }

    return pai;
}

/* Verifica se a raiz da árvore precisa ser dividida;
 * Se necessário, chama o split para a raiz e aumenta a altura da árvore
 * chama a insereChave com a raiz da árvore */
BTREE_Priori *inserirPriori(BTREE_Priori *raiz, int chave) {
    BTREE_Priori *filhoEsquerda = raiz;
    BTREE_Priori *novaRaiz;

    if (filhoEsquerda->numChaves == (M - 1)) { //Se a raiz estiver cheia;
        novaRaiz = criaArvoreBPriori(); //Este nó será a nova raiz
        novaRaiz->folha = FALSE; //Se estiver cheia passa a não ser mais folha
        novaRaiz->filhos[0] = filhoEsquerda; //Novo nó recebe o filho a esquerda
        novaRaiz = splitPriori(novaRaiz, 0, filhoEsquerda); //Faço a divisão da raiz
        novaRaiz = inserirNaoCheio(novaRaiz, chave); //Insiro a nova chave na folha

        return novaRaiz;
    } else {
        return inserirNaoCheio(filhoEsquerda, chave); //faz a análise se para os filhos
    }
}

/* Função que imprime a árvore em níveis */
void imprimePriori(BTREE_Priori *arv, int nivel) {
    int i;

    if (arv == NULL) return;

    printf("Nivel %d : ", nivel);

    for (i = 0; i < arv->numChaves; i++)
        printf("%d ", (int) arv->chaves[i]);

    putchar('\n');
    nivel++;

    for (i = 0; i <= arv->numChaves; i++)
        imprimePriori(arv->filhos[i], nivel); //Desce a recursão para os filhos
}

/* Função que localiza a posição em que será inserido o nó */
int buscaEmNoPriori(BTREE_Priori *arv, int chave, int *pos) {
    for ((*pos) = 0; (*pos) < arv->numChaves; ++(*pos)) //Roda até o máximo de chaves inseridas
        if (chave == arv->chaves[*pos]) //Se o elemento a ser inserido já existir no vetor
            return 1; //caso a chave exista
        else if (chave < arv->chaves[*pos]) //Se o elemento for menor que o procurado
            break;
    return 0; //caso a chave não exista
}

/* Remove uma chave */
int removerChavePriori(BTREE_Priori *arv, int chave) {
    int i, pos;
    BTREE_Priori *predecessor;

    if (arv->folha == TRUE) {
        if (buscaEmNoPriori(arv, chave, &pos)) { //localiza a exata posição da chave a ser removida

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
        if (buscaEmNoPriori(arv, chave, &pos)) { //localiza a exata posição da chave a ser removida
            /* LOCALIZAR MÁXIMO */

            predecessor = arv->filhos[pos];

            while (predecessor->folha == FALSE) //Enquanto a chave não for folha
                predecessor = predecessor->filhos[predecessor->numChaves]; //Buscamos o valor máximo dela (PREDECESSOR)

            /* troca predecessor com a chave a ser removida e
             * chama a função para remover a chave */
            arv->chaves[pos] = predecessor->chaves[predecessor->numChaves - 1];
            predecessor->chaves[predecessor->numChaves - 1] = chave;
        }

        if (!removerChavePriori(arv->filhos[pos], chave))
            return 0;

        /* MERGE A PRIORI */

        /* Se o filho a esquerda e a direita possuírem o número minímo de elementos,
         * chama a função que faz a redistribuição ou concatenação(MERGE),
         * mas nesse caso sempre será realizado a concatenação,
         * pois não houve um estouro */
        if ((arv->filhos[pos]->numChaves == (M / 2) - 1)
                && (arv->filhos[pos + 1] != NULL)) {
            if (arv->filhos[pos + 1]->numChaves == (M / 2) - 1) {
                printf("\n%sMERGE À PRIORI%s\n", KBLU2, KNRM);
                RedistribuiOuConcatenaPriori(arv, pos);
            }
        } else if (arv->filhos[pos]->numChaves < (M / 2) - 1) {
            /* Se o nó tiver menos ue a quantidade minima de chaves
             * são feitos os reajustes */
            RedistribuiOuConcatenaPriori(arv, pos);
        }
    }

    return 1;
}

/* Faz a concatenação ou redistribuição*/
void RedistribuiOuConcatenaPriori(BTREE_Priori *arv, int pos) {
    int i, j;
    BTREE_Priori *esq, *dir;

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
        while ((dir->numChaves < M / 2) == TRUE) { //enquanto o valor for menor que o minímo

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