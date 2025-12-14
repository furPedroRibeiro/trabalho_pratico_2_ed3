//Aluno 1: Pedro Luis de Alencar Ribeiro Nº USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda Nº USP: 15443221

#include "default_and_eds.h"
#include "geral.h"

#ifndef FUNCIONALIDADE_13_H
#define FUNCIONALIDADE_13_H

//Estrutura para representar um nó da fila usada no BFS
typedef struct noFila{
    int idxVertice;
    struct noFila *prox;
} noFila;

//Estrutura para representar uma fila
typedef struct fila{
    noFila *inicio;
    noFila *fim;
} fila;

//Typedef para compatibilidade com código existente
typedef fila Fila;

//Estrutura para ordenar arestas durante o BFS
typedef struct arestaOrdenada{
    int idxDestino;
    char nomeUsuarioDestino[100];
    char dataInicio[11];
    char dataFim[11];
    char grauAmizade;
    arestaGrafo *arestaOriginal;
} arestaOrdenada;

//Função principal
void encontrarCaminhosMaisCurtos(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado, char *nomeUsuarioCelebridade);

//Alias para compatibilidade com main.c
void caminhoMaisCurtoCelebridade(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado, char *nomeUsuarioCelebridade);

//Funções auxiliares para a fila
fila* criarFila();
void enfileirar(fila *f, int indiceVertice);
int desenfileirar(fila *f);
int filaVazia(fila *f);
void liberarFila(fila *f);

//Funções auxiliares do algoritmo (exportadas para uso em outras funcionalidades)
int buscarIndicePorNome(verticeGrafo *vertices, int numVertices, char *nomeUsuario);
int encontrarIndicePorNome(verticeGrafo *vertices, int numVertices, char *nomeUsuario); //Alias para compatibilidade
void imprimirCaminho(int *predecessores, arestaGrafo **arestasUsadas, verticeGrafo *vertices, int indiceOrigem, int indiceDestino);
int comparaArestasOrdenadas(const void *a, const void *b);
int comparaArestasGrafo(arestaGrafo *a, arestaGrafo *b);
void construirGrafoAtivo(verticeGrafo *vertices, int numVertices, noSegue *registrosSegue, int qtdRegistrosSegue, resultadoBusca *resultados);

#endif