#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define VAZIO "!!!!!!!!!!"
#define RETIRADO "**********"
#define M  5
#define N  11

typedef unsigned int TipoApontador;
typedef char TipoChave[N];
typedef unsigned TipoPesos[N];

typedef struct TipoItem {
  TipoChave Chave;
} TipoItem;

typedef unsigned int TipoIndice;
typedef TipoItem TipoDicionario[M];

TipoDicionario Tabela;
TipoPesos p;
TipoItem Elemento;

char carro1[5][N];
char carro2[5][N];
int idx1 = 0, idx2 = 0;

void GeraPesos(TipoPesos p) {
  struct timeval semente;
  gettimeofday(&semente,NULL);
  srand((int)(semente.tv_sec + 1000000 * semente.tv_usec));
  for (int i = 0; i < N; i++)
    p[i] = 1 + (int)(10000.0 * rand() / (RAND_MAX + 1.0));
}

TipoIndice h(TipoChave Chave, TipoPesos p) {
  unsigned int Soma = 0;
  for (int i = 0; i < strlen(Chave); i++)
    Soma += (unsigned int)Chave[i] * p[i];
  return (Soma % M);
}

void Inicializa(TipoDicionario T) {
  for (int i = 0; i < M; i++)
    memcpy(T[i].Chave, VAZIO, N);
}

TipoApontador Pesquisa(TipoChave Ch, TipoPesos p, TipoDicionario T) {
  unsigned int i = 0;
  unsigned int Inicial = h(Ch, p);
  while (strcmp(T[(Inicial + i) % M].Chave, VAZIO) != 0 &&
         strcmp(T[(Inicial + i) % M].Chave, Ch) != 0 && i < M)
    i++;
  if (strcmp(T[(Inicial + i) % M].Chave, Ch) == 0)
    return ((Inicial + i) % M);
  else return M;
}

void Insere(TipoItem x, TipoPesos p, TipoDicionario T) {
  unsigned int i = 0;
  unsigned int Inicial = h(x.Chave, p);

  if (Pesquisa(x.Chave, p, T) < M) return;

  while (strcmp(T[(Inicial + i) % M].Chave, VAZIO) != 0 &&
         strcmp(T[(Inicial + i) % M].Chave, RETIRADO) != 0 && i < M)
    i++;

  if (i < M)
    strcpy(T[(Inicial + i) % M].Chave, x.Chave);
}

void LerLinha(char *p, int Tam) {
  fgets(p, Tam, stdin);
  p[strcspn(p, "\n")] = 0;
}

int JaNoCarro(char nome[N]) {
  for (int i = 0; i < idx1; i++)
    if (strcmp(carro1[i], nome) == 0)
      return 1;
  for (int i = 0; i < idx2; i++)
    if (strcmp(carro2[i], nome) == 0)
      return 1;
  return 0;
}

void MostrarCarros() {
  printf("- Primeiro Carro: ");
  for (int i = 0; i < idx1; i++) printf("%s - ", carro1[i]);
  printf("\n");

  printf("- Segundo Carro: ");
  for (int i = 0; i < idx2; i++) printf("%s - ", carro2[i]);
  printf("\n");
}

int main() {
  srand(time(NULL));
  Inicializa(Tabela);
  GeraPesos(p);

  int n;
  scanf("%d", &n);
  getchar();

  for (int i = 0; i < n; i++) {
    LerLinha(Elemento.Chave, N);

    if (JaNoCarro(Elemento.Chave)) continue;

    Insere(Elemento, p, Tabela);

    // Alternar entre os carros: par no carro 2, ímpar no carro 1
    if (i % 2 == 0 && idx2 < 5)
      strcpy(carro2[idx2++], Elemento.Chave);
    else if (idx1 < 5)
      strcpy(carro1[idx1++], Elemento.Chave);
  }

  MostrarCarros();

  char nome[N];
  while (1) {
    LerLinha(nome, N);
    if (strcmp(nome, "0") == 0) break;

    if (Pesquisa(nome, p, Tabela) < M)
      printf("Pessoa Confirmada\n");
    else
      printf("Pessoa Não Confirmada\n");
  }

  return 0;
}
