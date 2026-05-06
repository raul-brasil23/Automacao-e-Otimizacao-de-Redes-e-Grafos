#ifdef MATRIZ
#include "grafo_matrizadj.c"
#else
#include "grafo_listaadj.c"
#endif

#include "heap.c"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>

int encontraMaisDistante(int* ant, float* chaveMinPeso, int numVertices);
void AGMPrim (Grafo* g, int verticeRaiz, int* ant, float* chaveMinPeso);
void visitaBPContaComponentes (Grafo* g, int u, Cor cor[]);
int contaComponentes(Grafo* g);
bool verifica_validade_vertice (int vertice, Grafo* g);
bool le_grafo(char* arquivo_entrada, FILE* arquivo_saida, Grafo *grafo);

// Função para encontrar o Vértice mais Distante -> Maior Peso
int encontraMaisDistante(int* ant, float* chaveMinPeso, int numVertices) {
    int verticeMaisDistante = 0;
    float maiorDistancia = 0;

    // O Vértice Mais Distante é com o Maior Peso
    for (int v = 1; v < numVertices; v++) {
        float distanciaAtual = 0;
        int atual = v;

        // Soma todos os Pesos do Vértice até a Origem
        while (ant[atual] != -1) {
            distanciaAtual += chaveMinPeso[atual];
            atual = ant[atual];
        }

        // Pega o Vértice mais Distante
        if (distanciaAtual > maiorDistancia) {
            maiorDistancia = distanciaAtual;
            verticeMaisDistante = v;
        } 
        else if (distanciaAtual == maiorDistancia) { // Caso de Desempate -> Pegar o Vértice Menor
            if (v < verticeMaisDistante) {
                verticeMaisDistante = v;
            }
        }
    }

    return verticeMaisDistante;
}

// Árvore Geradora Mínima (AGM) - Algoritmo de Prim
void AGMPrim (Grafo* g, int verticeRaiz, int* ant, float* chaveMinPeso) {
    Heap* h = (Heap*) malloc(sizeof(Heap));
    inicializaHeap (h, obtemNrVertices(g));

    int numVertices = obtemNrVertices(g);

    // Inicializa todos os Vértices com Peso Máximo, sem anterior e Insere no Heap
    for (int v = 0; v < numVertices; v++) {
        chaveMinPeso[v] = FLT_MAX;
        ant[v] = -1;
        insereNoHeap (h, v, FLT_MAX);
    }

    chaveMinPeso[verticeRaiz] = 0.0;
    // Diminui chave do Vértice Raiz
    diminuiChave (h, verticeRaiz, 0);

    while (!verificaHeapVazio(h)) {
        int u = extraiMinimo(h);
        
        ApontadorVerticeAdjacente atual = primeiroListaAdj(u, g);

        while (atual != VERTICE_INVALIDO) {
            int v = verticeDestino (atual, g);

            // Verificar o Peso Mínimo da Aresta (u,v)
            if ((pertenceAoHeap(h, v)) && (obtemPesoAresta(u, v, g) < chaveMinPeso[v])) {
                ant[v] = u;
                chaveMinPeso[v] = obtemPesoAresta(u, v, g);
                diminuiChave(h, v, obtemPesoAresta(u, v, g));
            }

            atual = proxListaAdj (u, g, atual);
        }
    }

    free(h);
    free(h->vetor);
    free(h->posicao);
}

// Função auxiliar para contar os Componentes Conexos de um Grafo
void visitaBPContaComponentes (Grafo* g, int u, Cor cor[]) {
    cor[u] = CINZA;

    ApontadorVerticeAdjacente atual = primeiroListaAdj(u, g);

    while (atual != VERTICE_INVALIDO) {
        int v = verticeDestino(atual, g);

        // Utiliza recursão para alcançar todos os Vértices do Grafo a partir desse Vértice v
        if (cor[v] == BRANCO) {
            visitaBPContaComponentes (g, v, cor);
        }

        atual = proxListaAdj (u, g, atual);
    }

    cor[u] = PRETO;
}

// Retorna o Número Total de Componentes Conexos do Grafo
int contaComponentes(Grafo* g) {
    verificaValidadeGrafo(g);

    Cor* cor = (Cor*) malloc(sizeof(Cor)*obtemNrVertices(g));

    int contador = 0;
    int numVertices = obtemNrVertices(g);

    for (int v = 0; v < numVertices; v++) cor[v] = BRANCO;

    for (int v = 0; v < numVertices; v++) {
        // Se for Conexo, só entra no if uma vez, pois os outros vértices já foramalcançados pelo primeiro
        if (cor[v] == BRANCO) {
            contador++;
            visitaBPContaComponentes (g, v, cor);
        }
    }

    free (cor);

    return contador;
}

bool verifica_validade_vertice (int vertice, Grafo* g) {
    verificaValidadeGrafo (g);

    int numVertices = obtemNrVertices(g);

    if (vertice >= numVertices) // 100 Vértices vão do Índice 0 a 99
        return false;

    if (vertice < 0) // O Vértice 0 é o Primeiro Vértice
        return false;

    return true;
}

bool le_grafo(char* arquivo_entrada, FILE* arquivo_saida, Grafo *grafo) {
    FILE* fp;
    int nVertices, nArestas;
    int v1, v2;
    Peso peso;

    fp = fopen (arquivo_entrada, "r");
    if (!fp) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo %s\n", arquivo_entrada);
        return false;
    }

    if (fscanf (fp, "%d %d", &nVertices, &nArestas) != 2) {
        fprintf (stderr, "Nao foi possivel ler o numero de vertices e de arestas\n");
        fclose(fp);
        return false;
    }

    inicializaGrafo (grafo, nVertices);

    // Variável para validar o arquivo de entrada
    bool erroGrafo = false;

    for (int i = 0; i < nArestas; i++) {
        fscanf (fp, "%d %d %f", &v1, &v2, &peso);

        // Variável para saber se devo inserir aresta no grafo ou não
        bool erroAresta = false;

        // Teste de Vértice Inválido
        bool validadeV1 = verifica_validade_vertice(v1, grafo);
        bool validadeV2 = verifica_validade_vertice(v2, grafo);

        if (!validadeV1) {
            fprintf (arquivo_saida, "ERRO: VERTICE INVALIDO (%d)\n", v1);
            erroAresta = true;
            erroGrafo = true;
        }

        if (!validadeV2) {
            fprintf (arquivo_saida, "ERRO: VERTICE INVALIDO (%d)\n", v2);
            erroAresta = true;
            erroGrafo = true;
        }
        
        // Só tem Auto-Laço e/ou Arestas Paralelas as Arestas com Vértices Válidos
        if (validadeV1 && validadeV2) {
            // Teste de Auto-Laço
            if (v1 == v2) {
                fprintf (arquivo_saida, "ERRO: AUTO-LACO (%d,%d)\n", v1, v2);
                erroAresta = true;
                erroGrafo = true;
            }

            // Teste de Aresta Paralela
            if (existeAresta(v1, v2, grafo) || existeAresta(v2, v1, grafo)) {
                int menor = v1;
                int maior = v2;

                if (v1 > v2) {
                    menor = v2;
                    maior = v1;
                }

                fprintf (arquivo_saida, "ERRO: ARESTA PARALELA (%d,%d)\n", menor, maior);
                erroAresta = true;
                erroGrafo = true;
            }
        }

        // Teste do Peso inválido
        if (peso <= 0) {
            fprintf (arquivo_saida, "ERRO: PESO INVALIDO (%.1f)\n", peso);
            erroGrafo = true;
        }

        // Evitar erros de inserção de arestas
        if (!erroAresta) {
            insereAresta (v1, v2, peso, grafo);
            insereAresta (v2, v1, peso, grafo);
        }

    }

    fclose(fp);

    if (erroGrafo) return false;
    else return true;
}

int main(int argc, char *argv[]) {
    Grafo grafo;

    if (argc != 3) { // Verificação de argumentos
        fprintf (stderr, "ERRO: O programa requer 3 argumentos!\n");
        return 1;
    }

    // Criação do Arquivo de Saída
    FILE* arquivo_saida = fopen(argv[2], "w");
    if (!arquivo_saida) {
        fprintf (stderr, "ERRO: Falha ao criar o arquivo de saida!\n");
        return 1;
    }

    // Recebe o retorno da função le_grafo para verificar se o programa deve prosseguir ou não
    bool grafoValido = le_grafo(argv[1], arquivo_saida, &grafo);
    if (!grafoValido) {
        fclose(arquivo_saida);
        return 1;
    }

    // Verificar se o Grafo é Conectado
    int componentesConexos = contaComponentes(&grafo);
    if (componentesConexos != 1) { // Se tiver mais de uma Árvore, o grafo é uma Floresta
        fprintf (arquivo_saida, "ERRO: GRAFO NAO CONECTADO\n");
        fclose(arquivo_saida);
        return 1;
    }

    int numVertices = obtemNrVertices(&grafo);

    // Vetores de Peso e Anteriores para utilizar no Algoritmo de Prim
    float* chaveMinPeso = (float*) malloc(sizeof(float)*numVertices);
    int* ant = (int*) malloc(sizeof(int)*numVertices);

    // Executa o Algoritmo de Prim para gerar a AGM
    AGMPrim(&grafo, 0, ant, chaveMinPeso);

    // Calculando Custo Total da AGM
    float custoTotal = 0.0;
    for (int v = 0; v < numVertices; v++) {
        custoTotal += chaveMinPeso[v];
    }

    // Printando Custo Total no Arquivo de Saída
    fprintf (arquivo_saida, "%.1f\n", custoTotal);

    // Printando Vértice Mais Distante da Origem na AGM no Arquivo de Saída
    int verticeMaisDistante = encontraMaisDistante (ant, chaveMinPeso, numVertices);
    fprintf (arquivo_saida, "%d\n", verticeMaisDistante);

    // Vetor de Vértices que compõem as Arestas da AGM
    int* ordemArestas = (int*) malloc(sizeof(int)*numVertices);
    for (int i = 1; i < numVertices; i++) {
        ordemArestas[i] = i;
    }

    // Ordenar o Vetor ordemArestas para printar corretamente no Arquivo de Saída
    for (int i = 1; i < numVertices - 1; i++) {
        for (int j = 1; j < numVertices - 1; j++) { // Compara o j com j+1

            // Vértices 1 e 2 da Aresta A
            int v1a = ordemArestas[j];
            int v2a = ant[v1a];

            // Vértices 1 e 2 da Aresta B
            int v1b = ordemArestas[j+1];
            int v2b = ant[v1b];

            // Qual Vértice é Menor e Maior na Aresta A
            int menorA = v1a;
            int maiorA = v2a;

            if (menorA > maiorA) {
                menorA = v2a;
                maiorA = v1a;
            }

            // Qual Vértice é Menor e Maior na Aresta B
            int menorB = v1b;
            int maiorB = v2b;

            if (menorB > maiorB) {
                menorB = v2b;
                maiorB = v1b;
            }

            // Verificação de Ordenação -> Trocar caso quebre a ordem Crescente do Vértice Origem e/ou Destino
            if ((menorA > menorB) || (menorA == menorB && maiorA > maiorB)) {
                int temp = ordemArestas[j];
                ordemArestas[j] = ordemArestas[j+1];
                ordemArestas[j+1] = temp;
            }
        }
    }

    // Printar as Arestas no Arquivo de Saída
    for (int i = 1; i < numVertices; i++) {
        int v1 = ordemArestas[i];
        int v2 = ant[v1];

        int menor = v1;
        int maior = v2;

        if (menor > maior) {
            maior = v1;
            menor = v2;
        }

        fprintf (arquivo_saida, "%d %d %.1f\n", menor, maior, chaveMinPeso[v1]);
    }

    free(chaveMinPeso);
    free(ant);
    free(ordemArestas);

    fclose(arquivo_saida);

    liberaGrafo(&grafo);

    return 0;
}