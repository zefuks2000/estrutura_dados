#ifndef INDEX_H
#define INDEX_H

#define MAXSIZE 17

typedef struct Index Index;

int index_createfrom(const char *key_file, const char *text_file, Index **idx);
int index_put(Index *idx, const char *key);
int index_get(const Index *idx, const char *key, int **occurrences, int *num_occurrences);
int index_print(const Index *idx);
void index_destroy(Index *idx);

#endif