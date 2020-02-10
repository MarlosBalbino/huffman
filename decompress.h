#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct binary_tree binary_tree;

/*
compressed_file: é o ponteiro que aponta para o arquivo já comprimido.
Obtém o tamanho do arquivo comprimido, do lixo e da árvore binária.
*/
void get_sizes(FILE *compressed_file);

/*
Cria um nó para a arvore binária lida no arquivo comprimido.
item: é um caractere que pertence a um nó folha da arvóre binária ou um asterisco
que pertence a um nó interno desta mesma árvore.
*/
binary_tree* create_new_bt(unsigned char item, binary_tree *left, binary_tree *right);

/*
lê e monta a árvore binária escrita no arquivo comprimido.
*/
binary_tree* get_tree(FILE *compressed_file, binary_tree *bt);

int is_bit_i_set(unsigned char c, int i);

/*
original_file: é o ponteiro que aponta para o arquivo descomprimido.
Aqui a função lê todo o arquivo comprimido e tranforma cada caractere em seu valor
binário original e os escreve no arquivo descomprimido.
*/
void write_file_out(FILE *compressed_file, FILE *original_file, binary_tree *bt);

/*
Gerecia toda a parde da descompressão.
Aqui é requisitado o nome do arquivo, o qual estará descomprimido e são chamadas todas as 
funções descritas nesse arquivo.
*/
void decompress();

