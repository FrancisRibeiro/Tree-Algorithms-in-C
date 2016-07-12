/*
 * File:   main.c
 * Author: francis
 *
 * Created on 17 de Abril de 2016, 21:39
 */

#include <stdio.h>
#include <stdlib.h>
#include "B_S_T.h"

int main() {
    int N = 8, dados[8] = {50, 100, 30, 20, 40, 45, 35, 37};

    ArvBin* raiz = cria_ArvBin();

    int i;
    for (i = 0; i < N; i++)
        insere_ArvBin(raiz, dados[i]);

    preOrdem_ArvBin(raiz);

    if (remove_ArvBin(raiz, 50)) {
        printf("removido\n");
        preOrdem_ArvBin(raiz);
    } else
        printf("NAO removido\n");

    libera_ArvBin(raiz);
    printf("\nFim!\n");
    return 0;
}
