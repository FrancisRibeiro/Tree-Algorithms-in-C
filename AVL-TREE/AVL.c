/*
 * File:   AVL.c
 * Author: francis
 *
 * Created on 20 de Abril de 2016, 09:28
 */

#include <stdio.h>
#include <stdlib.h>
#include "AVL.h"

arvore* AVL_criaNo(int dado) {
    arvore *novoNo;
    novoNo = (arvore*) calloc(1, sizeof (arvore));
    novoNo->dir = NULL;
    novoNo->esq = NULL;
    novoNo->pai = NULL;
    novoNo->bal = 0;
    novoNo->raiz = dado;
    return novoNo;
}

void AVL_rotacaoDir(arvore *x) {
    arvore *y, *z;
    y = x->esq;
    z = y->dir;
    y->dir = x;
    y->pai = x->pai;
    x->pai = y;
    if (z) {
        x->esq = z;
        z->pai = x;
    } else {
        x->esq = NULL;
    }
    if (y->pai->raiz <= y->raiz) y->pai->dir = y;
    else y->pai->esq = y;
    x->bal = 0;
    y->bal = 0;
}

void AVL_rotacaoEsq(arvore *x) {
    arvore *y, *z, *xPai;
    y = x->dir;
    z = y->esq;
    xPai = x->pai;
    y->esq = x;
    y->pai = xPai;
    x->pai = y;
    if (z) {
        x->dir = z;
        z->pai = x;
    } else {
        x->dir = NULL;
    }
    if (xPai->raiz <= y->raiz) xPai->dir = y;
    else xPai->esq = y;
    x->bal = 0;
    y->bal = 0;
}

void AVL_insert(arvore *x, int dado) {
    //arvore *aux = x,*auxPai, *novoNo;
    arvore *auxPai, *novoNo;
    novoNo = AVL_criaNo(dado);
    if (!x->dir) {
        x->dir = novoNo;
        novoNo->pai = x;
        return;
    }
    while (x) {
        auxPai = x;
        if (dado >= x->raiz) x = x->dir;
        else x = x->esq;
    }
    if (dado >= auxPai->raiz) auxPai->dir = novoNo;
    else auxPai->esq = novoNo;
    novoNo->pai = auxPai;
    AVL_fbUpdateIn(auxPai, dado);
}

void AVL_balanceamentoIn(arvore *no) {
    arvore *filho;
    int netoBal;
    if (no->bal == 2) {
        filho = no->dir;
        if (filho->bal < 0) {
            netoBal = filho->esq->bal;
            AVL_rotacaoDir(filho);
            AVL_rotacaoEsq(no);
            if (netoBal == 1) no->bal = -1;
            if (netoBal == -1) filho->bal = 1;
        } else {
            AVL_rotacaoEsq(no);
        }
    }
    if (no->bal == -2) {
        filho = no->esq;
        if (filho->bal > 0) {
            netoBal = filho->dir->bal;
            AVL_rotacaoEsq(filho);
            AVL_rotacaoDir(no);
            if (netoBal == -1) no->bal = 1;
            if (netoBal == 1) filho->bal = -1;
        } else {
            AVL_rotacaoDir(no);
        }
    }
}

void AVL_fbUpdateIn(arvore *no, int num) {
    if (no->raiz == -1000) return; //Para se chegar no final da árvore
    if (num < no->raiz) (no->bal)--; //Se no número for menor que a raiz, ou seja, o número foi inserido à esquerda, diminui um do fator
    else (no->bal)++; //Caso ele seja inserido à direita, diminui um do fator
    if (no->bal == 0) return; //Se fizer o fator ficar igual a zero, quer dizer que isso não mudou a altura total da árvore, portanto não é necessário continuar atualizando
    if (no->bal == 2 || no->bal == -2) { //Se estiver desbalanceado, chama a função balanceamento.
        AVL_balanceamentoIn(no);
        return;
    }
    AVL_fbUpdateIn(no->pai, no->raiz); //faz o balanceamento com o pai, e volta para o começo
}

arvore* AVL_findNode(arvore *x, int num) {
    arvore *aux = x;
    while (aux->raiz != num) {
        if (aux->raiz < num)aux = aux->dir;
        else aux = aux->esq;
    }
    return aux;
}

void AVL_removeFolha(arvore *x, int num) {
    if (x->raiz < x->pai->raiz) x->pai->esq = NULL;
    else x->pai->dir = NULL;
    AVL_fbUpdateOut(x->pai, num);
    free(x);
}

void AVL_removeUmFilho(arvore *x, int num) {
    if (x->esq == NULL) {
        if (x->pai->raiz > x->raiz)x->pai->esq = x->dir;
        else x->pai->dir = x->dir;
        x->dir->pai = x->pai;
        free(x);
    } else {
        if (x->pai->raiz > x->raiz) x->pai->esq = x->esq;
        else x->pai->dir = x->esq;
        x->esq->pai = x->pai;
        free(x);
    }
    AVL_fbUpdateOut(x->pai, num);
}

void AVL_removeDoisFilhos(arvore *x, int num) {
    arvore *s = x->dir;
    while (s->esq != NULL) s = s->esq;
    x->raiz = s->raiz;
    AVL_removeFolha(s, num);
}

void AVL_remove(arvore *x, int num) {
    arvore *aux;
    aux = x;
    while (aux->raiz != num) {
        if (num > aux->raiz)aux = aux->dir;
        else aux = aux->esq;
    }
    if (aux == NULL) return;
    if (!aux->esq&&!aux->dir) {
        AVL_removeFolha(aux, num);
        return;
    }
    if (aux->esq == NULL || aux->dir == NULL) {
        AVL_removeUmFilho(aux, num);
        return;
    }
    if (aux->esq != NULL && aux->dir != NULL) {
        AVL_removeDoisFilhos(aux, num);
        return;
    }
}

void AVL_fbUpdateOut(arvore *x, int num) {
    if (x->raiz == -1000) return; //Para se chegar no final da árvore
    if (num < x->raiz) x->bal++; //Se o número removido for menor que o seu pai, ou seja, se removeu um número da esquerda, ele aumenta o fator
    else x->bal--; //Se removeu um da direita, ele diminui o fator
    if (x->bal == 2 || x->bal == -2) { //Caso desbalanceie, ele chama a função balancear
        AVL_balanceamentoOut(x);
        x = x->pai;
    }
    if (x->bal == 1 || x->bal == -1) return; //Se a remoção fez com que algum fator fosse para 1 ou -1, quer dizer que não alterou o tamanho total da árvore, portanto, para.
    AVL_fbUpdateOut(x->pai, x->raiz); //Faz o update com o pai, e volta para o começo
}

void AVL_balanceamentoOut(arvore *no) {
    arvore *filho;
    int filhoBal, netoBal;
    if (no->bal == 2) {
        filho = no->dir;
        filhoBal = filho->bal;
        if (filho->bal < 0) {
            netoBal = filho->esq->bal;
            AVL_rotacaoDir(filho);
            AVL_rotacaoEsq(no);
            if (netoBal == 1) no->bal = -1;
            if (netoBal == -1) filho->bal = 1;
        } else {
            AVL_rotacaoEsq(no);
            if (filhoBal == 0) {
                no->bal = 1;
                filho->bal = -1;
            }
        }
    }
    if (no->bal == -2) {
        filho = no->esq;
        filhoBal = filho->bal;
        if (filho->bal > 0) {
            netoBal = filho->dir->bal;
            AVL_rotacaoEsq(filho);
            AVL_rotacaoDir(no);
            if (netoBal == -1) no->bal = 1;
            if (netoBal == 1) filho->bal = -1;
        } else {
            AVL_rotacaoDir(no);
            if (filhoBal == 0) {
                no->bal = 1;
                filho->bal = -1;
            }
        }
    }
}

void AVL_fprintf(arvore *x, FILE *arq2) {
    if (!x) return;
    AVL_fprintf(x->esq, arq2);
    fprintf(arq2, "Valor = %d\tFB = %d\n", x->raiz, x->bal);
    AVL_fprintf(x->dir, arq2);
}

void AVL_printf(arvore *x) {
    if (!x) return;
    AVL_printf(x->esq);
    printf("Valor = %d\tFB = %d\n", x->raiz, x->bal);
    AVL_printf(x->dir);
}
