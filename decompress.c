#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

unsigned long long f_size; //tamanho do arquivo comprimido.
unsigned char byte_1; //primeiro byte do arquivo original e comprimido.
unsigned char byte_2;
unsigned char trash;
unsigned char c = 0;
short bt_sz2 = 0;

typedef struct binary_tree binary_tree;
struct binary_tree
{
    unsigned char item;
    binary_tree *left;
    binary_tree *right;
};

void get_sizes(FILE *compressed_file)
{  
    fseek(compressed_file, 0, 2);
    f_size = ftell(compressed_file);
    rewind(compressed_file);
    byte_1 = fgetc(compressed_file);    
    byte_2 = fgetc(compressed_file);
    trash = (byte_1 >> 5);
    byte_1 = (byte_1 << 3);
    byte_1 = (byte_1 >> 3);
    bt_sz2 = bt_sz2 | byte_1;
    bt_sz2 = (bt_sz2 << 8) | byte_2;
}

binary_tree* create_new_bt(unsigned char item, binary_tree *left, binary_tree *right)
{
    binary_tree *new_binary_tree = (binary_tree*) malloc(sizeof(binary_tree));
    new_binary_tree->item = item;
    new_binary_tree->left = left;
    new_binary_tree->right = right;
    return new_binary_tree;
}

binary_tree* get_tree(FILE *compressed_file, binary_tree *bt)
{
    c = fgetc(compressed_file);
    if(c == '*')
    {
        bt = create_new_bt(c, NULL, NULL);
        bt->left = get_tree(compressed_file, bt->left);
        bt->right = get_tree(compressed_file, bt->right);
    }
    else
    {
        if(c == '\\')
        {
            c = fgetc(compressed_file);
        }
        bt = create_new_bt(c, NULL, NULL);
    }
    return bt;    
}

int is_bit_i_set(unsigned char c, int i)
{
    unsigned char mask = 0;
    mask = 1 << i;
    return mask & c;
}

void write_file_out(FILE *compressed_file, FILE *original_file, binary_tree *bt)
{
    int i;
    int is_bit;
    binary_tree *current = bt;    
    unsigned long long current_f_size = ftell(compressed_file);

    while (current_f_size+1 != f_size)
    {
        c = fgetc(compressed_file);
        current_f_size = ftell(compressed_file);
        for(i = 7; i >= 0; i--)
        {
            if(is_bit_i_set(c, i))
            {
                current = current->right;
            }
            else
            {
                current = current->left;
            }            
            if(current->left == NULL && current->right == NULL)
            {
                fputc(current->item, original_file);
                current = bt;
            }            
        }
        i = 7;        
    }
    c = fgetc(compressed_file);
    for(i = 7; i >= trash; i--)
    {
        if(is_bit_i_set(c, i))
        {
            current = current->right;
        }
        else
        {
            current = current->left;
        }            
        if(current->left == NULL && current->right == NULL)
        {
            fputc(current->item, original_file);
            current = bt;
        }
    }    
}

void decompress()
{
    char output_file[50];
    printf("\nType output file name:\n");
    scanf("%s", output_file);

    FILE *compressed_file = fopen("compressed.huff", "rb");
    if(compressed_file == NULL)
    {
        printf("\nError: File not fund\n\n");
        return;
    }
    FILE *original_file = fopen(output_file, "wb");

    printf("\n------------------------- DECOMPRESSING ---------------------------\n");
    binary_tree *bt = NULL;
    get_sizes(compressed_file);
    bt = get_tree(compressed_file, bt);
    write_file_out(compressed_file, original_file, bt);
    printf("Decompress concluded :)\n");
    printf("-------------------------------------------------------------------\n\n");

    fclose(compressed_file);
    fclose(original_file);
    free(bt);
}
