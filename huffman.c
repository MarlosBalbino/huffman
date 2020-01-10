#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int* open_file_and_count_chars(int *ascii)
{
    FILE *file = fopen("test.txt", "r");

    if(file == NULL)
    {
        printf("Deu merda");
        return 0;
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

void print_binary_tree_pre_order(node *bt)
{
    if (bt != NULL) {
        printf("%c", bt->item);
        print_binary_tree_pre_order(bt->left);
        print_binary_tree_pre_order(bt->right);
    }
}

int main()
{
    int ascii[256] = {0};

    open_file_and_count_chars(ascii);
    priority_queue *pq = create_empty_priority_queue();
    put_chars_in_priority_queue(ascii, pq);
    create_huffman_binary_tree(pq);

    node *aux = pq->head;

    //imprime arvore binaria
    print_binary_tree_pre_order(aux);




    return 0;
}