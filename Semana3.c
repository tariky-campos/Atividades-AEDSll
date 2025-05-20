#include <stdio.h>
#include <stdlib.h>

typedef struct TipoNO *Apontador;

typedef struct TipoNO
{
    int valor;
    Apontador Esq, Dir;
} TipoNO;

void Antecessor(Apontador q, Apontador *r)
{
    if ((*r)->Dir != NULL)
    {
        Antecessor(q, &(*r)->Dir);
        return;
    }
    q->valor = (*r)->valor;
    q = *r;
    *r = (*r)->Esq;
    free(q);
}

void Retira(int x, Apontador *p)
{
    Apontador Aux;
    if (*p == NULL)
    {
        printf("Erro : Registro nao esta na arvore\n");
        return;
    }
    if (x < (*p)->valor)
    {
        Retira(x, &(*p)->Esq);
        return;
    }
    if (x > (*p)->valor)
    {
        Retira(x, &(*p)->Dir);
        return;
    }

    if ((*p)->Dir == NULL)
    {
        Aux = *p;
        *p = (*p)->Esq;
        free(Aux);
        return;
    }
    if ((*p)->Esq != NULL)
    {
        Antecessor(*p, &(*p)->Esq);
        return;
    }
    Aux = *p;
    *p = (*p)->Dir;
    free(Aux);
}
void imprimir_ordem(Apontador *no)
{
    if (*no != NULL)
    {
        imprimir_ordem(&(*no)->Esq);
        printf("%d ", (*no)->valor);
        imprimir_ordem(&(*no)->Dir);
    }
}
int EncontrarMinimo(Apontador no)
{
    while (no->Esq != NULL)
    {
        no = no->Esq;
    }
    return no->valor;
}

void RemoverOrdenado(Apontador *no)
{
    while (*no != NULL)
    {
        int menor = EncontrarMinimo(*no);
        printf("%d ", menor);
        Retira(menor, no);
    }
}

void Inserir(Apontador *no, int valor)
{
    if (*no == NULL)
    {
        *no = (Apontador)malloc(sizeof(TipoNO));
        (*no)->valor = valor;
        (*no)->Dir = NULL;
        (*no)->Esq = NULL;
        return;
    }
    if (valor > (*no)->valor)
        Inserir(&((*no)->Dir), valor);
    else
        Inserir(&((*no)->Esq), valor);
}

int main()
{
    TipoNO *no = NULL;
    int N, num;

    scanf("%d", &N);

    for (int i = 0; i < N; i++)
    {
        scanf("%d", &num);
        Inserir(&no, num);
    }
    RemoverOrdenado(&no);
    return 0;
}
