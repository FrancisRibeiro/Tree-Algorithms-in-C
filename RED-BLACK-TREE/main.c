/*
 * File:   main.c
 * Author: francis
 *
 * Created on 8 de Maio de 2016, 16:34
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RB.h"

int main() {
    FILE *arq;
    char nomeArquivo[30];
    int dado;

    printf("Digite o nome do arquivo de entrada [nao eh necessario '.txt']: ");
    scanf("%s", &nomeArquivo);
    strcat(nomeArquivo, ".txt");

    arvore *A = criaArvore();
    arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo %s", nomeArquivo);
        return 0;
    }

    while (fscanf(arq, "%d", &dado) != EOF) {
        if (ferror(arq)) {
            printf("Erro ao ler arquivo de dados");
            return 0;
        }
        insereno(A, dado);
    }
    printf("\n");
    printf("   Valor\tCor [V = Vermelha e P = Preta]\n\n");
    percorreOrdem(A->dir);
    printf("\n");

}

