#include <stdlib.h>
#include <stdio.h>
#include <grafo.h>

int main () {
    // Testes Marcos
    Grafo* meuGrafo = criarGrafo(96); //arestas, grau_max,
    carregarGrafo(meuGrafo, 0);
    imprimirGrafo(meuGrafo);

    // TODO: Mudar a insere aresta pra colocar dos dois lados
    //
    return 0;
}