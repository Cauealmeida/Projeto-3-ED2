#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "bt.h"

void deleteContagem(){
   
    if(remove("armazena_contagem.bin")==0){
        printf ("Arquivo de contagem removido com sucesso\n");
    }
    else{
        printf("Erro, arquivo de contagem nao foi removido\n");
    }
}
void deleteArmazena(){
   
    if(remove("Armazena.bin")==0){
        printf ("Arquivo de contagem removido com sucesso\n");
    }
    else{
        printf("Erro, arquivo de contagem nao foi removido\n");
    }
}
void deleteContagemBusca(){
   
    if(remove("armazena_contagembusca.bin")==0){
        printf ("Arquivo de contagem-busca removido com sucesso\n");
    }
    else{
        printf("Erro, arquivo de contagem-busca nao foi removido\n");
    }
}

int insereDados (FILE *entrada, dados insere[tamanhoArquivoProfessora], char keyConcat[tamanhoArquivoBuscaProfessora][qdteCaracteres]){
	//função para puxar as informações do arquivo da professora
	entrada = fopen("insere.bin", "r+b");
	fread(insere, sizeof(dados), tamanhoArquivoProfessora, entrada);    
	printf("\nregistros do arquivo carregados na memoria:\n");
   	for (int i = 0; i<tamanhoArquivoProfessora; i++) {
		   
        printf("%s %s %s %s %.2f %.2f\n", insere[i].id_aluno, insere[i].sigla_disc, insere[i].nome_aluno, insere[i].nome_disc, insere[i].media, insere[i].freq);

		//Concatenação
        strcpy(keyConcat[i], insere[i].id_aluno);  
        strcat(keyConcat[i], insere[i].sigla_disc); 
		keyConcat[i][qdteCaracteres] = '\0';
		
	}    
	fclose(entrada);
	return 1;
}

int insereDadosBusca (FILE *entrada, dados busca[tamanhoArquivoBuscaProfessora], char keySearch[tamanhoArquivoBuscaProfessora][qdteCaracteres] ){
	//função para puxar as informações do arquivo de busca da professora
	entrada = fopen("busca.bin", "r+b");
	printf("\nAs keys da busca sao:\n");

   	for (int i = 0; i<tamanhoArquivoBuscaProfessora; i++) {
		fread(busca[i].id_aluno, sizeof(char), 4, entrada);
        busca[i].id_aluno[3] = '\0';  // Garantir terminador nulo

        fread(busca[i].sigla_disc, sizeof(char), 4, entrada);
        busca[i].sigla_disc[3] = '\0'; 

        // Concatena id_aluno e sigla_disc
        strcpy(keySearch[i], busca[i].id_aluno);  
        strcat(keySearch[i], busca[i].sigla_disc); 	
		printf("%s\n", keySearch[i]);
    }         
	fclose(entrada);
	return 1;
}

void escreveDados(dados L[20], int byteOffSet[20], int inicio, int contagem){
	char buffer[115];
	int tamanhoString = 0;
	FILE *armazena = fopen("Armazena.bin", "r+b");

	if(armazena == NULL){
		armazena = fopen("Armazena.bin", "wb");
	}
	fseek(armazena, 0, SEEK_END);
	
	
	for(inicio; inicio < contagem; inicio++){
		byteOffSet[inicio] = ftell(armazena);
		sprintf(buffer,"%s#%s#%s#%s#%.2f#%.2f#",L[inicio].id_aluno, L[inicio].sigla_disc, L[inicio].nome_aluno, L[inicio].nome_disc, L[inicio].media, L[inicio].freq);
		tamanhoString = strlen(buffer);
		//printf("%d", tamanhoString);
		//printf("Buffer: '%s'\n", buffer);
		fwrite(&tamanhoString, sizeof(int), 1, armazena);
		fwrite(buffer, strlen(buffer), 1, armazena);	
	}
	fclose(armazena);	
}



int armazenaCont (int quantidadeCadastros){
	//função para armazenar a contagem
	FILE *armazenaContagem;
	armazenaContagem = fopen("armazena_contagem.bin", "w+b");
    fwrite(&quantidadeCadastros, sizeof(int), 1, armazenaContagem);
	fclose(armazenaContagem);
	return 1;
}

int armazenaContBusca (int quantidadeCadastros){
	//função para armazenar a contagem
	FILE *armazenaContagem;
	armazenaContagem = fopen("armazena_contagembusca.bin", "w+b");
    fwrite(&quantidadeCadastros, sizeof(int), 1, armazenaContagem);
	fclose(armazenaContagem);
	return 1;
}

int recuperaCont (){
	//função para puxar para a memoria as informações do arquivo que armazenou a contagem
	int cont = 0;
	FILE *armazenaContagem;
	armazenaContagem = fopen("armazena_contagem.bin", "rb");
	fread(&cont, sizeof(cont), 1, armazenaContagem);
	fclose(armazenaContagem);
	return cont;
}



int recuperaContBusca (){
	//função para puxar para a memoria as informações do arquivo que armazenou a contagem
	int cont = 0;
	FILE *armazenaContagem;
	armazenaContagem = fopen("armazena_contagembusca.bin", "rb");
	fread(&cont, sizeof(cont), 1, armazenaContagem);
	fclose(armazenaContagem);
	return cont;
	
}

