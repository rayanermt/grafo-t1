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
    int nro_vertices;       // Número de vértices alocados.
    int lista_tam;          // Espaço total do vetor de listas.
    int nro_arestas;
    int eh_ponderado;       // 1: É ponderado, 0: Não é
    int eh_direcionado;     // 1: É direcionado, 0: Não é
    int grau_max;
    Node** lista_adj;       // Vetor de listas ligadas.
    char** labels;
    int* grau;
};

// Criar um novo grafo com número pré-determinado de vértices.
Grafo* criarGrafo(int nro_vertices, int eh_ponderado, int eh_direcionado) {
    Grafo* gr = (Grafo*) malloc( sizeof(Grafo) );

    if (gr != NULL) {
        gr->nro_vertices = nro_vertices;
        gr->nro_arestas = 0;
        gr->lista_adj = (Node **) malloc(nro_vertices * sizeof(Node *));
        gr->lista_tam = nro_vertices;
        gr->labels = (char **) malloc((nro_vertices * sizeof(char*) ));
        gr->eh_ponderado = eh_ponderado;
        gr->eh_direcionado = eh_direcionado;
        gr->grau = (int*) malloc((nro_vertices* sizeof (int) ));

        if (gr->labels == NULL) {
            printf("Erro ao alocar memória para labels.\n");
            return 0;
        }

        // Preenche o vetor de labels com strings vazias à princípio.
        for (int i = 0; i < nro_vertices; i++) {
            gr->labels[i] = (char *) malloc(256);
            sprintf(gr->labels[i], " ");
            gr->lista_adj[i] = NULL;
            gr->grau[i] = 0;
        }
    }

    return gr;
}

// Carregar informações de um arquivo em um grafo vazio.
void carregarGrafo(char *arquivo, Grafo* grafo, int ehDigrafo) {
    FILE *arquivoAberto = fopen(arquivo, "r");
    if (arquivoAberto == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    int a, b;
    char linha[256]; // Buffer para armazenar a linha lida

    while (fgets(linha, sizeof(linha), arquivoAberto)) {
        if (sscanf(linha, "%d %d", &a, &b) == 2) {

            insereAresta(grafo, a-1, b-1, 1);
        } else {
            printf("Erro ao processar a linha: %s", linha);
        }
    }
    fclose(arquivoAberto);
}

void buscaProfundidade(Grafo *gr, int inicio, int *visitado, int numeroComponente) {
    Node *p;
    visitado[inicio] = numeroComponente;  // Marca o vértice com o identificador da componente
    for (p = gr->lista_adj[inicio]; p != NULL; p = p->prox) {
        if (visitado[p->label] == -1) {
            buscaProfundidade(gr, p->label, visitado, numeroComponente);
        }
    }
}

int buscaComponentesConexa(Grafo *grafo) {
    int numeroVertices = grafo->nro_vertices;
    int i, componenteAtual = 1, j;
    int vetorAux[numeroVertices];
    for (i = 0; i < numeroVertices; i++) {
        vetorAux[i] = -1;
    }
    for (i = 0; i < numeroVertices; i++) {
        if (vetorAux[i] == -1) {
            buscaProfundidade(grafo, i, vetorAux, componenteAtual);
            componenteAtual++;
        }
    }  
    return componenteAtual - 1;
}

// Criar um novo vértice no grafo, realocar espaço para a lista de adjacência em 50% (caso necessário).
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
    gr->lista_adj[gr->nro_vertices] = NULL;
    gr->nro_vertices++;

    return 1;
}

// Função auxiliar para criar nova aresta, sem que seja necessário recursão.
int insereArestaAux(Grafo* gr, int u, int v, float w) {
    Node* aux;      // Novo vértice a ser inserido na lista de adjacência.

    // Percorre toda a lista de adjacência de U para saber se a aresta já existe.
    for (aux = gr->lista_adj[u]; aux != NULL; aux = aux->prox)
        if (aux->label == v)
            return 0;       // A aresta já existe.

    // Adicionar um novo vértice ao início da lista de adjacência de U.
    aux = malloc(sizeof(Node*));

    if(aux != NULL) {
        aux->label = v;
        aux->prox = gr->lista_adj[u];
        aux->peso = w;
        gr->lista_adj[u] = aux;
        gr->nro_arestas++;
        gr->grau[u]++;
        return 0;
    } else {
        return 1;
    }
}

// Adicionar uma aresta (u,v) no grafo, com peso w.
int insereAresta(Grafo* gr, int u, int v, float w) {
    Node* aux;

    if (gr == NULL)
        return 0;

    // Assegura que os vértices estão dentro do grafo.
    if (u < 0 || u >= gr->nro_vertices)
        return 0;
    if (v < 0 || v >= gr->nro_vertices || v == u)
        return  0;

    
    insereArestaAux(gr, u, v, w);
    if(gr->eh_direcionado == 0)
        insereArestaAux(gr, v, u, w);

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

    (removeArestaAux(gr, u, v));
    gr->nro_arestas--;

    // Remove a aresta dos dois lados, para grafos não direcionados.
    if (!gr->eh_direcionado) {
        removeArestaAux(gr, v, u);
        gr->nro_arestas--;
    }

    return 1;
}

// Retorna o grau de um vértice específico.
int grau(Grafo* gr, int vertice) {
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
        printf("Vertice %d %d\n", i, gr->grau[i]);
    }
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

// Imprimir o grafo.
void imprimirGrafo(Grafo* gr) {
    for (int v = 0; v < gr->nro_vertices; v++) {
        Node* temp = gr->lista_adj[v];
        printf("\nVertice %d", v+1);

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

int liberaGrafo(Grafo* gr) {
    if (gr == NULL) return 0;
    Node *atual, *apagar;       // Variáveis auxiliares.
    int x;                      // Variável para armazenar o valor de cada vértice.

    for(x = 0; x < gr->nro_vertices; x++) {
        atual = gr->lista_adj[x];
        while (atual) {         // Libera por cada nó da lista.
            apagar = atual;
            atual = atual->prox;
            free(apagar);
        }
    }
    free(gr->lista_adj);
    gr->nro_vertices = 0;
    gr->nro_arestas = 0;
    gr->lista_adj = NULL;
    return 1;
}

// Função auxiliar para procurar a menor distância
int procuraMenorDistancia(float *dist, int *visitado, int NV) {
    int i, menor = -1, primeiro = 1;
    for (i = 0; i < NV; i++) {
        if (dist[i] >= 0 && visitado[i] == 0) {
            if (primeiro) {
                menor = i;
                primeiro = 0;
            } else if (dist[menor] > dist[i]) {
                menor = i;
            }
        }
    }
    return menor;
}

// Função para calcular os menores caminhos a partir de um vértice
void menorCaminho_Grafo(Grafo *gr, int ini, int *ant, float *dist) {
    int i, NV = gr->nro_vertices, vert;
    int *visitado = (int *)malloc(NV * sizeof(int));
    for (i = 0; i < NV; i++) {
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }
    dist[ini] = 0;

    while (1) {
        vert = procuraMenorDistancia(dist, visitado, NV);
        if (vert == -1)
            break;

        visitado[vert] = 1;

        Node *adj = gr->lista_adj[vert];
        while (adj != NULL) {
            int ind = adj->label;
            int peso = adj->peso > 0 ? adj->peso : 1;  // Peso default = 1
            if (dist[ind] < 0 || dist[ind] > dist[vert] + peso) {
                dist[ind] = dist[vert] + peso;
                ant[ind] = vert;
            }
            adj = adj->prox;
        }
    }

    free(visitado);
}

// Função para calcular o menor caminho médio do grafo
float menorCaminhoMedio(Grafo *gr) {
    int i, j, NV = gr->nro_vertices;
    float somaMenoresCaminhos = 0;
    int *ant = (int *)malloc(NV * sizeof(int));
    float *dist = (float *)malloc(NV * sizeof(float));

    for (i = 0; i < NV; i++) {
        menorCaminho_Grafo(gr, i, ant, dist);

        for (j = 0; j < NV; j++) {
            if (dist[j] >= 0 && i != j) {
                somaMenoresCaminhos += dist[j];
            }
        }
    }

    free(ant);
    free(dist);

    return somaMenoresCaminhos / (NV * (NV - 1));
}