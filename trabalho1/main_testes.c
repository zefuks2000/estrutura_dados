#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main() {
    Matrix* M = NULL;
    Matrix *B=NULL;
    Matrix *C=NULL;
    // 1. O matrix_create vai ler do stdin (que vamos redirecionar do arquivo)
    // Ele deve alocar a cabeça, ler as dimensões e ler os valores.
    printf("Lendo dados e criando matriz...\n");
    
    if (matrix_create(&M) == -1) {
        printf("Erro fatal: Falha ao criar a matriz (malloc ou input invalido).\n");
        return 1;
    }

    // 2. Se chegou aqui, a estrutura deve estar montada na memória..
    // Vamos imprimir para ver se os nós estão nos lugares certos.
    printf("\n--- Resultado da Impressao ---\n");
    matrix_print(M);
    matrix_print()
   
    // (Aqui você chamaria matrix_destroy(M) futuramente)
    return 0;
}