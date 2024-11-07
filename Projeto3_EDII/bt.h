/* bt.h
    header file for btree programs
*/
#include <string.h>
#include <stdio.h>
#define MAXKEYS 3
#define MINKEYS 1
#define NIL (-1)
#define NOKEY "@@@@@@"
#define NO 0
#define YES 1
#define tamanhoArquivoProfessora 11
#define tamanhoArquivoBuscaProfessora 3
#define qdteCaracteres 7
#define TAMANHO_STR 7

typedef struct{
    char id_aluno[4];
    char sigla_disc[4];
    char nome_aluno[50];
    char nome_disc[50];
    float media;
    float freq;
}dados;

#pragma pack(push, 1)
typedef struct {
    short keycount; // number of keys in page
    char key[MAXKEYS][TAMANHO_STR]; // the actual keys
    int byteOffSet[MAXKEYS];
    short child[MAXKEYS+1]; // ptrs to rrns of descendants
} BTPAGE;

#pragma pack(pop)


//#define PAGESIZE sizeof(btpage)
extern short root; // rrn of root page
//extern int btfd; // file descriptor of btree file
extern int infd; // file descriptor of input file

/* prototypes */
void btclose ();
void delete();
short btopen ();
short btread (short rrn, BTPAGE *page_ptr);
short btwrite (short rrn, BTPAGE *page_ptr);
short create_root (char key[TAMANHO_STR], int byteOffSet, short left, short right);
short create_tree(char keyConcat[tamanhoArquivoProfessora], int byteOffSet);
short getpage ();
short getroot ();
short insert (short rrn, char key[TAMANHO_STR], int byteOffSet, short *promo_r_child, char promo_key[TAMANHO_STR], int *promo_byteOffSet);
void ins_in_page (char key[TAMANHO_STR], int byteOffSet,short r_child, BTPAGE *p_page);
void pageinit (BTPAGE *p_page);
void putroot(short root);
short search_node (char key[TAMANHO_STR], BTPAGE *p_page, short *pos);
void split(char key[TAMANHO_STR], int byteOffSet, short r_child, BTPAGE *p_oldpage, char promo_key[TAMANHO_STR], int *promo_byteOffSet, short *promo_r_child, BTPAGE *p_newpage);

int insereDados (FILE *entrada, dados insere[tamanhoArquivoProfessora], char keyConcat[tamanhoArquivoBuscaProfessora][qdteCaracteres]);
int insereDadosBusca (FILE *entrada, dados busca[tamanhoArquivoBuscaProfessora], char keySearch[tamanhoArquivoBuscaProfessora][qdteCaracteres]);
int armazenaCont (int cont);
int recuperaCont ();
int escreveArquivo(dados L[20], FILE *arq, int inicio, int contagem);
void escreveDados(dados L[20], int byteOffSet[MAXKEYS], int inicio, int contagem);
void escreveDadosBusca(dados L[20], int inicio, int contagem);
void deleteContagem();
void deleteContagemBusca();
void deleteArmazena();
int armazenaContBusca (int cont);
int recuperaContBusca ();
short search(short rrn, char key[TAMANHO_STR], short found_rrn, short found_pos);
short print(short rrn);
short searchArq(int byteOffSet);