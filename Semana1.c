#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct{
    int E, indice;
    char nome[100];
} Item;

int busca_binaria(Item *arr, int tamanho, int alvo) {
    int inicio = 0, fim = tamanho - 1, meio;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2; 

        if (arr[meio].E == alvo) {
            return arr[meio].indice; 
        }
        else if (arr[meio].E < alvo) {
            inicio = meio + 1; 
        }
        else {
            fim = meio - 1; 
        }
    }
    
    return -1; 
}

void selectionSort(Item *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].E < arr[min_idx].E) {
                min_idx = j;
            }
        }
        Item temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}

int main() {
  

    int N, M;
    char linha[100], *ptr;
    
    scanf("%d\n", &N);
    
    Item lista[N], copia[100];

    for (int i = 0; i < N; i++) {
        fgets(linha, 100, stdin);
        lista[i].indice = i + 1;
        
        ptr = strtok(linha, " ");
        lista[i].E = atoi(ptr);
        ptr = strtok(NULL, "\n");
        strncpy(lista[i].nome, ptr, 100 - 1);
        copia[i] = lista[i];
    }
    
    scanf("%d", &M);

    selectionSort(lista, N);

    int x = busca_binaria(lista, N, M);
    printf("Posição: %d, Livro: %s\n", x, copia[x - 1].nome);
}
