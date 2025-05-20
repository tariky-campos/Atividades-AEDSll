#include <stdio.h>
#include <stdlib.h>

#define max 50
typedef struct TipoNo* Apontador;
typedef struct TipoNo
{
    char letra;
    unsigned fim: 1;
    Apontador Esq, Meio, Dir;
}TipoNo;

Apontador inicializaNo(char letra){
    TipoNo *no = (Apontador)malloc(sizeof(TipoNo));
    no->letra = letra;
    no->fim = 0;
    no->Dir = no->Esq = no->Meio = NULL;
    return no;
}

void insert(Apontador *no, char *palavra){
    if(!(*no)) *no = inicializaNo(*palavra);

    if((*palavra) < (*no)->letra) insert(&((*no)->Esq), palavra);
    else if ((*palavra) > (*no)->letra) insert(&((*no)->Dir), palavra);
    else{
        if(*(palavra +1)) insert(&((*no)->Meio),palavra+1);
        else (*no)->fim = 1;
    }

}

void traverseTSTUtil(Apontador no, char *buffer, int depth){
    if(no){
        traverseTSTUtil(no->Esq, buffer, depth);

        buffer[depth] = no->letra;
        if(no->fim){
            buffer[depth + 1] = '\0';
            printf("%s\n",buffer);
        }

        traverseTSTUtil(no->Meio, buffer, depth +1);
        traverseTSTUtil(no->Dir, buffer, depth);
    }
}

void traverseTST(Apontador no){
    char buffer[max];
    traverseTSTUtil(no, buffer, 0);
}

int searchTST(Apontador no, char* palavra){
    if(!no) return 0;

    if(*palavra < (no)->letra) return searchTST(no->Esq, palavra);

    else if(*palavra > (no)->letra) return searchTST(no->Dir, palavra);
    
    else{
        if(*(palavra +1)=='\0') return no->fim;
        return searchTST(no->Meio, palavra + 1);
    }
}

int main(){
    int N;
    char nome[max];
    
    
    scanf("%d",&N);
    while(N != 0){
        TipoNo *no = NULL;
        for(int i = 0; i < N; i++){
            scanf("%s",nome);
            insert(&no,nome);
        }
 
        scanf("%s",nome);
        int x = searchTST(no,nome);
 
        if(x == 1){
            printf("Pessoa Encontrada\n");
        }else printf("Pessoa Não Encontrada\n");
        
        scanf("%d",&N);
    }

}