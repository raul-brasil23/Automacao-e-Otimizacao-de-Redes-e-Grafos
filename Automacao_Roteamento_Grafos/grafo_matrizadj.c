#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

#define MAXNUMVERTICES 100
#define ARESTA_NULA -1
#define VERTICE_INVALIDO -1

typedef float Peso;
typedef int ApontadorVerticeAdjacente;

// Estrutura específica da Matriz
typedef struct {
    Peso mat[MAXNUMVERTICES][MAXNUMVERTICES];
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



bool inicializaGrafo (Grafo* g, int numVertices) {
    if ((numVertices > MAXNUMVERTICES) || (numVertices <= 0)) {
        fprintf (stderr, "ERRO: Numero de vertices invalido!");
        return false;
    }

    g->numVertices = numVertices;

    int i, j;
    
    for (i = 0; i < numVertices; i++) {
        for (j = 0; j < numVertices; j++) {
            g->mat[i][j] = ARESTA_NULA;
        }
    }
    
    return true;
}

int obtemNrVertices(Grafo* g) {
    verificaValidadeGrafo (g);
    return g->numVertices;
}

void verificaValidadeGrafo (Grafo* g) {
    if (!g) {
        fprintf (stderr, "ERRO: Grafo invalido!");
        exit (-1);
    }
}

void verificaValidadeVertice (int vertice, Grafo* g) {
    if ((vertice >= g->numVertices) || (vertice < 0)) {
        fprintf (stderr, "ERRO: Vertice invalido!");
        exit (-1);
    }
}

Peso obtemPesoAresta (int v1, int v2, Grafo* g) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v1, g);
    verificaValidadeVertice (v2, g);

    return g->mat[v1][v2];
}

void insereAresta (int v1, int v2, Peso p, Grafo* g) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v1, g);
    verificaValidadeVertice (v2, g);

    g->mat[v1][v2] = p;
}

bool existeAresta (int v1, int v2, Grafo* g) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v1, g);
    verificaValidadeVertice (v2, g);
    return (g->mat[v1][v2] != ARESTA_NULA);
}

bool removeAresta (int v1, int v2, Peso* p, Grafo* g) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v1, g);
    verificaValidadeVertice (v2, g);

    if (!existeAresta (v1, v2, g)) {
        fprintf (stderr, "ERRO: Nao existe aresta!");
        return false;
    }

    (*p) = g->mat[v1][v2];
    g->mat[v1][v2] = ARESTA_NULA;

    return true;
}

bool listaAdjVazia (int v, Grafo* g) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v, g);

    for (int j = 0; j < g -> numVertices; j++) {
        if (g->mat[v][j] != ARESTA_NULA) return false; 
    }

    return true;
}

ApontadorVerticeAdjacente proxListaAdj (int v, Grafo* g, ApontadorVerticeAdjacente atual) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v, g);
    if (atual < -1 || atual >= g->numVertices) {
        fprintf (stderr, "ERRO: Vertice invalido!");
        exit (-1);
    }

    for (int j = atual + 1; j < g -> numVertices; j++) {
        if (g->mat[v][j] != ARESTA_NULA) return j;
    }

    return VERTICE_INVALIDO;
}

ApontadorVerticeAdjacente primeiroListaAdj (int v, Grafo* g) {
    verificaValidadeGrafo (g);
    verificaValidadeVertice (v, g);

    ApontadorVerticeAdjacente primeiro = proxListaAdj (v, g, -1);
    
    return primeiro;
}

void imprimeGrafo (Grafo* g) {
    int i, j;
    for (i = 0; i < g->numVertices; i++) {
        for (j = 0; j < g->numVertices; j++) {
            printf ("%.1f\t", g->mat[i][j]);
        }
        printf ("\n");
    }
}

// Não precisa fazer nada para Matriz de Adjacência
void liberaGrafo (Grafo* g) {}

int verticeDestino (ApontadorVerticeAdjacente p, Grafo* g) {
    return (int) p;
}