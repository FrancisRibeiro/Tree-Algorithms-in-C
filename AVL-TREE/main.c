/*
 * File:   main.c
 * Author: francis
 *
 * Created on 20 de Abril de 2016, 09:28
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL.h"
#include <time.h>

int main() {
    FILE *arq;
    int dado, i;
    arvore *x = AVL_criaNo(-1000);
    char nomeArquivo[100], op = 's';

    printf("Digite o nome do arquivo de entrada [nao eh necessario '.txt']: ");
    scanf("%s", &nomeArquivo);
    strcat(nomeArquivo, ".txt");


    arq = fopen(nomeArquivo, "r");
    if (!arq) {
        printf("\nErro ao abrir o arquivo %s\n", nomeArquivo);
        return 0;
    }
    while (!feof(arq)) {
        fscanf(arq, "%d", &dado);
        if (ferror(arq)) {
            printf("\nErro ao ler arquivo %s\n", nomeArquivo);
            return 0;
        }
        AVL_insert(x, dado);
    }

    printf("\n");
    AVL_printf(x->dir);

    printf("\n");
    do {
        printf("Digite um numero para ser removido: ");
        scanf("%d", &dado);
        AVL_remove(x, dado);
        printf("\n\nArvore depois da remocao:\n\n");
        AVL_printf(x->dir);
        printf("\nDeseja remover mais elementos (s / n)?: ");
        scanf(" %c", &op);
        printf("\n");
    } while (op == 's' || op == 'S');
    printf("\nArvore final:\n");
    AVL_printf(x->dir);
    printf("\n");
    fclose(arq);
    return 0;
}
