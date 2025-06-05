#include <stdlib.h>
#include <stdio.h>
#define M 2
#define MM (M * 2)
#define FALSE 0
#define TRUE  1

typedef long TipoChave;

typedef struct TipoRegistro {
  TipoChave Chave;
} TipoRegistro;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
  short n;
  TipoRegistro r[MM];
  TipoApontador p[MM + 1];
} TipoPagina;

// Funções da árvore B
void Inicializa(TipoApontador *Dicionario) { *Dicionario = NULL; }

// Insere registro dentro de uma página da árvore B
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir) {
  // Move os elementos maiores para a direita até encontrar a posição correta
  short NaoAchouPosicao;
  int k = Ap->n;
  NaoAchouPosicao = (k > 0);
  while (NaoAchouPosicao) {
    if (Reg.Chave >= Ap->r[k - 1].Chave) {
      NaoAchouPosicao = FALSE;
      break;
    }
    Ap->r[k] = Ap->r[k - 1];
    Ap->p[k + 1] = Ap->p[k];
    k--;
    if (k < 1) NaoAchouPosicao = FALSE;
  }
  Ap->r[k] = Reg;
  Ap->p[k + 1] = ApDir;
  Ap->n++;
}

// Função principal de inserção recursiva
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu,
         TipoRegistro *RegRetorno, TipoApontador *ApRetorno) {
  long i = 1, j;
  TipoApontador ApTemp;
  if (Ap == NULL) {
    // Caso base: cria nova raiz se a árvore estiver vazia
    *Cresceu = TRUE;
    *RegRetorno = Reg;
    *ApRetorno = NULL;
    return;
  }

  // Busca pela posição correta na página
  while (i < Ap->n && Reg.Chave > Ap->r[i - 1].Chave) i++;

  if (Reg.Chave == Ap->r[i - 1].Chave) {
    printf("Erro: Registro ja esta presente\n");
    *Cresceu = FALSE;
    return;
  }

  if (Reg.Chave < Ap->r[i - 1].Chave) i--;

  // Chamada recursiva para subárvore apropriada
  Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

  if (!*Cresceu) return;

  if (Ap->n < MM) {
    // Se há espaço na página, insere normalmente
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    *Cresceu = FALSE;
    return;
  }

  // === SPLIT: página cheia, precisa dividir ===
  ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
  ApTemp->n = 0;
  ApTemp->p[0] = NULL;

  // Verifica onde o novo registro se encaixa e divide corretamente
  if (i < M + 1) {
    // Insere último elemento na nova página temporária
    InsereNaPagina(ApTemp, Ap->r[MM - 1], Ap->p[MM]);
    Ap->n--;
    InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
  } else {
    InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
  }

  // Move a metade superior da página original para a nova página
  for (j = M + 2; j <= MM; j++)
    InsereNaPagina(ApTemp, Ap->r[j - 1], Ap->p[j]);

  // A página atual fica com a metade inferior
  Ap->n = M;
  ApTemp->p[0] = Ap->p[M + 1];

  // Valor do meio se torna a nova raiz da divisão
  *RegRetorno = Ap->r[M];
  *ApRetorno = ApTemp;
}

// Cria uma nova raiz se necessário (após split)
void Insere(TipoRegistro Reg, TipoApontador *Ap) {
  short Cresceu;
  TipoRegistro RegRetorno;
  TipoPagina *ApRetorno, *ApTemp;
  Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
  if (Cresceu) {
    // Nova raiz criada com valor do meio
    ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
    ApTemp->n = 1;
    ApTemp->r[0] = RegRetorno;
    ApTemp->p[1] = ApRetorno;
    ApTemp->p[0] = *Ap;
    *Ap = ApTemp;
  }
}

void ImprimeI(TipoApontador p, int nivel) {
  if (p == NULL) return;
  printf("Nivel %d : ", nivel);
  for (int i = 0; i < p->n; i++)
    printf("%ld ", p->r[i].Chave);
  printf("\n");
  for (int i = 0; i <= p->n; i++)
    ImprimeI(p->p[i], nivel + 1);
}

void Imprime(TipoApontador p) { ImprimeI(p, 0); }

void TestaI(TipoApontador p, int pai, short direita) {
  if (p == NULL) return;
  if (p->r[0].Chave > pai && !direita) {
    printf("Erro: filho %ld maior que pai %d\n", p->r[0].Chave, pai);
    return;
  }
  int antecessor = 0;
  for (int i = 0; i < p->n; i++) {
    if (p->r[i].Chave <= antecessor) {
      printf("Erro: irmaos fora de ordem: %ld <= %d\n", p->r[i].Chave, antecessor);
      return;
    }
    antecessor = p->r[i].Chave;
  }
  for (int i = 0; i < p->n; i++)
    TestaI(p->p[i], p->r[i].Chave, FALSE);
  TestaI(p->p[p->n], p->r[p->n - 1].Chave, TRUE);  
}

void Testa(TipoApontador p) {
  if (p == NULL) return;
  for (int i = 0; i < p->n; i++)
    TestaI(p->p[i], p->r[i].Chave, FALSE);
  TestaI(p->p[p->n], p->r[p->n - 1].Chave, TRUE);
}



// Função principal
int main() {
  TipoRegistro x;
  TipoPagina *D;
  Inicializa(&D);

  printf("=== INSERCAO DE VALORES NA ARVORE B ===\n");
  printf("Digite valores inteiros para inserir na arvore (0 para encerrar):\n");

  while (1) {
    printf("Chave: ");
    if (scanf("%ld", &x.Chave) != 1) {
      printf("Entrada invalida. Digite apenas inteiros.\n");
      while (getchar() != '\n'); // Limpa buffer
      continue;
    }
    if (x.Chave == 0) break;
    Insere(x, &D);
    printf("\nArvore B apos inserir %ld:\n", x.Chave);
    Imprime(D);
  }

  Testa(D);
  return 0;
}
