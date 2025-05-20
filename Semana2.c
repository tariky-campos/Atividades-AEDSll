#include <stdio.h>
#include <stdlib.h>

typedef struct TipoNO *Apontador;

typedef struct TipoNO{
    int valor;
    Apontador Esq, Dir;
}TipoNO;

// testando sem a funcao inizializar
void Inserir(Apontador *no, int valor){
    if(*no == NULL){
        *no = (Apontador)malloc(sizeof(TipoNO));
        (*no)->valor = valor;
        (*no)->Dir = NULL;
        (*no)->Esq = NULL;
        return;
    }
    else{
        if(valor > (*no)->valor){
            Inserir(&((*no)->Dir), valor);
            return;
        }
        else{
            Inserir(&((*no)->Esq), valor);
            return;
            
        }
    }
}

void Imprimir(Apontador *no){
    if(*no != NULL){
        Imprimir(&(*no)->Esq);
        printf("%d ",(*no)->valor);
        Imprimir(&(*no)->Dir);
    }

}

void PreOrdem(Apontador *no){
    if(*no != NULL){
        printf("%d ",(*no)->valor);
        PreOrdem(&(*no)->Esq);
        PreOrdem(&(*no)->Dir);
    }

}

void PosOrdem(Apontador *no){
    if(*no != NULL){
        PosOrdem(&(*no)->Esq);
        PosOrdem(&(*no)->Dir);
        printf("%d ",(*no)->valor);
        
    }

}

int main(){
    TipoNO *no = NULL;
    int N,num;
    scanf("%d",&N);
    
    for(int i=0;i<N;i++){
        scanf("%d",&num);
        Inserir(&no, num);
    }
    

    printf("Pré-Ordem: ");
    PreOrdem(&no);
    printf("\nEm Ordem: ");
    Imprimir(&no);
    printf("\nPós-Ordem: ");
    PosOrdem(&no);


}