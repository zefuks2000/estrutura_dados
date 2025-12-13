#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

struct matrix {
    struct matrix* right;
    struct matrix* below;
    int line;
    int column;
    float info;
};

// Função auxiliar para obter ou criar cabeça de linha
static Matrix* get_line_header(Matrix* m, int line) {
    if (m == NULL || line <= 0) return NULL;
    
    Matrix* current = m;
    // Percorre pela coluna 0 (cabeças de linha)
    while (current->below != NULL && current->below->line < line) {
        current = current->below;
    }
    
    // Se encontrou, retorna
    if (current->below != NULL && current->below->line == line) {
        return current->below;
    }
    
    // Cria nova cabeça de linha
    Matrix* line_header = (Matrix*)malloc(sizeof(Matrix));
    if (line_header == NULL) return NULL;
    
    line_header->line = line;
    line_header->column = 0;
    line_header->info = 0.0;
    line_header->right = NULL;
    line_header->below = current->below;
    current->below = line_header;
    
    return line_header;
}

// Função auxiliar para obter ou criar cabeça de coluna
static Matrix* get_column_header(Matrix* m, int column) {
    if (m == NULL || column <= 0) return NULL;
    
    Matrix* current = m;
    // Percorre pela linha 0 (cabeças de coluna)
    while (current->right != NULL && current->right->column < column) {
        current = current->right;
    }
    
    // Se encontrou, retorna
    if (current->right != NULL && current->right->column == column) {
        return current->right;
    }
    
    // Cria nova cabeça de coluna
    Matrix* col_header = (Matrix*)malloc(sizeof(Matrix));
    if (col_header == NULL) return NULL;
    
    col_header->line = 0;
    col_header->column = column;
    col_header->info = 0.0;
    col_header->below = NULL;
    col_header->right = current->right;
    current->right = col_header;
    
    return col_header;
}

// Função auxiliar para encontrar nó na linha
static Matrix* find_node_in_line(Matrix* line_header, int column) {
    if (line_header == NULL) return NULL;
    
    Matrix* current = line_header;
    while (current->right != NULL && current->right->column < column) {
        current = current->right;
    }
    
    if (current->right != NULL && current->right->column == column) {
        return current->right;
    }
    
    return NULL;
}

// Função auxiliar para encontrar nó na coluna
static Matrix* find_node_in_column(Matrix* col_header, int line) {
    if (col_header == NULL) return NULL;
    
    Matrix* current = col_header;
    while (current->below != NULL && current->below->line < line) {
        current = current->below;
    }
    
    if (current->below != NULL && current->below->line == line) {
        return current->below;
    }
    
    return NULL;
}

int matrix_create(Matrix** m) {
    if (m == NULL) return -1;
    
    *m = (Matrix*)malloc(sizeof(Matrix));
    if (*m == NULL) return -1;
    
    (*m)->right = NULL;
    (*m)->below = NULL;
    (*m)->line = 0;
    (*m)->column = 0;
    (*m)->info = 0.0;
    
    return 0;
}

int matrix_destroy(Matrix* m) {
    if (m == NULL) return -1;
    
    // Primeiro, libera todos os nós de dados (não cabeças)
    Matrix* current_line = m->below;
    Matrix* next_line;
    Matrix* current_node;
    Matrix* next_node;
    
    // Percorre todas as linhas (exceto cabeças de coluna na linha 0)
    while (current_line != NULL) {
        next_line = current_line->below;
        current_node = current_line->right;
        
        // Libera todos os nós da linha
        while (current_node != NULL) {
            next_node = current_node->right;
            free(current_node);
            current_node = next_node;
        }
        
        // Libera o cabeçalho da linha
        free(current_line);
        current_line = next_line;
    }
    
    // Agora libera os cabeçalhos de coluna (na linha 0)
    Matrix* current_col = m->right;
    Matrix* next_col;
    while (current_col != NULL) {
        next_col = current_col->right;
        free(current_col);
        current_col = next_col;
    }
    
    // Por fim, libera o nó cabeça principal
    free(m);
    
    return 0;
}

int matrix_getelem(const Matrix* m, int x, int y, float *elem) {
    if (m == NULL || elem == NULL) return -1;
    if (x <= 0 || y <= 0) return -1;
    
    Matrix* line_header = get_line_header((Matrix*)m, x);
    if (line_header == NULL) {
        *elem = 0.0;
        return 0;
    }
    
    Matrix* node = find_node_in_line(line_header, y);
    if (node == NULL) {
        *elem = 0.0;
        return 0;
    }
    
    *elem = node->info;
    return 0;
}

int matrix_setelem(Matrix* m, int x, int y, float elem) {
    if (m == NULL) return -1;
    if (x <= 0 || y <= 0) return -1;
    
    // Se o elemento é zero, remove o nó se existir
    if (elem == 0.0) {
        Matrix* line_header = get_line_header(m, x);
        if (line_header == NULL) return 0;
        
        Matrix* current = line_header;
        while (current->right != NULL && current->right->column < y) {
            current = current->right;
        }
        
        if (current->right != NULL && current->right->column == y) {
            Matrix* to_remove = current->right;
            current->right = to_remove->right;
            
            // Remove também da lista de colunas
            Matrix* col_header = get_column_header(m, y);
            if (col_header != NULL) {
                Matrix* col_current = col_header;
                while (col_current->below != NULL && col_current->below->line < x) {
                    col_current = col_current->below;
                }
                if (col_current->below != NULL && col_current->below->line == x) {
                    col_current->below = to_remove->below;
                }
            }
            
            free(to_remove);
        }
        
        return 0;
    }
    
    // Obtém cabeças de linha e coluna
    Matrix* line_header = get_line_header(m, x);
    Matrix* col_header = get_column_header(m, y);
    
    if (line_header == NULL || col_header == NULL) return -1;
    
    // Verifica se o nó já existe na linha
    Matrix* node = find_node_in_line(line_header, y);
    
    if (node == NULL) {
        // Cria novo nó
        node = (Matrix*)malloc(sizeof(Matrix));
        if (node == NULL) return -1;
        
        node->line = x;
        node->column = y;
        node->info = elem;
        
        // Insere na linha
        Matrix* line_current = line_header;
        while (line_current->right != NULL && line_current->right->column < y) {
            line_current = line_current->right;
        }
        node->right = line_current->right;
        line_current->right = node;
        
        // Insere na coluna
        Matrix* col_current = col_header;
        while (col_current->below != NULL && col_current->below->line < x) {
            col_current = col_current->below;
        }
        node->below = col_current->below;
        col_current->below = node;
    } else {
        // Atualiza valor existente
        node->info = elem;
    }
    
    return 0;
}

int matrix_print(const Matrix* m) {
    if (m == NULL) return -1;
    
    // Encontra dimensões da matriz considerando cabeças de linha e coluna
    int max_line = 0, max_column = 0;
    Matrix* line_header = m->below;
    
    while (line_header != NULL) {
        if (line_header->line > max_line) max_line = line_header->line;
        Matrix* node = line_header->right;
        while (node != NULL) {
            if (node->column > max_column) max_column = node->column;
            node = node->right;
        }
        line_header = line_header->below;
    }
    
    // Verifica também as cabeças de coluna
    Matrix* col_header = m->right;
    while (col_header != NULL) {
        if (col_header->column > max_column) max_column = col_header->column;
        col_header = col_header->right;
    }
    
    if (max_line == 0 || max_column == 0) {
        printf("Matriz vazia\n");
        return 0;
    }
    
    // Imprime a matriz
    for (int i = 1; i <= max_line; i++) {
        for (int j = 1; j <= max_column; j++) {
            float elem;
            matrix_getelem(m, i, j, &elem);
            printf("%.2f", elem);
            if (j < max_column) printf(" ");
        }
        printf("\n");
    }
    
    return 0;
}

int matrix_add(const Matrix* m, const Matrix* n, Matrix** r) {
    if (m == NULL || n == NULL || r == NULL) return -1;
    
    if (matrix_create(r) != 0) return -1;
    
    // Encontra dimensões máximas considerando cabeças de linha e coluna
    int max_line = 0, max_column = 0;
    
    // Percorre m - encontra max_line e max_column
    Matrix* line_header = m->below;
    while (line_header != NULL) {
        if (line_header->line > max_line) max_line = line_header->line;
        Matrix* node = line_header->right;
        while (node != NULL) {
            if (node->column > max_column) max_column = node->column;
            node = node->right;
        }
        line_header = line_header->below;
    }
    
    // Percorre cabeças de coluna de m
    Matrix* col_header = m->right;
    while (col_header != NULL) {
        if (col_header->column > max_column) max_column = col_header->column;
        col_header = col_header->right;
    }
    
    // Percorre n - encontra max_line e max_column
    line_header = n->below;
    while (line_header != NULL) {
        if (line_header->line > max_line) max_line = line_header->line;
        Matrix* node = line_header->right;
        while (node != NULL) {
            if (node->column > max_column) max_column = node->column;
            node = node->right;
        }
        line_header = line_header->below;
    }
    
    // Percorre cabeças de coluna de n
    col_header = n->right;
    while (col_header != NULL) {
        if (col_header->column > max_column) max_column = col_header->column;
        col_header = col_header->right;
    }
    
    // Cria cabeças de coluna para todas as colunas (para preservar dimensões)
    for (int j = 1; j <= max_column; j++) {
        get_column_header(*r, j);
    }
    
    // Soma os elementos
    for (int i = 1; i <= max_line; i++) {
        for (int j = 1; j <= max_column; j++) {
            float elem1, elem2;
            matrix_getelem(m, i, j, &elem1);
            matrix_getelem(n, i, j, &elem2);
            float sum = elem1 + elem2;
            if (sum != 0.0) {
                matrix_setelem(*r, i, j, sum);
            }
        }
    }
    
    return 0;
}

int matrix_multiply(const Matrix* m, const Matrix* n, Matrix** r) {
    if (m == NULL || n == NULL || r == NULL) return -1;
    
    if (matrix_create(r) != 0) return -1;
    
    // Encontra dimensões
    int m_max_line = 0, m_max_column = 0;
    int n_max_line = 0, n_max_column = 0;
    
    // Dimensões de m
    Matrix* line_header = m->below;
    while (line_header != NULL) {
        if (line_header->line > m_max_line) m_max_line = line_header->line;
        Matrix* node = line_header->right;
        while (node != NULL) {
            if (node->column > m_max_column) m_max_column = node->column;
            node = node->right;
        }
        line_header = line_header->below;
    }
    
    // Verifica cabeças de coluna de m
    Matrix* col_header = m->right;
    while (col_header != NULL) {
        if (col_header->column > m_max_column) m_max_column = col_header->column;
        col_header = col_header->right;
    }
    
    // Dimensões de n
    line_header = n->below;
    while (line_header != NULL) {
        if (line_header->line > n_max_line) n_max_line = line_header->line;
        Matrix* node = line_header->right;
        while (node != NULL) {
            if (node->column > n_max_column) n_max_column = node->column;
            node = node->right;
        }
        line_header = line_header->below;
    }
    
    // Verifica cabeças de coluna de n
    col_header = n->right;
    while (col_header != NULL) {
        if (col_header->column > n_max_column) n_max_column = col_header->column;
        col_header = col_header->right;
    }
    
    // Verifica se é possível multiplicar
    if (m_max_column != n_max_line) return -1;
    
    // Cria cabeças de coluna para todas as colunas (para preservar dimensões)
    for (int j = 1; j <= n_max_column; j++) {
        get_column_header(*r, j);
    }
    
    // Multiplica
    for (int i = 1; i <= m_max_line; i++) {
        for (int j = 1; j <= n_max_column; j++) {
            float sum = 0.0;
            for (int k = 1; k <= m_max_column; k++) {
                float elem1, elem2;
                matrix_getelem(m, i, k, &elem1);
                matrix_getelem(n, k, j, &elem2);
                sum += elem1 * elem2;
            }
            if (sum != 0.0) {
                matrix_setelem(*r, i, j, sum);
            }
        }
    }
    
    return 0;
}

int matrix_transpose(const Matrix* m, Matrix** r) {
    if (m == NULL || r == NULL) return -1;
    
    if (matrix_create(r) != 0) return -1;
    
    // Percorre todos os elementos de m e os insere transpostos em r
    Matrix* line_header = m->below;
    while (line_header != NULL) {
        Matrix* node = line_header->right;
        while (node != NULL) {
            matrix_setelem(*r, node->column, node->line, node->info);
            node = node->right;
        }
        line_header = line_header->below;
    }
    
    return 0;
}

int matrix_read(Matrix** m) {
    if (m == NULL) return -1;
    
    if (matrix_create(m) != 0) return -1;
    
    int lines, columns;
    if (scanf("%d %d", &lines, &columns) != 2) {
        matrix_destroy(*m);
        return -1;
    }
    
    if (lines <= 0 || columns <= 0) {
        matrix_destroy(*m);
        return -1;
    }
    
    float value;
    for (int i = 1; i <= lines; i++) {
        for (int j = 1; j <= columns; j++) {
            if (scanf("%f", &value) != 1) {
                matrix_destroy(*m);
                return -1;
            }
            if (value != 0.0) {
                if (matrix_setelem(*m, i, j, value) != 0) {
                    matrix_destroy(*m);
                    return -1;
                }
            }
        }
    }
    
    return 0;
}
