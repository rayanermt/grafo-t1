typedef struct grafo Grafo;
typedef struct node Node;

Grafo* criarGrafo(int nro_vertices, int eh_ponderado, int eh_direcionado);
int insereVertice(Grafo* gr, char* label);
int insereAresta(Grafo* gr, int u, int v, float w);
int removeAresta(Grafo* gr, int u, int v);
void imprimirGrafo(Grafo* grafo);
int grau(Grafo* gr, int vertice);

int grauMax(Grafo *gr, int *v);
int grauMedio(Grafo *gr);
int obterGrauVertice(Grafo* gr, int vertice);
void carregarGrafo(Grafo* grafo, int ehDigrafo);

int liberaGrafo(Grafo* gr);