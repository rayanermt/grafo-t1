#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <grafo.h>

struct node {
    Node* prox;
    int label;  // Identificador numérico.
    float peso;
};

struct grafo {
    int nro_vertices;   // Número de vértices alocados.
    int lista_tam;        // Espaço total do vetor de listas.
    int nro_arestas;
    int eh_ponderado;
    int grau_max;
    Node** lista_adj;       // Vetor de listas ligadas.
    char** labels;
    int* grau;
};

void carregarGrafo(Grafo* grafo, int ehDigrafo) {
    char* myFile = "E://Rayane//projetos//grafos//retweet.mtx";
        FILE *arquivo = fopen(myFile, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int a, b;
    char linha[256]; // Buffer para armazenar a linha lida

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "%d %d", &a, &b) == 2) {
            insereAresta(grafo, a, b, 0);
        } else {
            printf("Erro ao processar a linha: %s", linha);
        }
    }

    fclose(arquivo);
}

int obterGrauVertice(Grafo* gr, int vertice) {
    if (gr == NULL || vertice < 0 || vertice >= gr->nro_vertices) {
        return -1; // Retorna -1 se o grafo for inválido ou o vértice não existir
    }
    return gr->grau[vertice];
}

int grauMedio(Grafo *gr){
    if(gr == NULL) {
        return 0;
    }
    if(gr->nro_vertices == 0) {
        return -1;
    }

    int i, j, soma = 0;
    for(i=0; i < gr->nro_vertices; i++){
        soma += gr->grau[i];
    }
    printf("Soma do grau de cada vertices: %d\nNumero de vértices: %d", soma, gr->nro_vertices);
    return soma / gr->nro_vertices;
}

int grauMax(Grafo *gr, int *v){
    if(gr == NULL) {
        return 0;
    }
    if(gr->nro_vertices == 0) {
        return -1;
    }

    int i, j, maior = 0;
    for(i=0; i < gr->nro_vertices; i++){
        if(maior < gr->grau[i]) {
            maior = gr->grau[i];
            *v = i;
        }
    }
    return maior;
}

// Criar um novo nó(vértice) no grafo, realocar espaço para a lista de adjacência em 50% (caso necessário).
int insereVertice(Grafo* gr, char* label) {
    int novoNroVertices = gr->nro_vertices + 1;

    if (novoNroVertices > gr->lista_tam) {
        int novoTamanho = gr->nro_vertices * 1.5;
        gr->lista_adj = realloc(gr->lista_adj, novoTamanho * sizeof(Node*));
    }

    if (gr->lista_adj == NULL)
        return 0;

    Node* novoNode = malloc(sizeof(Node));

    if (novoNode != NULL) {
        novoNode->prox = NULL;
        novoNode->label = gr->nro_vertices;
        novoNode->peso = 0;
    }

    gr->labels = realloc(gr->labels, novoNroVertices * sizeof(char*));
    gr->labels[gr->nro_vertices] = label;
    gr->lista_adj[gr->nro_vertices] = novoNode;
    gr->nro_vertices++;

    return 1;
}

// Criar um novo grafo.
Grafo* criarGrafo(int nro_vertices, int eh_ponderado) {
    Grafo* gr = (Grafo*) malloc( sizeof(Grafo) );

    if (gr != NULL) {
        gr->nro_vertices = nro_vertices;
        gr->nro_arestas = 0;
        gr->lista_adj = (Node **) malloc(nro_vertices * sizeof(Node *));
        gr->lista_tam = nro_vertices;
        gr->labels = (char **) malloc((nro_vertices * sizeof(char*) ));
        gr->eh_ponderado = eh_ponderado;

        if (gr->labels == NULL) {
            printf("Erro ao alocar memória para labels.\n");
            return 0;
        }

        // Preenche o vetor de labels com strings vazias à princípio.
        for (int i = 0; i < nro_vertices; i++) {
            gr->labels[i] = (char *) malloc(256);
            sprintf(gr->labels[i], " ");
            gr->lista_adj[i] = NULL;
        }
    }

    return gr;
}

// Adicionar uma aresta (u,v) no grafo, com peso w.
int insereAresta(Grafo* gr, int u, int v, float w) {
    Node* aux;

    if (gr == NULL)
        return 0;

    // Assegura que os vértices estão dentro do grafo.
    if (u < 0 || u >= gr->nro_vertices)
        return 0;
    if (v < 0 || v >= gr->nro_vertices)
        return  0;

    // Percorre toda a lista de adjacência de U para saber se a aresta já existe.
    for (aux = gr->lista_adj[u]; aux != NULL; aux = aux->prox)
        if (aux->label == v)
            return 0;       // A aresta já existe.

    // Adicionar um novo vértice ao início da lista de adjacência de U.
   aux = malloc(sizeof(Node*));
   aux->label = v;
   aux->prox = gr->lista_adj[u];
   aux->peso = w;
   gr->lista_adj[u] = aux;
   gr->nro_arestas++;

    // TODO: Tratar grafo não direcionado.
    return 1;
}

// Função auxiliar para grafos não direcionados.
int removeArestaAux(Grafo* gr, int u, int v) {
    Node* ant = NULL;                   // Vértice anterior à V.
    Node* atual = gr->lista_adj[u];     // Vértice para o início da lista de adjacência de U.

    while (atual && atual->label < v) {
        ant = atual;
        atual = atual->prox;
    }

    if (atual && atual->label == v) {
        if (ant)
            ant->prox = atual->prox; // Se já havia um anterior na lista ligada, este é atualizado com ovértice seguinte ao que foi deletado
        else    // Se a lista de adjacência estava vazia.
            gr->lista_adj[u] = atual->prox;
        free(atual);
        return 1;
    }

    printf("ERRO: A aresta nao existe.\n");
    return 0;
}

// Remove a aresta (u, v) do grafo, caso exista.
int removeAresta(Grafo* gr, int u, int v) {
    // Assegurar que os vértices existem no grafo.
    if (gr == NULL || u < 0 || u >= gr->nro_vertices)
        return 0;
    if (v < 0 || v >= gr->nro_vertices)
        return  0;

    // Remove a resta dos dois lados, para grafos não direcionados.
    if (removeArestaAux(gr, u, v)) {
        removeArestaAux(gr, v, u);
        gr->nro_arestas--;
    }

    // TODO: Tratar grafo direcionado.
    return 1;
}

// Imprimir o grafo
void imprimirGrafo(Grafo* gr) {
    for (int v = 0; v < gr->nro_vertices; v++) {
        Node* temp = gr->lista_adj[v];
        printf("\nVertice %d", v);

        // Formatar a impressão dos dados.
        (strcmp(gr->labels[v], " ") != 0) ? printf(" (%s): \n", gr->labels[v]) : printf("\n");

        while (temp) {
            printf("%d ", temp->label);
            (gr->eh_ponderado == 0) ?  printf("-> ") : printf("(%.2f) -> ", temp->peso);
            temp = temp->prox;
        }
        printf("\n");
    }
}