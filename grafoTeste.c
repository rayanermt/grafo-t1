#include <stdlib.h>
#include <stdio.h>
#include <grafo.h>

int main () {
    Grafo* meuGrafo = criarGrafo(5);
    insereAresta(meuGrafo, 1, 2, 0.1);
    insereAresta(meuGrafo, 4, 2, 0.2);
    insereAresta(meuGrafo, 1, 2, 0);

    imprimirGrafo(meuGrafo);

    return 0;
}