/* btutil.c
Contains utility function for btree program
*/
#include "bt.h"
#include<stdio.h>
#include <string.h>
short create_root(char key[TAMANHO_STR], int byteOffSet, short left, short right){
    BTPAGE page;
    short rrn;
    rrn = getpage();
    pageinit(&page);
    strcpy(page.key[0], key);
    //printf("\nkey inicio %s\n", key);
    page.byteOffSet[0] = byteOffSet;
    page.child[0] = left;
    page.child[1] = right;
    page.keycount = 1;
    btwrite(rrn, &page);
    putroot(rrn);
    return(rrn);
}

void pageinit(BTPAGE *p_page){
    int j;
    for (j = 0; j < MAXKEYS; j++){
        strncpy(p_page->key[j], NOKEY,7);
        p_page->byteOffSet[j] = NIL;
        p_page->child[j] = NIL;
    }
    
    p_page->child[MAXKEYS] = NIL;
}

short search_node(char key[TAMANHO_STR], BTPAGE *p_page, short *pos){

    int i;

    for (i = 0; i < p_page->keycount && strcmp(key, p_page->key[i]) > 0; i++);
    *pos = i;

    if (*pos < p_page->keycount && strcmp (key, p_page->key[*pos]) == 0){
        return(YES);
    }else{
        return(NO);
    }
}

void ins_in_page(char key[TAMANHO_STR], int byteOffSet,short r_child, BTPAGE *p_page)
{
    int j;
    for(j = p_page-> keycount; strcmp(key, p_page->key[j-1]) < 0 && j > 0; j--){
        strcpy(p_page->key[j], p_page->key[j-1]);
        p_page->byteOffSet[j] = p_page->byteOffSet[j-1];
        p_page->child[j+1] = p_page->child[j];
    }
    p_page->keycount++;
    strcpy(p_page->key[j], key);
    //printf("\nkey: %s\n", key);
    //printf("\npagekey: %s\n", p_page->key[0]);
    p_page->byteOffSet[j] = byteOffSet;
    p_page->child[j+1] = r_child;
}

void split(char key[TAMANHO_STR], int byteOffSet, short r_child, BTPAGE *p_oldpage, char promo_key[TAMANHO_STR] , int *promo_byteOffSet, short *promo_r_child, BTPAGE *p_newpage){
    
    int j;
    short mid;
    char workkeys[MAXKEYS+1][TAMANHO_STR];
    int workbyteOffSet[MAXKEYS+1];
    short workchil[MAXKEYS+2];

    

    for (j = 0; j < MAXKEYS; j++){
        strcpy(workkeys[j], p_oldpage->key[j]);
        workbyteOffSet[j] = p_oldpage->byteOffSet[j];
        workchil[j] = p_oldpage->child[j];
    }

    workchil[j] = p_oldpage->child[j];



    for (j = MAXKEYS; strcmp(key, workkeys[j-1])<0 && j > 0; j--){
        strcpy(workkeys[j], workkeys[j-1]);
        workbyteOffSet[j] = workbyteOffSet[j-1];
        workchil[j+1] = workchil[j];
    }
    strcpy(workkeys[j], key);
    workbyteOffSet[j] = byteOffSet;
    workchil[j+1] = r_child;

    *promo_r_child = getpage();
    pageinit(p_newpage);

    for (j = 0; j < MINKEYS; j++){
        strcpy(p_oldpage->key[j],workkeys[j]);
        p_oldpage->byteOffSet[j]=workbyteOffSet[j];
        p_oldpage->child[j] = workchil[j];
        strcpy(p_newpage->key[j], workkeys[MINKEYS+2]);
        p_newpage->byteOffSet[j] = workbyteOffSet[MINKEYS+2];
        p_newpage->child[j] = workchil[MINKEYS+2];
        strcpy(p_oldpage->key[j+1], workkeys[MINKEYS]);
        p_oldpage->byteOffSet[j+1] = workbyteOffSet[MINKEYS];
        p_oldpage->child[j+1] = workchil[MINKEYS];
        strcpy(p_oldpage->key[j+MINKEYS+1], NOKEY);
        p_oldpage->byteOffSet[j+MINKEYS+1] = NIL;
        p_oldpage->child[j+MINKEYS+2]=NIL;
    }

    p_oldpage->child[MINKEYS+1] = workchil[MINKEYS+1];
    p_newpage->child[MINKEYS] = workchil[j+1+MINKEYS];
    p_newpage->keycount = MINKEYS;
    p_oldpage->keycount = MAXKEYS - MINKEYS;
    strcpy(promo_key, workkeys[MINKEYS+1]);
    *promo_byteOffSet = workbyteOffSet[MINKEYS+1];
    
   

}


