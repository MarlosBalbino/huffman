#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "compress.h"
#include "decompress.h"

int main()
{
    int option;    
    while (1)
    {
        printf("Type: 1 to Compress - 2 to Decompress - 0 to Exit.\n");
        scanf("%d", &option);
        if(option == 1) compress();
        else if(option == 2) decompress();
        else if(option == 0) return 0;        
        else printf("\nError: Invalid comand\n\n");     
    }
    
    return 0;
}
