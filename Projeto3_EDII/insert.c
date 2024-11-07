/* insert.c
Contains insert() function to insert a key into a btree.
Calls itself recursively until bottom of tree is reached.
Then insert key node.
If node is out of room,
- calls split() to split node
- promotes middle key and rrn of new node
*/
#include "bt.h"
#include <stdio.h>
#include <string.h>
char keySaver[TAMANHO_STR];

short insert (short rrn, char key[TAMANHO_STR], int byteOffSet, short *promo_r_child, char promo_key[TAMANHO_STR], int *promo_byteOffSet){

    BTPAGE page, // current page
    newpage; // new page created if split occurs
    int found, promoted; // boolean values
    short pos,
    p_b_rrn; // rrn promoted from below
    char p_b_key[TAMANHO_STR]; // key promoted from below
    int p_b_byteOffSet;
    if (rrn == NIL)
    {
        strcpy(promo_key, key);
        *promo_byteOffSet = byteOffSet;
        *promo_r_child = NIL;
        return(YES);
    }

    btread(rrn, &page);
    found = search_node ( key, &page, &pos);
    if (found)
    {
        printf ("Chave %s duplicada\n", key);
        return(0);
    }

    promoted = insert(page.child[pos], key, byteOffSet, &p_b_rrn, p_b_key, &p_b_byteOffSet);

    if (!promoted)
    {
        return(NO);
    }

    if(page.keycount < MAXKEYS)
    {   
        ins_in_page(p_b_key, p_b_byteOffSet, p_b_rrn, &page);
        // printf("repete sem split\n\n");
        //printf("\n page key %s\n", page.key[0]);
        btwrite(rrn, &page);
        if(strcmp(keySaver, key)){
            printf("Chave %s inserida com sucesso \n", key);
        }
        return(NO);
        
    }else{

        split(p_b_key, p_b_byteOffSet, p_b_rrn, &page, promo_key, promo_byteOffSet, promo_r_child, &newpage);
        printf("Divisao de no\n");
        printf("Chave %s promovida\n", promo_key);
        strcpy(keySaver, key);
        if(btwrite(rrn, &page)){
            printf("Chave %s inserida com sucesso\n", key);
        }
        btwrite(*promo_r_child, &newpage);
        
        
        // printf("repete no split\n\n");
        return(YES);
    }
}