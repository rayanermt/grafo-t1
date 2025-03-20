#include <stdlib.h>
#include <stdio.h>
#include <grafo.h>

int main () {
    Grafo* tweetGrafo = criarGrafo(96, 0, 0);
    carregarGrafo(tweetGrafo, 0);

    insereVertice(tweetGrafo, "novoTweet");
    insereAresta(tweetGrafo, 96, 1, 0);
    insereAresta(tweetGrafo, 96, 3, 0);

    imprimirGrafo(tweetGrafo);
    int g = grauMedio(tweetGrafo);

    return 0;
}