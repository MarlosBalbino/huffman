#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma region structs
typedef struct node node;
struct node
{
    char item;
    int priority;
    node *next;
    node *left;
    node *right;
};

typedef struct priority_queue
{
    node *head;
}priority_queue;

typedef struct element element;
struct element
{
	int value;
	element *next;
};

typedef struct hash_table
{
	element *table[256];
}hash_table;
#pragma endregion

#pragma region nodes
priority_queue* create_empty_priority_queue()
{
    priority_queue *pq = (priority_queue*) malloc(sizeof(priority_queue));
    pq->head = NULL;
    return pq;
}

node* create_new_node(char i, int p, node *left, node *right)
{
    node *new_node = (node*) malloc(sizeof(node));
    new_node->item = i;
    new_node->priority = p;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

hash_table* create_hash_table()
{
	hash_table *new_hash_table = (hash_table*) malloc(sizeof(hash_table));

	
	for(int i = 0; i < 256; i++)
	{
		new_hash_table->table[i] = NULL;
	}
	return new_hash_table;
}

element* create_new_element(int value) 
{
	element *new_element = (element*) malloc(sizeof(element));
	new_element->value = value;
	new_element->next = NULL;
	return new_element;
}
#pragma endregion

void count_chars(int *ascii, FILE *file)
{
    if(file == NULL)
    {
        printf("Deu merda");
        return;
    }

    char x;
    
    while(fscanf(file, "%c", &x) != EOF)
    {
        ascii[x]++;
    }
}

void enqueue(priority_queue *pq, node *new_node)
{
    if(pq->head == NULL || new_node->priority <= pq->head->priority)
    {
        new_node->next = pq->head;
        pq->head = new_node;
    }
    else
    {
        node *current = pq->head;
        while(current->next != NULL && current->next->priority < new_node->priority)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }    
}

priority_queue* put_chars_in_priority_queue(int *ascii, priority_queue *pq)
{
    node *node = NULL;
    for(int i = 0; i < 256; i++)
    {
        if(ascii[i] != 0)
        {
            node = create_new_node(i, ascii[i], NULL, NULL);
            enqueue(pq, node);
        }
    }
    return pq;
}

void create_huffman_binary_tree(priority_queue *pq)
{
    node *new_node = NULL;
    node *aux = NULL;

    while(1)
    {
        new_node = create_new_node('*', pq->head->priority + pq->head->next->priority, pq->head, pq->head->next);
        aux = pq->head;
        if(pq->head->next->next != NULL)
        {
            pq->head = pq->head->next->next;
            aux->next->next = NULL;
            aux->next = NULL;
            enqueue(pq, new_node);
        }
        else
        {
            break;
        }                
    }
    pq->head = new_node;
    aux->next = NULL;
}

void set_path_to_hash_table(char key, hash_table *ht, int *binary_path, int path_size)
{    
	element *new_element = NULL;
	element *current = NULL;

	for(int i = 0; i < path_size; i++) 
	{
		new_element = create_new_element(binary_path[i]);

		if(ht->table[key] != NULL)
		{
			current->next = new_element;
			current = new_element;
		}
		else
		{
			ht->table[key] = new_element;
			current = new_element;			
		}
	}	
}

void write_bt_set_ht(node *bt, hash_table *ht, int *binary_path, int path_size, int *bt_size, FILE *file_out)
{ 
    (*bt_size)++;
    if (bt->left != NULL && bt->right != NULL) 
    {        
        fputc(bt->item, file_out);
        binary_path[path_size] = 0;
        write_bt_set_ht(bt->left, ht, binary_path, path_size + 1, bt_size, file_out);
        binary_path[path_size] = 1;
        write_bt_set_ht(bt->right, ht, binary_path, path_size + 1, bt_size, file_out);
    }
    else
    {
            
        if(bt->item == '*') fputc('\\', file_out), (*bt_size)++;        
        if(bt->item == '\\') fputc('\\', file_out), (*bt_size)++;   
        fputc(bt->item, file_out);
        set_path_to_hash_table(bt->item, ht, binary_path, path_size);
    }
}

// void get(hash_table *ht, int key)
// {
// 	element *current = ht->table[key];

//     printf("%c: ", key);
// 	while(current != NULL)
// 	{
// 		printf("%d ", current->value);
// 		current = current->next;
// 	}
//     printf("\n");
// }

unsigned char set_bit(unsigned char c, int i)
{
	unsigned char mask = 0;
	mask = 1 << i;
	return mask | c;
}

void compress(hash_table *ht, FILE *file, FILE *file_out, short bt_size)
{
    element *current = NULL;
    char key;
    int bit;
    int i = 7;
    unsigned char c = 0;
    while(fscanf(file, "%c", &key) != EOF)
    {    
        printf("teste key: %c\n", key); 
        current = ht->table[key];
        while(current != NULL)
        {            
            bit = current->value;
            current = current->next;

            if(bit)
            {
                c = set_bit(c, i);
            }            

            if(i == 0)
            {
                fputc(c, file_out);
                c = 0;
                i = 8;
            }
            i--;
        }  
    }
    fputc(c, file_out);
    //rewind(file_out);
    // char trash = i+1;
	// char byte_1 = (trash<<5) | (bt_size>>8);
	// char byte_2 = bt_size;
	// fputc(byte_1, file_out);
	// fputc(byte_2, file_out);
}

// void print_binary_tree_pre_order(node *bt)
// {
//     if (bt != NULL)
//     {
//         printf("%c", bt->item);
//         print_binary_tree_pre_order(bt->left);
//         print_binary_tree_pre_order(bt->right);
//     }
// }

int main()
{
    int bt_size = 0;
    int ascii[256] = {0};
    int binary_path[256] = {0};
    
    FILE *file = fopen("test.jpg", "r");
    FILE *file_out = fopen("compress.txt", "w");
    
    count_chars(ascii, file);
    rewind(file);
    priority_queue *pq = create_empty_priority_queue();
    put_chars_in_priority_queue(ascii, pq);
    //printf("menor prioridade: %c \n", pq->head->item);
    create_huffman_binary_tree(pq);
    hash_table *ht = create_hash_table();
    fputc(0, file_out);
    fputc(0, file_out);
    system("pause");
    write_bt_set_ht(pq->head, ht, binary_path, 0, &bt_size, file_out);
    printf("bt_size: %d\n", bt_size);
    // get(ht, '*');
    // get(ht, 'B');
    // get(ht, 'F');
    // get(ht, 'E');
    // get(ht, 'D');
    // get(ht, 'A');  
    compress(ht, file, file_out, bt_size);
    printf("teste\n");
    fclose(file);
    fclose(file_out);

    //print_binary_tree_pre_order(pq->head);
    // printf("\n");
    
   
   






    return 0;
}
