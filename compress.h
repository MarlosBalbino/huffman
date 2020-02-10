#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct node node;

typedef struct priority_queue priority_queue;

typedef struct element element;

typedef struct hash_table hash_table;

/*
Cria a fila de prioridade, um nó que contém um ponteiro head que aponta 
para o tipo *node.
*/
priority_queue* create_empty_priority_queue();

/*
Cria um novo nó para a fila de prioridade ou para a arvore binária.
i: é um caractere, de um nó folha, existente no arquivo que será comprimido (arquivo original) ou
é um asterisco indicando que ele pertence à um nó interno e não à um nó folha.
Esse caractere será colocado como item no nó criado.
p: é a prioridade do item "i".
left: é um ponteiro que aponta para o nó da esquerda.
right: é um ponteiro que aponta para o nó da direita.
*/
node* create_new_node(unsigned char i, int p, node *left, node *right);

/*
Cria um nó para a hash_table.
Aqui é atribuido NULL às 256 posições do array "table" contido na hash_table;
*/
hash_table* create_hash_table();

/*
Cria um nó do tipo element para a hash_table, que contém os itens value e next.
Atribui 1 ou 0 à "value". Se trata do caminho binário adquirido pela a arvore binária.
Atrubui NULL à next;
*/
element* create_new_element(int value);

/*
Conta a frequência dos caracters contido no arquivo original.
ascii: é um array com 256 posições. Cada indice do array representa um caractere da
tabela ascii.
Cada posição do array incrementa +1 de acordo com a aparição do caractere no arquivo original.
file: é o ponteiro que aponta para o arquivo original.
*/
void count_chars(int *ascii, FILE *file);

/*
pq: é a fila de priodirade.
new_node: é o nó que será enfileirado na fila de prioridade de acordo com sua prioridade.
*/
void enqueue(priority_queue *pq, node *new_node);

/*
Essa função percorre o arrray "ascii", verificando cada caractere que foi lido no 
arquivo original e suas respectivas prioridades.
Aqui é chamada a função "create_new_node" e é passado para ela como argumento cada
caractere, sua priridade, um ponteiro NULL para "left" e "right".
Também é cahamada a funão "enqueue" e é passado para ela como argumento a fila de
prioridade e o nó criado na função "create_new_node".
Por fim, retorna a fila de prioridade para a função main.
*/
priority_queue* put_chars_in_priority_queue(int *ascii, priority_queue *pq);

/*
Aqui o algoritmo tranforma a fila de prioridade na árvore binária.
Aqui também serão chamadas as funções "create_new_node" e "enqueue".
É passado como argumento para a "create_new_node" o asterisco do nó interno, a soma
das prioridade dos dois primerios nós da fila de prioridade e os ponteiros que apontam
para estes.
É passado como argumento para a "enqueue" a fila de prioridade e o nó criado na "create_new_node".

*/
void create_huffman_binary_tree(priority_queue *pq);

/*
ht: é a hash_table.
binary_path: é um array que contém em cada posição os valores 1 ou 0 que compõe o
caminho para carada caractere nos nós folhas da árvore binária.
key: é o caractere de um nó folha que servirá como o indice do array "table" contido
na hash_table.
path_size: é o tamanho do array binary_path.
A função percorre o array binary_path e passa como argumento para a função "create_new_element"
cada valor 1 ou 0 contido no array.
*/
void set_path_to_hash_table(unsigned char key, hash_table *ht, int *binary_path, int path_size);

/*
file_out: é o ponteiro que aponta para o arquivo comprimido.
Aqui a função percorre toda a árvore binária:
criando para cada caractere dos nós folhas, um caminho binário que é adicionado ao 
array "binary_path";
escrevenda a árvore binária no arquivo comprimido;
contando o tamanho da árvore binária;
contando o tamanho do arrray "binary_path".
*/
void write_bt_set_ht(node *bt, hash_table *ht, int *binary_path, int path_size, short *bt_sz, FILE *file_out);

unsigned char set_bit(unsigned char c, int i);

/*
Com o caminho binário de cada caractere já setado na "hash_table", aqui os caracteres
do arquivo original serão lidos, e de acordo com seus respectivos caminhos binários,
eles serão transformados e escritos no arquivo comprimido.
Aqui são escritos, também, no arquivo, o lixo e o tamanho da árvore binária.
*/
void write(hash_table *ht, FILE *file, FILE *file_out);

/*
Gerecia toda a parde da compressão.
Aqui é requisitado o nome do arquivo, o qual será comprimido e são chamadas todas as 
funções descritas nesse arquivo, com exceção da "enqueue", e a "set_path_to_hash_table".
*/
void compress();