/* 
 * File:   AVL.h
 * Author: francis
 *
 * Created on 20 de Abril de 2016, 09:28
 */

typedef struct arvore {
    int raiz;
    struct arvore *esq;
    struct arvore *dir;
    struct arvore *pai;
    int bal;
} arvore;


arvore* AVL_criaNo(int dado); //
void AVL_rotacaoDir(arvore *x); //
void AVL_rotacaoEsq(arvore *x); //
void AVL_insert(arvore *x, int dado); //
void AVL_balanceamentoIn(arvore *no); //
void AVL_fbUpdateIn(arvore *no, int num); //
arvore* AVL_findNode(arvore *x, int num); // NÃO
void AVL_removeFolha(arvore *x, int num); //
void AVL_removeUmFilho(arvore *x, int num); //
void AVL_removeDoisFilhos(arvore *x, int num); //
void AVL_remove(arvore *x, int num); //
void AVL_fbUpdateOut(arvore *x, int num); //
void AVL_balanceamentoOut(arvore *no);
void AVL_ERD(arvore *x, FILE *arq2);