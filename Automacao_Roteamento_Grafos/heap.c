#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

typedef float Peso;

typedef struct {
    int vertice;
    Peso chave;
} Item;

typedef struct {
    Item* vetor;
    int* posicao;
    int maxElem;
    int elementos;
} Heap;

/*
===============================
    INTERFACE MODULARIZADA 
===============================
*/

// Funções Bases de Heap
void inicializaHeap (Heap* h, int numElementos);
void insereNoHeap(Heap* h, int vertice, Peso chave);
int extraiMinimo(Heap* h);
bool pertenceAoHeap(Heap* h, int vertice);
void diminuiChave(Heap* h, int vertice, Peso novaChave);
// Funções de Verificação
void verificaValidadeHeap (Heap* h);
bool verificaHeapVazio (Heap* h);
// Funções para Encontrar elementos no Heap
int encontraPai (int i);
int encontraFilhoEsquerda (int i);
int encontraFilhoDireita (int i);
// Funções auxiliares do Heap
void troca(Heap* h, int i, int j);
void sobeNoHeap(Heap* h, int i);
void desceNoHeap(Heap* h, int i);

void verificaValidadeHeap (Heap* h) {
    if (!h) {
        fprintf (stderr, "ERRO: Heap inexistente!");
        exit (-1);
    }
}

bool verificaHeapVazio (Heap* h) {
    if (h->elementos == 0) return true;
    else return false;
}

void inicializaHeap (Heap* h, int numElementos) {
    verificaValidadeHeap(h);

    if (numElementos <= 0) {
        fprintf (stderr, "ERRO: Numero invalido de elementos!");
        exit(-1);
    }

    h->maxElem = numElementos;
    h->elementos = 0;

    h->vetor = (Item*) calloc(numElementos+1, sizeof(Item)); // Pra manter as contas de Pai, FilhoEsq e FilhoDir corretas
    h->posicao = (int*) calloc(numElementos, sizeof(int));

    if((!h->vetor) || (!h->posicao)) {
        fprintf (stderr, "ERRO: Falha de alocacao de memoria na funcao inicializaHeap!");
        exit(-1);
    }

}

int encontraPai (int i) {
    return i/2;
}

int encontraFilhoEsquerda (int i) {
    return i*2;
}

int encontraFilhoDireita (int i) {
    return (i*2) + 1;
}

void troca(Heap* h, int i, int j) {
    Item temp = h->vetor[i];
    h->vetor[i] = h->vetor[j];
    h->vetor[j] = temp;

    h->posicao[h->vetor[i].vertice] = i;
    h->posicao[h->vetor[j].vertice] = j;
}

void sobeNoHeap(Heap* h, int i) {

    while (i > 1) { // Enquanto não for raiz
        int pai = encontraPai(i);

        if (h->vetor[i].chave < h->vetor[pai].chave) {
            troca(h, i, pai);
            i = pai;
        }
        else break;
    }
}

void desceNoHeap(Heap* h, int i) {
    int filhoEsq = encontraFilhoEsquerda(i);
    int filhoDir = encontraFilhoDireita(i);

    int menor = i; // Raiz

    if (filhoEsq <= h->elementos && h->vetor[filhoEsq].chave < h->vetor[menor].chave) menor = filhoEsq;

    if (filhoDir <= h->elementos && h->vetor[filhoDir].chave < h->vetor[menor].chave) menor = filhoDir;

    if (menor != i) {
        troca (h, i, menor);
        desceNoHeap (h, menor);
    }

}

void insereNoHeap(Heap* h, int vertice, Peso chave) {
    if (h->elementos >= h-> maxElem) {
        fprintf (stderr, "ERRO: Heap cheio!");
        exit(-1);
    }

    h->elementos++;

    h->vetor[h->elementos].vertice = vertice;
    h->vetor[h->elementos].chave = chave;
    h->posicao[vertice] = h->elementos;

    sobeNoHeap (h, h->elementos);
}

int extraiMinimo(Heap* h) {
    if(verificaHeapVazio(h)) {
        fprintf (stderr, "ERRO: Heap vazio!");
        exit (-1);
    }

    int menorVert = h->vetor[1].vertice; // Guardar quem tá sendo extraído
    h->posicao[menorVert] = -1; // Foi extraído do Heap

    Item ultimo = h->vetor[h->elementos];
    h->elementos--;

    // Só substitua caso sobre elementos
    if (h->elementos > 0) {
        h->vetor[1] = ultimo;
        h->posicao[ultimo.vertice] = 1;
        desceNoHeap (h, 1);
    }

    return menorVert;
}

bool pertenceAoHeap(Heap* h, int vertice) {
    // Não entrou ou já saiu
    if (h->posicao[vertice] == -1 || h->posicao[vertice] == 0) return false;

    return true;
}

void diminuiChave(Heap* h, int vertice, Peso novaChave) {
    if(!pertenceAoHeap(h, vertice)) return;

    int indice = h->posicao[vertice];

    h->vetor[indice].chave = novaChave;
    sobeNoHeap (h, indice);
}