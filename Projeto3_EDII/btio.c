/* btio.c
Contains btree functions that directly involve file I/O:
*/
#include <stdio.h>
#include "bt.h"
#include <string.h>
#define PAGESIZE sizeof(BTPAGE)
//#include "fileio.h"
FILE *btfd; // global file descriptor for "btree.dat"



void delete(){
   
    if(remove("btree.bin")==0){
        printf ("Arquivo removido com sucesso\n");
    }
    else{
        printf("Erro, arquivo nao foi removido\n");
    }
}
short btopen()
{   
    if (btfd = fopen("btree.bin", "r+b")){
        return 1;
    }
   
}

void btclose()
{
    fclose(btfd);
}

short getroot(){
    short root;
    //long lseek();
    fseek(btfd, 0, 0);

    if (fread(&root, sizeof(short), 1, btfd) == 0)
    {
        printf("%hd\n", root);
        printf("Error: Unable to get root. \007\n");
        return(0);
    }
    return (root);
}

void putroot(short root){
    fseek(btfd, 0, 0);
    fwrite(&root, sizeof(short), 1,btfd);
}

short create_tree(char keyConcat[tamanhoArquivoProfessora], int byteOffSet){
    int define = PAGESIZE;
    short header = -1;
    btfd = fopen("btree.bin", "wb"); 
    fwrite(&header, sizeof(short), 1, btfd);
    fclose (btfd);
    btopen();
    printf("Chave %s inserida com sucesso \n", keyConcat);

    return (create_root(keyConcat, byteOffSet, NIL, NIL));
}

short getpage(){ //!checar qdo dá primeira pagina 
    long addr;
    fseek(btfd,0, SEEK_END);
    addr = ftell(btfd) - 2;

    if(addr < 0){
        addr = 0;
    }
    
    return ((short) addr / PAGESIZE);
}

short btread (short rrn, BTPAGE *page_ptr){ 
    long addr;
    addr = (long)rrn * (long)PAGESIZE + 2;
    fseek(btfd, addr, 0);
    return(fread(page_ptr, PAGESIZE, 1, btfd));
}

short btwrite(short rrn, BTPAGE *page_ptr){
    long addr;
    addr = (long)rrn * (long)PAGESIZE +2;
    fseek(btfd, addr, 0);
    return(fwrite(page_ptr, PAGESIZE, 1, btfd));  
}
short searchArq(int byteOffSet){
    FILE *armazena = fopen("Armazena.bin", "r+b");
    char buffer[115];
    char *registro;
    int tamanhoRegistro;
    fseek(armazena, byteOffSet, 0);
    fread(&tamanhoRegistro, sizeof(int),1,armazena);
    fread(buffer, sizeof(char),tamanhoRegistro,armazena);
    buffer[tamanhoRegistro]= '\0';
    fclose(armazena);
    registro = strtok(buffer, "#");
    while(registro != NULL){
        printf("%s ", registro);
        registro = strtok(NULL, "#");

    }
    
    
}

short search(short rrn, char key[TAMANHO_STR], short found_rrn, short found_pos){
    BTPAGE page;
    short pos;
    short proxPos = 5;
    short found = 0;
    if (rrn == NIL){
        return NIL;
    }
    else{
        btopen();
        btread(rrn, &page);
        btclose();
        for(int i = 0; i < MAXKEYS; i++){  
            //printf("pagekey %c \n ", page.key[i]);
            if(strcmp(key, page.key[i])==0){
                found = 1;
                pos = i;
            }
            
        }

        if(found){
            found_rrn = rrn;
            found_pos = pos;
            printf("O RRN da busca foi %hd , a posicao na pagina foi %hd\n", found_rrn, found_pos);
            printf("chave localizada foi: %s\n", page.key[pos]);
            printf("O registro localizado foi: ");
            searchArq(page.byteOffSet[pos]);
            return found;
        }
        else{
            for(int j = 0; j < page.keycount; j++){
                if (strcmp(key, page.key[j]) > 0 && strcmp(key, page.key[j+1]) < 0){
                    proxPos = j+1;
                    
                }
                if(strcmp(key, page.key[0]) < 0){
                    proxPos = 0;
                   
                }
                if(strcmp(key, page.key[page.keycount-1]) > 0){
                    proxPos = page.keycount;
                 
                }
            }
            return (search(page.child[proxPos], key, found_rrn, found_pos));
        }
    }
}

short print(short rrn){
    BTPAGE page;
    int i =0;
    if (rrn == NIL)
    {
        return NIL;
    }

    else{
        btopen();
        btread(rrn, &page);
        btclose();

        for(int i = 0; i < page.keycount+1; i++){
            print(page.child[i]);
            if(strcmp(page.key[i], NOKEY) != 0 && i < MAXKEYS){
                //printf("Chave: %s ", page.key[i]);
                printf("Registro: ");
                searchArq(page.byteOffSet[i]);
                printf("\n");
            }
            }
            if(i == page.keycount){
                return 1;
            }    
        }
        
}