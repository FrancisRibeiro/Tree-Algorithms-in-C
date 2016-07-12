#include "common.h"
#include "priori.h"
#include "convencional.h"

int main() {
    int i, opt = 0, opt2 = 0, chave, R;
    BTREE_Conv *C = criaArvoreBConvencional();
    BTREE_Priori *P = criaArvoreBPriori();
    FILE *arq;
    int dado, tam;
    char nomeArquivo[100];

    printf("Digite o nome do arquivo de entrada [não eh necessario '.txt']: ");
    scanf("%s", &nomeArquivo);
    strcat(nomeArquivo, ".txt");

    arq = fopen(nomeArquivo, "r");
    if (!arq) {
        printf("%s\nErro ao abrir o arquivo %s%s\n\n", KRED, nomeArquivo, KNRM);
        return 0;
    }

INVALIDA:

    printf("\n%sÁrvores Balanceadas:%s \n\n"
            "Menu de Infra\n"
            "         1. B-Tree Convencional\n"
            "         2. B-Tree com Split e Merge à Priori\n\n", KCYN, KNRM);
    printf("Sua escolha: ");
    scanf("%d", &opt);
    switch (opt) {
        case 1:
            printf("\n%sÁrvore Convencional Selecionada!%s\n", KGRN2, KNRM);
            fscanf(arq, "%d", &tam);

            for (i = 0; i < tam; i++) {
                fscanf(arq, "%d", &dado);
                if (ferror(arq)) {
                    printf("\nErro ao ler arquivo %s\n", nomeArquivo);
                    return 0;
                }
                C = inserirNaArvoreConvencional(C, dado);
            }
            break;
        case 2:
            printf("\n%sÁrvore à Priori Selecionada!%s\n", KGRN2, KNRM);

            fscanf(arq, "%d", &tam);

            for (i = 0; i < tam; i++) {
                fscanf(arq, "%d", &dado);
                if (ferror(arq)) {
                    printf("\nErro ao ler arquivo %s\n", nomeArquivo);
                    return 0;
                }
                P = inserirPriori(P, dado);
            }
            break;
        default:
            printf("\n%sOpção inválida!%s\n", KRED, KNRM);
            goto INVALIDA;
    }

KEEP:
    printf("     \nMenu de Operações\n"
            "         1. Inserir novo elemento\n"
            "         2. Remover elemento da árvore\n"
            "         3. Imprimir árvore\n"
            "         4. Sair\n\n");
    printf("Sua escolha: ");
    scanf("%d", &opt2);
    switch (opt2) {
        case 1:
            if (opt == 1) {
                printf("Informe a chave: ");
                scanf("%d", &chave);
                inserirConvencional(C, chave);
                printf("\n%sChave Inserida!%s\n", KGRN2, KNRM);
            } else {
                printf("Informe a chave: ");
                scanf("%d", &chave);
                P = inserirPriori(P, chave);
                printf("\n%sChave Inserida!%s\n", KGRN2, KNRM);
            }
            goto KEEP;
            break;
        case 2:
            if (opt == 1) {
                printf("Informe a chave: ");
                scanf("%d", &chave);
                R = removerChaveConvencional(C, chave);
                printf("\n%sChave Removida!%s\n", KRED, KNRM);
            } else {
                printf("Informe a chave: ");
                scanf("%d", &chave);
                R = removerChavePriori(P, chave);
                printf("\n%sChave Removida!%s\n", KRED, KNRM);
            }
            goto KEEP;
            break;
        case 3:
            if (opt == 1) {
                printf("\n%sÁrvore Convencional%s\n\n", KGRN2, KNRM);
                imprimeConvencional(C, 0);
            } else {
                printf("\n%sÁrvore à Priori%s\n\n", KGRN2, KNRM);
                imprimePriori(P, 0);
            }
            goto KEEP;
        case 4:
            printf("\n%sFinalizando o programa!!!%s\n\n", KCYN, KNRM);
            break;
        default:
            printf("\n%sOpção inválida!%s\n", KRED, KNRM);
            goto KEEP;
    }

    return 0;
}