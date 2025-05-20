#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    Vertical, Horizontal
} TipoInclinacao;

typedef struct No *Apontador;

typedef struct TipoRegistro {
    int chave;
    char nome[100];
} TipoRegistro;


typedef struct No {
    TipoRegistro reg;
    Apontador esq, dir;
    TipoInclinacao BitE, BitD;
} No;


#define FALSE 0
#define TRUE 1


void EE(Apontador *Ap) {
    Apontador Ap1 = (*Ap)->esq;
    (*Ap)->esq = Ap1->dir;
    Ap1->dir = *Ap;
    Ap1->BitE = Vertical;
    (*Ap)->BitE = Vertical;
    *Ap = Ap1;
}

void ED(Apontador *Ap) {
    Apontador Ap1 = (*Ap)->esq;
    Apontador Ap2 = Ap1->dir;
    Ap1->BitD = Vertical;
    (*Ap)->BitE = Vertical;
    Ap1->dir = Ap2->esq;
    Ap2->esq = Ap1;
    (*Ap)->esq = Ap2->dir;
    Ap2->dir = *Ap;
    *Ap = Ap2;
}

void DD(Apontador *Ap) {
    Apontador Ap1 = (*Ap)->dir;
    (*Ap)->dir = Ap1->esq;
    Ap1->esq = *Ap;
    Ap1->BitD = Vertical;
    (*Ap)->BitD = Vertical;
    *Ap = Ap1;
}

void DE(Apontador *Ap) {
    Apontador Ap1 = (*Ap)->dir;
    Apontador Ap2 = Ap1->esq;
    Ap1->BitD = Vertical;
    (*Ap)->BitD = Vertical;
    Ap1->esq = Ap2->dir;
    Ap2->dir = Ap1;
    (*Ap)->dir = Ap2->esq;
    Ap2->esq = *Ap;
    *Ap = Ap2;
}


void IInsere(TipoRegistro x, Apontador *no, TipoInclinacao *IAp, short *Fim) {
    if (*no == NULL) {
        *no = (Apontador)malloc(sizeof(No));
        (*no)->reg = x;
        (*no)->esq = NULL;
        (*no)->dir = NULL;
        (*no)->BitE = Vertical;
        (*no)->BitD = Vertical;
        *IAp = Horizontal;
        *Fim = FALSE;
        return;
    }
    
    if (x.chave < (*no)->reg.chave) { 
        IInsere(x, &(*no)->esq, &(*no)->BitE, Fim);
        if (*Fim) return;
        if ((*no)->BitE != Horizontal) { *Fim = TRUE; return; }
        if ((*no)->esq->BitE == Horizontal) {
            EE(no);
            *IAp = Horizontal;
            return;
        }
        if ((*no)->esq->BitD == Horizontal) {
            ED(no);
            *IAp = Horizontal;
        }
        return;
    }
    if (x.chave > (*no)->reg.chave) { 
        IInsere(x, &(*no)->dir, &(*no)->BitD, Fim);
        if (*Fim) return;
        if ((*no)->BitD != Horizontal) { *Fim = TRUE; return; }
        if ((*no)->dir->BitD == Horizontal) {
            DD(no);
            *IAp = Horizontal;
            return;
        }
        if ((*no)->dir->BitE == Horizontal) {
            DE(no);
            *IAp = Horizontal;
        }
        return;
    }
    
    printf("Erro: Chave ja esta na arvore\n");
    *Fim = TRUE;
}

void Insere(TipoRegistro x, Apontador *raiz) {
    short Fim;
    TipoInclinacao IAp;
    IInsere(x, raiz, &IAp, &Fim);
}

void EmOrdem(Apontador no,int n) {
    if (no != NULL) {
        static int primeiro =1;
        EmOrdem(no->esq,n);
        if(primeiro){
            printf("%s",no->reg.nome);
            primeiro = 0;
        }
        else{
            printf(" - %s",no->reg.nome);
        }
        EmOrdem(no->dir,n);
    }
}

int main() {
    Apontador raiz = NULL;
    int N,n;
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        TipoRegistro reg;
        
        scanf("%s %d", reg.nome, &reg.chave);
        Insere(reg, &raiz);
        if(i == N-1){
            n = reg.chave; 
        }
    }
    EmOrdem(raiz,n);
    printf("\n");

    return 0;
}
