/* 
 * REPRESENTACAO DE GRAFOS - Versao 2020/2
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <string.h>

/* 
 * Estrutura de dados para representar grafos
 */
typedef struct a
{ /* Celula de uma lista de arestas */
  int nome;
  struct a *prox;
  int cost;
} Aresta;

typedef struct vert
{ /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele */
  int nome;
  int cont;
  Aresta *menor;
  Aresta *prim;
} Vertice;

// Estrutura para controlar as rotas (permutacoes das ordem dos locais)
typedef struct rota
{
  int total;
  Aresta *prim;
} Rota;

/*
 * Declaracoes das funcoes para manipulacao de grafos 
 */
void criaGrafo(Vertice **G, int ordem);
void destroiGrafo(Vertice **G, int ordem);
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2, int cost);
void imprimeGrafo(Vertice G[], int ordem);
void dijkstra(Vertice G[], int ordem, int origem, int *dist, int *vertice_anterior);
int menor_indice(int ordem, int* dist, int* visitados);
int fatorial(int fat);
void swap(int *a, int *b);
void permutar(int *arr, int start, int end);

int **permutacoes;
int permutacaoIdx = 0;

#define lugares 4

/*
 * Programinha simples para testar a representacao de grafo
 */
int main(int argc, char *argv[])
{
  Vertice *G;
  int ordemG = 38;

  int localesIndex[11] = {0, 2, 3, 5, 6, 7, 8, 11, 24, 31, 34};
  char localesNames[11][50] = {"Casa", "Natural da Terra", "Brasfor", "Igreja Betesda", "Losinox", "CM Comandos Lineares", "IMI Nogren", "Campo do Anhanguera", "Flores Martinho", "Catedral do Livro", "Comercial Nova Fatima"};
  Rota *rotas = (Rota*) malloc(sizeof(Rota) * fatorial(lugares));

  criaGrafo(&G, ordemG);
  acrescentaAresta(G, ordemG, 0, 1, 130);
  acrescentaAresta(G, ordemG, 0, 37, 50);
  acrescentaAresta(G, ordemG, 1, 2, 210);
  acrescentaAresta(G, ordemG, 2, 3, 75);
  acrescentaAresta(G, ordemG, 3, 4, 110);
  acrescentaAresta(G, ordemG, 4, 5, 40);
  acrescentaAresta(G, ordemG, 4, 9, 110);
  acrescentaAresta(G, ordemG, 5, 6, 210);
  acrescentaAresta(G, ordemG, 6, 7, 70);
  acrescentaAresta(G, ordemG, 7, 8, 80);
  acrescentaAresta(G, ordemG, 9, 10, 60);
  acrescentaAresta(G, ordemG, 9, 11, 25);
  acrescentaAresta(G, ordemG, 11, 12, 65);
  acrescentaAresta(G, ordemG, 12, 13, 100);
  acrescentaAresta(G, ordemG, 12, 35, 155);
  acrescentaAresta(G, ordemG, 13, 14, 40);
  acrescentaAresta(G, ordemG, 13, 15, 65);
  acrescentaAresta(G, ordemG, 13, 30, 70);
  acrescentaAresta(G, ordemG, 15, 16, 200);
  acrescentaAresta(G, ordemG, 15, 19, 35);
  acrescentaAresta(G, ordemG, 16, 17, 60);
  acrescentaAresta(G, ordemG, 17, 18, 25);
  acrescentaAresta(G, ordemG, 19, 20, 70);
  acrescentaAresta(G, ordemG, 20, 21, 60);
  acrescentaAresta(G, ordemG, 21, 22, 60);
  acrescentaAresta(G, ordemG, 22, 23, 55);
  acrescentaAresta(G, ordemG, 23, 24, 70);
  acrescentaAresta(G, ordemG, 24, 25, 30);
  acrescentaAresta(G, ordemG, 25, 26, 90);
  acrescentaAresta(G, ordemG, 25, 33, 90);
  acrescentaAresta(G, ordemG, 26, 27, 45);
  acrescentaAresta(G, ordemG, 26, 28, 70);
  acrescentaAresta(G, ordemG, 26, 33, 175);
  acrescentaAresta(G, ordemG, 28, 29, 35);
  acrescentaAresta(G, ordemG, 28, 30, 70);
  acrescentaAresta(G, ordemG, 30, 31, 135);
  acrescentaAresta(G, ordemG, 31, 32, 110);
  acrescentaAresta(G, ordemG, 32, 33, 65);
  acrescentaAresta(G, ordemG, 32, 34, 10);
  acrescentaAresta(G, ordemG, 34, 37, 80);
  acrescentaAresta(G, ordemG, 35, 36, 140);
  acrescentaAresta(G, ordemG, 35, 37, 85);

  // imprimeGrafo(G, ordemG);

  // ALTERAR DEFINE LUGARES NO TOPO DO ARQUIVO CASO FOR MUDAR A QUANTIDADE DE LUGARES
  // Lista dos indices (numeros inteiros) dos lugares a serem visitados
  int visitar[lugares];

  // Lugares a serem visitados >excluindo< a casa
  char labels[lugares][50] = {"Brasfor", "Natural da Terra", "Campo do Anhanguera", "Casa"};

  // Adicionando indices dos lugares na lista de visitas
  for (int i = 0; i < lugares; i++)
  {
    for (int j = 0; j < 11; j++)
    {
      if (!strcmp(labels[i], localesNames[j]))
      {
        visitar[i] = localesIndex[j];
        break;
      }
    }
  }

  // Alocando memoria para a matrix de permutacoes (quantidade de linhas: lugares! (fatorial) quantidade de colunas: lugares)
  permutacoes = (int **)malloc(fatorial(lugares) * sizeof(int *));
  for (int i = 0; i < fatorial(lugares); i++)
    permutacoes[i] = (int *)malloc(lugares * sizeof(int));

  // Preenchendo a matriz de permutacoes
  permutar(visitar, 0, lugares - 1);

  int *dist = (int*) malloc(sizeof(int) * ordemG);
  int *vertice_anterior = (int*) malloc(sizeof(int) * ordemG);

  int current_origin = permutacoes[0][0];

  for (int i=0; i < lugares; i++) {
    dijkstra(G, ordemG, visitar[i], dist, vertice_anterior);

    for (int j=0; j < fatorial(lugares); j++) {
      if (permutacoes[j][0] != visitar[i]) {
        continue;
      }

      //! percorrer dist para ver se o caminho reverso é possivel e se passa por todos os locais
    }
  }

  dijkstra(G, ordemG, 0, dist, vertice_anterior);
  // printf("\n IMPRIMINDO GRAFICO \n");
  // imprimeGrafo(G, ordemG);
  
  destroiGrafo(&G, ordemG);
  // system("PAUSE");
  return 0;
}

/*
 * Implementacao do algoritmo Dijkstra
 */
void dijkstra(Vertice G[], int ordem, int origem, int *dist, int *vertice_anterior)
{
  Vertice vAux;
  Aresta *aAux;
  
  int *visitados = (int*) malloc(sizeof(int) * ordem);
  
  for (int i=0; i < ordem; i++) {
    dist[i] = INT_MAX;
    vertice_anterior[i] = INT_MAX;
    visitados[i] = 0;
  }

  dist[origem] = 0;
  vertice_anterior[origem] = origem;

  for (int count=0; count < ordem; count++) {
    int menor = menor_indice(ordem, dist, visitados); //vai retornar a origem como primeiro ponto
    visitados[menor] = 1;

    aAux = G[menor].prim;

    while(aAux != NULL) {
      if (dist[menor] + aAux->cost <= dist[aAux->nome] && !visitados[aAux->nome]) {
        dist[aAux->nome] = dist[menor] + aAux->cost;
        vertice_anterior[aAux->nome] = menor;
      }

      aAux = aAux->prox;
    }
  }
}

int menor_indice(int ordem, int *dist, int *visitados) {
  int min = INT_MAX;
  int min_index;
 
    for (int i = 0; i < ordem; i++)
        if (visitados[i] == 0 && dist[i] <= min) {
            min = dist[i];
            min_index = i;
        }
 
    return min_index;
}

/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicilamente, sem nenhuma aresta 
 */
void criaGrafo(Vertice **G, int ordem)
{
  int i;
  *G = (Vertice *)malloc(sizeof(Vertice) * ordem); /* Alcacao dinamica de um vetor de vertices */

  for (i = 0; i < ordem; i++)
  {
    (*G)[i].nome = i;
    (*G)[i].prim = NULL; /* Cada vertice sem nenua aresta incidente */
  }
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vertice **G, int ordem)
{
  int i;
  Aresta *a, *n;

  for (i = 0; i < ordem; i++)
  { /* Remove lista de adjacencia de cada vertice */
    a = (*G)[i].prim;
    while (a != NULL)
    {
      n = a->prox;
      free(a);
      a = n;
    }
  }
  free(*G);
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j
 *   serao criadas, na estrutura de dados, arestas (i,j) e (j,i)
 */
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2, int cost)
{
  Aresta *A1, *A2;

  if (v1 < 0 || v1 >= ordem) /* Testo se vertices sao validos */
    return 0;
  if (v2 < 0 || v2 >= ordem)
    return 0;

  /* Acrescento aresta na lista do vertice v1 */
  A1 = (Aresta *)malloc(sizeof(Aresta));
  A1->nome = v2;
  A1->cost = cost;
  A1->prox = G[v1].prim;
  G[v1].prim = A1;

  /* Acrescento aresta na lista do vertice v2 se v2 != v1 */
  if (v1 == v2)
    return 1;

  A2 = (Aresta *)malloc(sizeof(Aresta));
  A2->nome = v1;
  A2->cost = cost;
  A2->prox = G[v2].prim;
  G[v2].prim = A2;

  return 1;
}

/*  
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vertice G[], int ordem)
{
  int i;
  Aresta *aux;

  printf("\nOrdem:   %d", ordem);
  printf("\nLista de Adjacencia:\n");

  for (i = 0; i < ordem; i++)
  {
    printf("\n    V%3d: ", i);
    aux = G[i].prim;
    for (; aux != NULL; aux = aux->prox)
      printf("%3d (%3d) ", aux->nome, aux->cost);
  }
  printf("\n\n");
}

int fatorial(int fat)
{
  if (fat == 1)
    return fat;

  return fat * fatorial(fat - 1);
}

// Trocar dois valores de posição
void swap(int *a, int *b)
{
  int temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

// Permutar lista
void permutar(int *arr, int start, int end)
{
  if (start == end)
  {
    for (int i=0; i < lugares; i++)
    {
      permutacoes[permutacaoIdx][i] = arr[i];
    }
    permutacaoIdx++;
    return;
  }
  int i;
  for (i = start; i <= end; i++)
  {
    swap((arr + i), (arr + start));
    permutar(arr, start + 1, end);
    swap((arr + i), (arr + start));
  }
}