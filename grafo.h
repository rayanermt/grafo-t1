typedef struct grafo Grafo;
typedef struct node Node;

Grafo* criarGrafo(int nro_vertices);
Node* criarNode(int valor);
int insereAresta(Grafo* grafo, int u, int v);
void printGrafo(Grafo* grafo);

void liberaGrafo(Grafo* grafo);
