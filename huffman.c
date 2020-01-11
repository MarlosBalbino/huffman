#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

priority_queue* create_empty_priority_queue()
{
    priority_queue *pq = (priority_queue*) malloc(sizeof(priority_queue));
    pq->head = NULL;
    return pq;
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

void create_path_to_hash_table(node *bt, hash_table *ht, int *binary_path, int path_size)
{
    
    if (bt->left != NULL && bt->right != NULL) 
    {
        binary_path[path_size] = 0;
        create_path_to_hash_table(bt->left, ht, binary_path, path_size + 1);
        binary_path[path_size] = 1;
        create_path_to_hash_table(bt->right, ht, binary_path, path_size + 1);
    }
    else
    {
        set_path_to_hash_table(bt->item, ht, binary_path, path_size);
    }    
}

void get(hash_table *ht, int key)
{
	element *current = ht->table[key];

    printf("%c: ", key);
	while(current != NULL)
	{
		printf("%d ", current->value);
		current = current->next;
	}
    printf("\n");
}

void print_binary_tree_pre_order(node *bt)
{
    if (bt != NULL) 
    {
        printf("%c", bt->item);
        print_binary_tree_pre_order(bt->left);
        print_binary_tree_pre_order(bt->right);
    }
}

int main()
{
    FILE *file = fopen("test.txt", "r");
    int ascii[256] = {0};

    count_chars(ascii, file);
    priority_queue *pq = create_empty_priority_queue();
    put_chars_in_priority_queue(ascii, pq);
    create_huffman_binary_tree(pq);

    hash_table *ht = create_hash_table();
    int binary_path[256];
    create_path_to_hash_table(pq->head, ht, binary_path, 0);
    get(ht, 'C');
    get(ht, 'B');
    get(ht, 'F');
    get(ht, 'E');
    get(ht, 'D');
    get(ht, 'A');

    print_binary_tree_pre_order(pq->head);
    printf("\n");

    // rewind(file);
    // FILE *file_out = fopen("compress.txt", "w");
    // char x;
    
    //  while(fscanf(file, "%c", &x) != EOF)
    // {
    //     fprintf(file_out, "%c", x);
    // }

    
   
   






    return 0;
}
