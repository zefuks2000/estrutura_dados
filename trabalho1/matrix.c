#include <stdlib.h>
#include <stdio.h>

typedef struct matrix {
    struct matrix* right;
    struct matrix* below;
    int line;
    int column;
    float info;
} Matrix;

Matrix* cria_no(Matrix* right_v, Matrix* below_v, int line_v, int column_v, float info_v) {
    Matrix* no = (Matrix*)malloc(sizeof(Matrix));
    if (no) {
        no->right = right_v;
        no->below = below_v;
        no->line = line_v;
        no->column = column_v;
        no->info = info_v;
    }
    return no;
}

Matrix* cria_matriz_vazia(int m, int n) {
    // Nó (linha = -1,coluna = -1) 
    Matrix* cabeca_principal = cria_no(NULL, NULL, -1, -1, 0);
    if (!cabeca_principal) return NULL;

    Matrix* atual = cabeca_principal;

    // Criando nós cabeçalhos de LINHA
    for (int i = 1; i <= m; i++) {
        Matrix* nova_linha = cria_no(NULL, NULL, i, -1, 0);
        nova_linha->right = nova_linha; // Aponta para si mesma (linha vazia)
        atual->below = nova_linha;
        atual = nova_linha;
    }
    atual->below = cabeca_principal; // Fecha o ciclo das linhas

    // Reinicia para criar cabeçalhos de COLUNA
    atual = cabeca_principal;
    for (int j = 1; j <= n; j++) {
        Matrix* nova_coluna = cria_no(NULL, NULL, -1, j, 0);
        nova_coluna->below = nova_coluna; // Aponta para si mesma (coluna vazia)
        atual->right = nova_coluna;
        atual = nova_coluna;
    }
    atual->right = cabeca_principal; // Fecha o ciclo das colunas

    return cabeca_principal;
}



int matrix_getelem(const Matrix* m, int x, int y, float* out) {
    if (!m || !out) return -1;

    Matrix* lin = m->below;
    while (lin != m && lin->line < x)
        lin = lin->below;
    if (lin == m || lin->line != x) {
        *out = 0;
        return 0;
    }

    Matrix* p = lin->right;
    while (p != lin && p->column < y)
        p = p->right;

    if (p != lin && p->column == y)
        *out = p->info;
    else
        *out = 0;

    return 0;
}


int matrix_setelem(Matrix* m, int x, int y, float elem) {
    if (!m || x <= 0 || y <= 0) return -1;

    /* localizar cabeçalho da linha */
    Matrix* head_l = m->below;
    while (head_l != m && head_l->line < x)
        head_l = head_l->below;
    if (head_l == m || head_l->line != x) return -1;

    /* localizar cabeçalho da coluna */
    Matrix* head_c = m->right;
    while (head_c != m && head_c->column < y)
        head_c = head_c->right;
    if (head_c == m || head_c->column != y) return -1;

    /* busca na linha */
    Matrix* ant_l = head_l;
    Matrix* p = head_l->right;
    while (p != head_l && p->column < y) {
        ant_l = p;
        p = p->right;
    }

    /* nó já existe */
    if (p != head_l && p->column == y) {
        if (elem == 0) {
            /* remove da linha */
            ant_l->right = p->right;

            /* remove da coluna */
            Matrix* ant_c = head_c;
            Matrix* q = head_c->below;
            while (q != head_c && q != p) {
                ant_c = q;
                q = q->below;
            }
            if (q == p)
                ant_c->below = p->below;

            free(p);
        } else {
            p->info = elem;
        }
    }
    /* inserir novo */
    else if (elem != 0) {
        Matrix* novo = cria_no(p, NULL, x, y, elem);
        if (!novo) return -1;
        ant_l->right = novo;

        Matrix* ant_c = head_c;
        Matrix* q = head_c->below;
        while (q != head_c && q->line < x) {
            ant_c = q;
            q = q->below;
        }
        novo->below = q;
        ant_c->below = novo;
    }

    return 0;
}

int matrix_create(Matrix **m) {
    int linhas, colunas;
    int i, j;
    float valor;

    // Tenta ler as dimensões. Se não conseguir ler dois inteiros, retorna erro.
    if (scanf("%d %d", &linhas, &colunas) != 2) {
        return -1; 
    }

    *m = cria_matriz_vazia(linhas, colunas);
    if (*m == NULL) return -1;

    // Loop de triplas
    while (scanf("%d", &i) == 1 && i != 0) {
        if (scanf("%d %f", &j, &valor) == 2) {
            matrix_setelem(*m, i, j, valor);
        } else {

            return -1;
        }
    }

    return 0;
}
int matrix_print(const Matrix* m) {
    if (m == NULL) return -1;

    int max_l = 0;
    int max_c = 0;

    Matrix* aux_l = m->below;
    while (aux_l != m) {
        if (aux_l->line > max_l) max_l = aux_l->line;
        aux_l = aux_l->below;
    }

    Matrix* aux_c = m->right;
    while (aux_c != m) {
        if (aux_c->column > max_c) max_c = aux_c->column;
        aux_c = aux_c->right;
    }

    printf("%d %d\n", max_l, max_c);

    aux_l = m->below; // Começa no cabeçalho da primeira linha
    while (aux_l != m) {
        Matrix* atual = aux_l->right; // Primeiro nó de dado da linha
        
        while (atual != aux_l) {

            printf("%d %d %.1f\n", atual->line, atual->column, atual->info);
            atual = atual->right;
        }
        aux_l = aux_l->below;
    }


    printf("0\n");

    return 0;
}


int matrix_destroy(Matrix* m) {
    if (m == NULL) return -1;

    // 1. Liberar os nós de dados e os cabeçalhos de linha
    Matrix* aux_linha = m->below;
    while (aux_linha != m) {
        Matrix* atual = aux_linha->right;
        
        // Liberar todos os nós de dados desta linha
        while (atual != aux_linha) {
            Matrix* temp = atual;
            atual = atual->right;
            free(temp);
        }
        
        // Agora que a linha está vazia, guardamos o cabeçalho para apagar
        Matrix* temp_head_linha = aux_linha;
        aux_linha = aux_linha->below;
        free(temp_head_linha);
    }


    Matrix* aux_coluna = m->right;
    while (aux_coluna != m) {
        Matrix* temp_head_col = aux_coluna;
        aux_coluna = aux_coluna->right;
        free(temp_head_col);
    }

    // 3. Por fim, liberar o nó mestre
    free(m);

    return 0;
}

int matrix_add(const Matrix* m, const Matrix* n, Matrix** r) {
    int lm = 0, cm = 0, ln = 0, cn = 0;

    for (Matrix* a = m->below; a != m; a = a->below)
        if (a->line > lm) lm = a->line;
    for (Matrix* a = m->right; a != m; a = a->right)
        if (a->column > cm) cm = a->column;

    for (Matrix* a = n->below; a != n; a = a->below)
        if (a->line > ln) ln = a->line;
    for (Matrix* a = n->right; a != n; a = a->right)
        if (a->column > cn) cn = a->column;

    if (lm != ln || cm != cn) return -1;

    *r = cria_matriz_vazia(lm, cm);
    if (!*r) return -1;

    /* copia m */
    for (Matrix* l = m->below; l != m; l = l->below)
        for (Matrix* p = l->right; p != l; p = p->right)
            matrix_setelem(*r, p->line, p->column, p->info);

    /* soma n */
    for (Matrix* l = n->below; l != n; l = l->below) {
        for (Matrix* p = l->right; p != l; p = p->right) {
            float v;
            matrix_getelem(*r, p->line, p->column, &v);
            matrix_setelem(*r, p->line, p->column, v + p->info);
        }
    }

    return 0;
}


int matrix_transpose(const Matrix* m, Matrix** r) {
    int max_l = 0, max_c = 0;
    // Descobre dimensões para inverter
    for(Matrix* a = m->below; a != m; a = a->below) max_l = a->line;
    for(Matrix* a = m->right; a != m; a = a->right) max_c = a->column;

    // Invertemos: linhas de m viram colunas de r e vice-versa
    *r = cria_matriz_vazia(max_c, max_l);
    if (!*r) return -1;

    for (Matrix* lin = m->below; lin != m; lin = lin->below) {
        for (Matrix* atual = lin->right; atual != lin; atual = atual->right) {
            // Inverte linha e coluna aqui
            matrix_setelem(*r, atual->column, atual->line, atual->info);
        }
    }
    return 0;
}

int matrix_multiply(const Matrix* m, const Matrix* n, Matrix** r) {
    int lm = 0, cm = 0, ln = 0, cn = 0;

    for (Matrix* a = m->below; a != m; a = a->below) lm = a->line;
    for (Matrix* a = m->right; a != m; a = a->right) cm = a->column;
    for (Matrix* a = n->below; a != n; a = a->below) ln = a->line;
    for (Matrix* a = n->right; a != n; a = a->right) cn = a->column;

    if (cm != ln) return -1;

    *r = cria_matriz_vazia(lm, cn);
    if (!*r) return -1;

    for (Matrix* lm_h = m->below; lm_h != m; lm_h = lm_h->below) {
        for (Matrix* cn_h = n->right; cn_h != n; cn_h = cn_h->right) {
            float soma = 0;
            Matrix* p = lm_h->right;
            Matrix* q = cn_h->below;

            while (p != lm_h && q != cn_h) {
                if (p->column == q->line) {
                    soma += p->info * q->info;
                    p = p->right;
                    q = q->below;
                } else if (p->column < q->line) {
                    p = p->right;
                } else {
                    q = q->below;
                }
            }

            if (soma != 0)
                matrix_setelem(*r, lm_h->line, cn_h->column, soma);
        }
    }
    return 0;
}
