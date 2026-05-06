#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

#define ARESTA_NULA -1
#define VERTICE_INVALIDO NULL

typedef float Peso;

// Estruturas específicas da Lista
typedef struct str_Aresta {
    int vdest;
    Peso peso;
    struct str_Aresta* prox;
} Aresta;

typedef Aresta* ApontadorVerticeAdjacente;

typedef struct {
    ApontadorVerticeAdjacente* listaAdj;
    int numVertices;
} Grafo;

// Utilizado para DFS
typedef enum {BRANCO, CINZA, PRETO} Cor;

/*
===============================
    INTERFACE MODULARIZADA 
===============================
*/

// Funções Bases de Grafos
bool inicializaGrafo(Grafo* g, int numVertices);
int obtemNrVertices (Grafo* g);
void insereAresta (int v1, int v2, Peso p, Grafo* g);
bool existeAresta (int v1, int v2, Grafo* g);
bool removeAresta (int v1, int v2, Peso* p, Grafo* g);
Peso obtemPesoAresta (int v1, int v2, Grafo* g);
bool listaAdjVazia (int v, Grafo* g);
ApontadorVerticeAdjacente primeiroListaAdj (int v, Grafo* g);
ApontadorVerticeAdjacente proxListaAdj (int v, Grafo* g, ApontadorVerticeAdjacente atual);
int verticeDestino (ApontadorVerticeAdjacente p, Grafo* g);
void imprimeGrafo (Grafo* g);
void liberaGrafo (Grafo* g);
// Funções de Verificação
void verificaValidadeGrafo (Grafo* g);
void verificaValidadeVertice (int vertice, Grafo* g); // verticeValido



void verificaValidadeGrafo (Grafo* g) {
    if (!g) {
        fprintf (stderr, "ERRO: Grafo inexistente!");
        exit (-1);
    }
}

void verificaValidadeVertice (int vertice, Grafo* g) {
    verificaValidadeGrafo (g);

    if (vertice >= g->numVertices) { // 100 Vértices vão do Índice 0 a 99
        fprintf (stderr, "ERRO: Limite de vertices foi excedido!");
        exit (-1);
    }

    if (vertice < 0) { // O Vértice 0 é o Primeiro Vértice
        fprintf (stderr, "ERRO: Numero do vertice (%d) deve ser positivo.\n", vertice);
        exit (-1);
    }
}

bool inicializaGrafo (Grafo* g, int numVertices) {
    verificaValidadeGrafo (g);

    if (numVertices <= 0) {
        fprintf (stderr, "ERRO: Numero invalido de vertices!");
        return false;
    }

    g->numVertices = numVertices;

    g->listaAdj = (ApontadorVerticeAdjacente*) calloc(numVertices, sizeof(ApontadorVerticeAdjacente));

    if(!g->listaAdj) {
        fprintf (stderr, "ERRO: Falha de alocacao de memoria na funcao inicializaGrafo!");
        return false;
    }
    return true;
}

int obtemNrVertices (Grafo* grafo) {
    verificaValidadeGrafo (grafo);
    return grafo->numVertices;
}

bool listaAdjVazia (int v, Grafo* g) {
    verificaValidadeVertice (v, g);

    if (!(g->listaAdj[v])) return true;
    else return false;
    // return (!(g->listaAdj[v])); // Se for vazia vai retornar true, else false
}

ApontadorVerticeAdjacente proxListaAdj (int v, Grafo* g, ApontadorVerticeAdjacente atual) {
    verificaValidadeVertice (v, g);

    if (!atual) {
        fprintf (stderr, "ERRO: Atual eh nulo!");
        return VERTICE_INVALIDO;
    }

    return atual->prox;
}

ApontadorVerticeAdjacente primeiroListaAdj (int v, Grafo* g) {
    verificaValidadeVertice (v, g);

    return g->listaAdj[v];
}

Peso obtemPesoAresta (int v1, int v2, Grafo* g) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v1, g);
    verificaValidadeVertice (v2, g);

    ApontadorVerticeAdjacente p = g->listaAdj[v1];

    while (p && (p->vdest != v2))
        p = p->prox;

    if (!p) return ARESTA_NULA; // Aresta não existe
    else return p -> peso;
}

bool existeAresta (int v1, int v2, Grafo* g) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v1, g);
    verificaValidadeVertice (v2, g);

    ApontadorVerticeAdjacente p;

    p = g->listaAdj[v1];

    while ((p) && (p->vdest != v2)) 
        p = p -> prox;
    
    if (p) return true;
    return false;
}

void insereAresta (int v1, int v2, Peso p, Grafo* g) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v1, g);
    verificaValidadeVertice (v2, g);

    Aresta* aresta = (Aresta*) calloc(1, sizeof(Aresta));
    if (!aresta) {
        fprintf (stderr, "ERRO: Erro na alocacao de memoria da aresta!\n");
        exit(-1);
    }

    aresta->vdest = v2;
    aresta->peso = p;

    // Se a Lista de Adjacentes de v1 estiver vazia, posso anexar a aresta aí (1ª aresta)
    if (!g->listaAdj[v1]) g->listaAdj[v1] = aresta;
    else {
        Aresta* atual = g->listaAdj[v1];

        // While até chegar na última aresta
        while (atual->prox) atual = atual->prox;

        atual->prox = aresta;
    }

    aresta->prox = NULL;
}

bool removeAresta (int v1, int v2, Peso* p, Grafo* g) { // Peso como referência para pegar o peso da aresta antes de excluí-la
    verificaValidadeVertice (v1, g);
    verificaValidadeVertice (v2, g);
    // Já verificam a validade do Grafo

    ApontadorVerticeAdjacente atual;
    ApontadorVerticeAdjacente anterior;

    atual = g->listaAdj[v1];
    anterior = NULL;

    while (atual && (atual->vdest != v2)) {
        anterior = atual;
        atual = atual->prox;
    }

    if (!atual) return false;

    if (!anterior)
        g->listaAdj[v1] = atual->prox;
    else anterior->prox = atual->prox;

    (*p) = atual -> peso;
    atual->prox = NULL;

    free (atual);
    atual = NULL;

    return true;
}

void liberaGrafo (Grafo* g) {
    verificaValidadeGrafo (g);

    ApontadorVerticeAdjacente atual;

    for (int v = 0; v < g->numVertices; v++) {
        atual = g->listaAdj[v];

        while (atual) {
            g->listaAdj[v] = atual->prox;

            atual->prox = NULL;
            free(atual);

            atual = g->listaAdj[v];
        }
    }

    free (g->listaAdj);
    g->listaAdj = NULL;
    g->numVertices = 0;
}

void imprimeGrafo (Grafo* g) {
    verificaValidadeGrafo (g);

    ApontadorVerticeAdjacente atual;

    for (int v = 0; v < g->numVertices; v++) {
        atual = g->listaAdj[v];

        printf ("Vertice %d -> ", v);
        while (atual) {
            printf ("(%d,%d)-Peso %.1f -> ", v, atual->vdest, atual->peso);

            atual = atual->prox;
        }
        printf ("NULL\n");
    }
}

int verticeDestino (ApontadorVerticeAdjacente p, Grafo* g) {
    if (p) return p->vdest;
    else {
        fprintf (stderr, "ERRO: Ponteiro para vertice destino invalido!\n");
        exit(-1);
    }
}