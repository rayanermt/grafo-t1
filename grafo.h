typedef struct grafo Grafo;
typedef struct node Node;

Grafo* criarGrafo(int nro_vertices);
int insereAresta(Grafo* gr, int u, int v, float w);

void imprimirGrafo(Grafo* grafo);
int insereVertice(Grafo* gr, int label);

void liberaGrafo(Grafo* grafo);
