#include <stdlib.h>
#include <stdio.h>
#include <grafo.h>

struct node {
    int vertice;        // Valor armazenado no vértice.
    struct node* prox;
};

struct grafo {
    int nro_vertices;
    struct node** lista_adj;
};

// Criar um novo nó(vértice) no grafo.
Node* criarNode(int valor) {
    struct node* novoNode = malloc(sizeof(Node));
    novoNode->vertice = valor;
    novoNode->prox = NULL;
    return novoNode;
}

// Criar um novo grafo.
Grafo* criarGrafo(int nro_vertices) {
    Grafo* grafo = malloc(sizeof(Grafo));
    grafo->nro_vertices = nro_vertices;

    grafo->lista_adj = malloc(nro_vertices * sizeof(Node*));

    for(int i = 0; i < nro_vertices; i++)
        grafo->lista_adj[i] = NULL;

    return grafo;
}

// Adicionar uma aresta de u -> v.
int insereAresta(Grafo* grafo, int u, int v) {
    if(grafo == NULL)
        return 0;

    if (u < 0 || u >= grafo->nro_vertices)
        return 0;
    if (v < 0 || v >= grafo->nro_vertices)
        return  0;

    Node* novoNode = criarNode(v);
    novoNode->prox = grafo->lista_adj[u];
    grafo->lista_adj[u] = novoNode;
    // TODO: Tratar dígrafo
    // TODO: adicionar pesos

    return 1;
}

// Imprimir o grafo
void printGrafo(Grafo* grafo) {
    for(int v = 0; v < grafo->nro_vertices; v++) {
        Node* temp = grafo->lista_adj[v];
        printf("\n Node %d:\n ", v);
        while (temp) {
            printf("%d -> ", temp->vertice);
            temp = temp->prox;
        }
        printf("\n");
    }
}