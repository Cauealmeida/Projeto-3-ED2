/* driver.c
Driver for btree tests
Open or creates b-tree file.
Get next key and calls insert to insert key in tree.
If necessary creates new root.
*/
#include <stdio.h>
#include "bt.h"
#include <string.h>


int main()
{
    int quantidadeRecuperadaBusca;
    int quantidadeFinalBusca;
    int remove;
    int quantidadeCadastros;
    short found_rrn;
    int quantidadeFinal;
    short found_pos;
    int quantidadeBusca;
    int validacao = 0;
    short header = -1;
    int quantidadeRecuperada;
    int promoted; // boolean: tells if a promotion from below
    short root;  // rrn of root page
    short promo_rrn; // rrn promoted from below
    char promo_key[TAMANHO_STR]; // key promoted from below
    int promo_byteOffSet;
    char key[TAMANHO_STR];// next key to insert in tree
    int loop = 0; 
    int inicio = 0;
    int contagem = 0;
    short resultadoBusca;
    int contagemRecuperada = 0;
    FILE *entradaInsercao, *entradaBusca;
    dados insere[tamanhoArquivoProfessora];
    dados busca[tamanhoArquivoBuscaProfessora];
    char copy[tamanhoArquivoProfessora][qdteCaracteres];
    char keyConcat[tamanhoArquivoProfessora][qdteCaracteres];
    char keySearch[tamanhoArquivoBuscaProfessora][qdteCaracteres];
    int byteOffSet[MAXKEYS];
    
    printf("Deseja remover algum arquivo? \n");
    printf(" 1 - remover arvore \n 2 - remover arquivo de contagem \n 3 - remover arquivo contagem busca\n 4 - remover todos os arquivos \n");
    scanf("%d", &remove);

    switch (remove){
    case 1:
        delete();
        deleteArmazena();
        break;
    case 2:
        deleteContagem();
        break;   
    case 3:
        deleteContagemBusca();
        break;  
     case 4:
        delete();
        deleteContagem();
        deleteArmazena();
        deleteContagemBusca();
        break;        
    default:
        break;
    }

    //carregar dados insere.bin em um vetor de struct
    insereDados(entradaInsercao, insere, keyConcat);
    //funções para armazenar contagem e escrever os dados.
    printf("\nQuantos cadastros serao armazenados?\n");
    scanf(" %d", &quantidadeCadastros);
    printf("\n");
    quantidadeRecuperada = recuperaCont ();
    quantidadeFinal = quantidadeRecuperada + quantidadeCadastros;
    armazenaCont (quantidadeFinal);
    for(int i = 0; i < tamanhoArquivoProfessora; i++){
        strcpy(copy[i],keyConcat[i]);
        //printf("%d\n", i);
        //printf("copy %s\n\n", copy[i]);
    }
    escreveDados(insere, byteOffSet, quantidadeRecuperada, quantidadeFinal);
    if (btopen()){
        root = getroot();
    }else{
        
        root = create_tree(copy[quantidadeRecuperada], byteOffSet[quantidadeRecuperada]);
        validacao = 1;

    }

    for(int i = quantidadeRecuperada+validacao; i < quantidadeFinal; i++){
        //printf("copy : %s\n", copy[i]);
        promoted = insert(root,  copy[i], byteOffSet[i], &promo_rrn, promo_key, &promo_byteOffSet);
        if (promoted)
        root = create_root(promo_key, promo_byteOffSet, root, promo_rrn);
    }

    btclose();
    
    printf("\nOs dados sao:\n");
    print(root);
    printf("\n\n");
   
    insereDadosBusca(entradaBusca, busca, keySearch);
    printf("Quantas buscas serao relizadas?\n");
    scanf(" %d", &quantidadeBusca);
    printf("\n");
    quantidadeRecuperadaBusca = recuperaContBusca ();
    quantidadeFinalBusca = quantidadeRecuperadaBusca + quantidadeBusca;
    
    armazenaContBusca (quantidadeFinalBusca);
    
    
    for(int i = quantidadeRecuperadaBusca; i < quantidadeFinalBusca; i++){
       
        resultadoBusca = search(root, keySearch[i], found_rrn , found_pos);
        if(resultadoBusca == -1){
            printf("Chave nao encontrada");
        }
        printf("\n\n");
        
    }

   
}