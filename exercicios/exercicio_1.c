#include<stdio.h>
#include <math.h>


int matriz_identidade(int* mat, int n){
	int p = 0;

	while(p <= n*n - 1){
	
	int col_pos = p %n;
	int row_pos = floor(p/n);
	
	if( col_pos == row_pos & mat[p] != 1){
	return 0;
	}
	if( col_pos != row_pos & mat[p] != 0){
	return 0;}
	p = p + 1;
	}
	
	return 1;

}

void run_test(const char* test_name, int* mat, int n, int expected) {
    int result = matriz_identidade(mat, n);
    printf("Teste: %-30s | N=%d | Esperado: %d | Obtido: %d | ",
           test_name, n, expected, result);

    if (result == expected) {
        printf("RESULTADO: OK\n");
    } else {
        printf("RESULTADO: FALHOU\n");
    }
}


int main() {
    printf("--- Iniciando Testes da função matriz_identidade ---\n");
    printf("====================================================\n");

    // Vetores de Teste
    int id_2x2[] = {1, 0, 0, 1};
    int id_3x3[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    int diag_err_2x2[] = {2, 0, 0, 1}; // Erro na diagonal (2 em vez de 1)
    int diag_err_3x3[] = {1, 0, 0, 0, 5, 0, 0, 0, 1}; // Erro na diagonal (5 em vez de 1)
    int out_diag_err_2x2[] = {1, 3, 0, 1}; // Erro fora da diagonal (3 em vez de 0)
    int out_diag_err_3x3[] = {1, 0, 0, 0, 1, 0, 4, 0, 1}; // Erro fora da diagonal (4 em vez de 0)
    int null_2x2[] = {0, 0, 0, 0}; // Matriz Nula (não é identidade)
    int id_1x1[] = {1};
    int not_id_1x1[] = {5};

    // Execução dos Testes
    
    // CASOS DE SUCESSO (Deve retornar 1)
    run_test("Identidade 2x2 (Sucesso)", id_2x2, 2, 1);
    run_test("Identidade 3x3 (Sucesso)", id_3x3, 3, 1);
    run_test("Identidade 1x1 (Sucesso)", id_1x1, 1, 1);

    // CASOS DE FALHA NA DIAGONAL (Deve retornar 0)
    run_test("Diagonal Errada 2x2", diag_err_2x2, 2, 0);
    run_test("Diagonal Errada 3x3", diag_err_3x3, 3, 0);
    run_test("Não Identidade 1x1", not_id_1x1, 1, 0);

    // CASOS DE FALHA FORA DA DIAGONAL (Deve retornar 0)
    run_test("Fora Diagonal Errada 2x2", out_diag_err_2x2, 2, 0);
    run_test("Fora Diagonal Errada 3x3", out_diag_err_3x3, 3, 0);
    
    // CASOS ESPECIAIS (Matriz Nula)
    run_test("Matriz Nula 2x2", null_2x2, 2, 0);

    printf("====================================================\n");
    printf("Fim dos Testes.\n");

    return 0;
}
