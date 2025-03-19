#include <stdlib.h>
#include <stdio.h>
#include <grafo.h>

struct node {
    Node* prox;
    int label;
    float peso;
};

struct grafo {
    int nro_vertices;
    int nro_arestas;
    int eh_ponderado;
    int grau_max;
    Node** lista_adj;       // Vetor de listas ligadas.
};

// Criar um novo nó(vértice) no grafo.
Node* insereVertice(int label) {
    struct node* novoNode = malloc(sizeof(Node));

    if (novoNode != NULL) {
        novoNode->prox = NULL;
        novoNode->label = label;
    }

    return novoNode;
}

// Criar um novo grafo.
Grafo* criarGrafo(int nro_vertices) {
    Grafo* gr = (Grafo*) malloc( sizeof(Grafo) );

    if (gr != NULL) {
        gr->nro_vertices = nro_vertices;
        gr->nro_arestas = 0;
        gr->lista_adj = (Node **) malloc(nro_vertices * sizeof(Node *));

        for (int i = 0; i < nro_vertices; i++) {
            gr->lista_adj[i] = NULL;
        }
    }

    return gr;
}

// Adicionar uma aresta (u,v) no grafo, com peso w.
int insereAresta(Grafo* gr, int u, int v, float w) {
    Node* aux;

    if(gr == NULL)
        return 0;

    // Assegura que os vértices estão dentro do grafo.
    if (u < 0 || u >= gr->nro_vertices)
        return 0;
    if (v < 0 || v >= gr->nro_vertices)
        return  0;

    // Percorre toda a lista de adjacência de U para saber se a aresta já existe.
    for (aux = gr->lista_adj[u]; aux != NULL; aux = aux->prox)
        if (aux->label == v)
            return 0;

// Adicionar um novo vértice ao início da lista de adjacência de U.
   aux = malloc(sizeof(Node*));
   aux->label = v;
   aux->prox = gr->lista_adj[u];
   aux->peso = w;
   gr->lista_adj[u] = aux;
   gr->nro_arestas++;

    // TODO: Tratar dígrafo
    return 1;
}

// Imprimir o grafo
void imprimirGrafo(Grafo* grafo) {
    for(int v = 0; v < grafo->nro_vertices; v++) {
        Node* temp = grafo->lista_adj[v];
        printf("\n Node %d:\n ", v);
        while (temp) {
            printf("%d (%.2f) -> ", temp->label, temp->peso);
            temp = temp->prox;
        }
        printf("\n");
    }
}