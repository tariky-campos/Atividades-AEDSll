#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    Vertical, Horizontal
} TipoInclinacao;
typedef struct No* Apontador;

typedef struct TipoRegistro {
    int Chave;
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
        (*no)->esq = (*no)->dir = NULL;
        (*no)->BitE = (*no)->BitD = Vertical;
        *IAp = Horizontal;
        *Fim = FALSE;
        return;
    }

    if (x.Chave < (*no)->reg.Chave) {
        IInsere(x, &(*no)->esq, &(*no)->BitE, Fim);
        if (*Fim) return;
        if ((*no)->BitE != Horizontal) { *Fim = TRUE; return; }
        if ((*no)->esq->BitE == Horizontal) { EE(no); *IAp = Horizontal; return; }
        if ((*no)->esq->BitD == Horizontal) { ED(no); *IAp = Horizontal; return; }
        return;
    }
    if (x.Chave > (*no)->reg.Chave) {
        IInsere(x, &(*no)->dir, &(*no)->BitD, Fim);
        if (*Fim) return;
        if ((*no)->BitD != Horizontal) { *Fim = TRUE; return; }
        if ((*no)->dir->BitD == Horizontal) { DD(no); *IAp = Horizontal; return; }
        if ((*no)->dir->BitE == Horizontal) { DE(no); *IAp = Horizontal; return; }
        return;
    }

    printf("Erro: Altura (chave) ja esta na arvore\n");
    *Fim = TRUE;
}

void Insere(TipoRegistro x, Apontador *raiz) {
    short Fim;
    TipoInclinacao IAp;
    IInsere(x, raiz, &IAp, &Fim);
}

void EmOrdem(Apontador no) {
    if (no != NULL) {
        static int primeiro = 1;
        EmOrdem(no->esq);  
        if (primeiro) {
            printf("%s", no->reg.nome);
            primeiro = 0;
        } else {
            printf(" - %s", no->reg.nome);
        }
        EmOrdem(no->dir);  
    }
}


void DirCurto(Apontador *Ap, short *Fim) {
    if ((*Ap)->BitD == Horizontal) {
        (*Ap)->BitD = Vertical;
        *Fim = TRUE;
        return;
    }
    Apontador Ap1 = (*Ap)->esq;
    if (Ap1->BitE == Vertical && Ap1->BitD == Vertical) {
        (*Ap)->BitE = Horizontal;
        *Fim = FALSE;
        return;
    }
    if (Ap1->BitD == Horizontal) { ED(Ap); *Fim = TRUE; }
    else if (Ap1->BitE == Horizontal) { EE(Ap); *Fim = TRUE; }
}

void Antecessor(Apontador q, Apontador *Ap, short *Fim) {
    if ((*Ap)->dir != NULL) {
        Antecessor(q, &(*Ap)->dir, Fim);
        if (!*Fim) DirCurto(Ap, Fim);
        return;
    }
    q->reg = (*Ap)->reg;
    Apontador temp = *Ap;
    *Ap = (*Ap)->esq;
    free(temp);
    *Fim = TRUE;
}

void EsqCurto(Apontador *Ap, short *Fim) {
    if ((*Ap)->BitE == Horizontal) {
        (*Ap)->BitE = Vertical;
        *Fim = TRUE;
        return;
    }
    Apontador Ap1 = (*Ap)->dir;
    if (Ap1->BitE == Vertical && Ap1->BitD == Vertical) {
        (*Ap)->BitD = Horizontal;
        *Fim = FALSE;
        return;
    }
    if (Ap1->BitE == Horizontal) { DE(Ap); *Fim = TRUE; }
    else if (Ap1->BitD == Horizontal) { DD(Ap); *Fim = TRUE; }
}



void IRetira(TipoRegistro x, Apontador *Ap, short *Fim) {
    if (*Ap == NULL) {
        printf("Erro: Altura nao esta na arvore\n");
        *Fim = TRUE;
        return;
    }

    if (x.Chave < (*Ap)->reg.Chave) {
        IRetira(x, &(*Ap)->esq, Fim);
        if (!*Fim) EsqCurto(Ap, Fim);
        return;
    }
    if (x.Chave > (*Ap)->reg.Chave) {
        IRetira(x, &(*Ap)->dir, Fim);
        if (!*Fim) DirCurto(Ap, Fim);
        return;
    }

    Apontador temp = *Ap;
    if ((*Ap)->esq == NULL) {
        *Ap = (*Ap)->dir;
        free(temp);
        *Fim = TRUE;
        return;
    }
    if ((*Ap)->dir == NULL) {
        *Ap = (*Ap)->esq;
        free(temp);
        *Fim = TRUE;
        return;
    }

    Antecessor(*Ap, &(*Ap)->esq, Fim);
    if (!*Fim) EsqCurto(Ap, Fim);
}


void Retira(TipoRegistro x, Apontador *Ap) {
    short Fim;
    IRetira(x, Ap, &Fim);
}

int BuscarChavePorNome(Apontador no, const char* nome) {
    if (no == NULL) return -1;
    int cmp = strcmp(nome, no->reg.nome);
    if (cmp == 0) return no->reg.Chave;

    int chaveEsq = BuscarChavePorNome(no->esq, nome);
    if (chaveEsq != -1) return chaveEsq;

    return BuscarChavePorNome(no->dir, nome);
}


int main() {
    Apontador raiz = NULL;
    int N;
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        TipoRegistro reg;
        scanf("%s %d", reg.nome, &reg.Chave);
        Insere(reg, &raiz);
    }

    int M;
    scanf("%d", &M);

    for (int i = 0; i < M; i++) {
        char nome[100];
        scanf("%s", nome);
        int chave = BuscarChavePorNome(raiz, nome);
        if (chave != -1) {
            TipoRegistro reg;
            reg.Chave = chave;
            strcpy(reg.nome, nome);
            Retira(reg, &raiz);
        } else {
            printf("Erro: nome '%s' nao encontrado na arvore\n", nome);
        }
    }
    

    EmOrdem(raiz);
    printf("\n");

    return 0;
}
