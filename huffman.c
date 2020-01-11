#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void print_results(int *bin, int array_size, char item)
{
    printf("%c: ", item);
    for(int i = 0; i < array_size; i++)
    {
        printf("%d ", bin[i]);
    }
    printf("\n");
}

void set_path_to_hash_table(node *bt, int *bin, int i)
{
    
    if (bt->left != NULL && bt->right != NULL) 
    {
        bin[i] = 0;
        set_path_to_hash_table(bt->left, bin, i + 1);
        bin[i] = 1;
        set_path_to_hash_table(bt->right, bin, i + 1);
    }
    else
    {
        print_results(bin, i, bt->item);
    }
    
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

    int bin[256];
    set_path_to_hash_table(pq->head, bin, 0);


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
