#include <stdio.h>
#include "matrix.h"

int main(void) {
    Matrix *A = NULL;
    Matrix *B = NULL;
    Matrix *C = NULL;
    
    // Leitura da matriz A do stdin
    if (!matrix_create(&A)) {
        int lines_A, cols_A;
        if (scanf("%d %d", &lines_A, &cols_A) == 2) {
            float value;
            for (int i = 1; i <= lines_A; i++) {
                for (int j = 1; j <= cols_A; j++) {
                    if (scanf("%f", &value) == 1) {
                        if (value != 0.0) {
                            matrix_setelem(A, i, j, value);
                        }
                    }
                }
            }
        }
        printf("Matriz A:\n");
        matrix_print(A);
    } else {
        fprintf(stderr, "Erro na alocacao de A como listas encadeadas.\n");
        return 1;
    }
    
    // Leitura da matriz B do stdin
    if (!matrix_create(&B)) {
        int lines_B, cols_B;
        if (scanf("%d %d", &lines_B, &cols_B) == 2) {
            float value;
            for (int i = 1; i <= lines_B; i++) {
                for (int j = 1; j <= cols_B; j++) {
                    if (scanf("%f", &value) == 1) {
                        if (value != 0.0) {
                            matrix_setelem(B, i, j, value);
                        }
                    }
                }
            }
        }
        printf("\nMatriz B:\n");
        matrix_print(B);
    } else {
        fprintf(stderr, "Erro na alocacao de B como listas encadeadas.\n");
        return 1;
    }
    
    printf("\n=== Soma C = A + B ===\n");
    if (!matrix_add(A, B, &C)) {
        matrix_print(C);
    } else {
        fprintf(stderr, "Erro na soma C=A+B.\n");
    }
    matrix_destroy(C);
    C = NULL;
    
    printf("\n=== Multiplicacao C = A * B ===\n");
    if (!matrix_multiply(A, B, &C)) {
        matrix_print(C);
    } else {
        fprintf(stderr, "Erro na multiplicacao C=A*B.\n");
    }
    matrix_destroy(C);
    C = NULL;
    
    printf("\n=== Transposicao C = A^T ===\n");
    if (!matrix_transpose(A, &C)) {
        matrix_print(C);
    } else {
        fprintf(stderr, "Erro na transposicao C=A^T.\n");
    }
    matrix_destroy(C);
    
    matrix_destroy(A);
    matrix_destroy(B);
    
    return 0;
}

