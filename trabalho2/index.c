
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "index.h"

#define MAXSIZE 17
#define HASH_SIZE 101

// Definição das estruturas usada: A tabela tem uma quantidade de entradas e cada entrada
// se liga a um nó de ocorrencia, que guarda a quantidades de ocorrecias das palavras
typedef struct OccNode {
    int line;
    struct OccNode *next;
} OccNode;

typedef struct Entry {
    char key[MAXSIZE];
    OccNode *occurrences;
    struct Entry *next; 
} Entry;

typedef struct Index {
    Entry *table[HASH_SIZE];
    char *text_filename;
} Index;

static int hash_function(const char *str) {
    unsigned int h = 0;
    while (*str)
        h = 31 * h + (unsigned char)*str++;
    return h % HASH_SIZE;
}

/* Essa função busca a palavra no arquivo de texto e monta a lista de ocorrencias correspondente
*/
static OccNode* cria_lista_ocorrencias(const char *text_file, const char *key) {
    FILE *fp = fopen(text_file, "r");
    if (!fp) return NULL;

    OccNode *head = NULL, *tail = NULL;
    char line[1024];
    int line_num = 0;

    while (fgets(line, sizeof(line), fp)) {
        line_num++;
        if (strstr(line, key)) {
            OccNode *node = malloc(sizeof(OccNode));
            if (!node) break;
            node->line = line_num;
            node->next = NULL;

            if (!head) head = node;
            else tail->next = node;
            tail = node;
        }
    }

    fclose(fp);
    return head;
}

/* Funções principais */

int index_createfrom(const char *key_file, const char *text_file, Index **idx) {
    FILE *kf = fopen(key_file, "r");
    if (!kf) return 1;

    Index *I = malloc(sizeof(Index));
    if (!I) {
        fclose(kf);
        return 2;
    }

    for (int i = 0; i < HASH_SIZE; i++)
        I->table[i] = NULL;

    I->text_filename = strdup(text_file);

    char key[MAXSIZE];
    while (fgets(key, sizeof(key), kf)) {
        key[strcspn(key, "\n\r")] = '\0'; // Remove \n e \r
        if (strlen(key) > 0) {
            index_put(I, key);
        }
    }

    fclose(kf);
    *idx = I;
    return 0;
}

int index_put(Index *idx, const char *key) {
    unsigned int h = hash_function(key);
    Entry *e = idx->table[h];

    while (e) {
        if (strcmp(e->key, key) == 0)
            break;
        e = e->next;
    }

    if (!e) {
        e = malloc(sizeof(Entry));
        if (!e) return 2;
        strncpy(e->key, key, MAXSIZE - 1);
        e->key[MAXSIZE - 1] = '\0';
        e->occurrences = NULL;
        e->next = idx->table[h];
        idx->table[h] = e;
    }

    /* Libera ocorrências antigas se houver atualização */
    OccNode *o = e->occurrences;
    while (o) {
        OccNode *tmp = o;
        o = o->next;
        free(tmp);
    }

    e->occurrences = cria_lista_ocorrencias(idx->text_filename, key);
    return 0;
}

int index_get(const Index *idx, const char *key, int **occurrences, int *num_occurrences) {
    unsigned int h = hash_function(key);
    Entry *e = idx->table[h];

    while (e) {
        if (strcmp(e->key, key) == 0)
            break;
        e = e->next;
    }

if (!e) {
    return 1; // palavra não pertence ao índice
}

if (!e->occurrences) {
    *num_occurrences = 0;
    *occurrences = NULL;
    return 0; // pertence, mas não ocorre
}

    int count = 0;
    OccNode *o = e->occurrences;
    while (o) {
        count++;
        o = o->next;
    }

    *num_occurrences = count;
    *occurrences = malloc(sizeof(int) * count);
    if (!*occurrences) return 2;

    o = e->occurrences;
    for (int i = 0; i < count; i++) {
        (*occurrences)[i] = o->line;
        o = o->next;
    }

    return 0;
}

static int cmp_entries(const void *a, const void *b) {
    Entry *ea = *(Entry **)a;
    Entry *eb = *(Entry **)b;
    return strcmp(ea->key, eb->key);
}

int index_print(const Index *idx) {
    int total = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        Entry *e = idx->table[i];
        while (e) {
            total++;
            e = e->next;
        }
    }

    if (total == 0) return 0;

    Entry **vec = malloc(sizeof(Entry *) * total);
    int k = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        Entry *e = idx->table[i];
        while (e) {
            vec[k++] = e;
            e = e->next;
        }
    }

    qsort(vec, total, sizeof(Entry *), cmp_entries);

    for (int i = 0; i < total; i++) {
        printf("%s:", vec[i]->key);
        OccNode *o = vec[i]->occurrences;
        if (o) {
            printf(" ");
            while (o) {
                printf("%d", o->line);
                if (o->next) printf(", ");
                o = o->next;
            }
        }
        printf("\n");
    }

    free(vec);
    return 0;
}

void index_destroy(Index *idx) {
    if (!idx) return;

    for (int i = 0; i < HASH_SIZE; i++) {
        Entry *e = idx->table[i];
        while (e) {
            Entry *tmp_e = e;
            e = e->next;

            OccNode *o = tmp_e->occurrences;
            while (o) {
                OccNode *tmp_o = o;
                o = o->next;
                free(tmp_o);
            }
            free(tmp_e);
        }
    }
    free(idx->text_filename);
    free(idx);
}