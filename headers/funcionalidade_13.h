//Aluno 1: Pedro Luis de Alencar Ribeiro Nº USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda Nº USP: 15443221

#include "default_and_eds.h"
#include "funcionalidade_11.h"

#ifndef FUNCIONALIDADE_13_H
#define FUNCIONALIDADE_13_H

//Estrutura para o nó da fila (BFS)
typedef struct noFila {
    int indiceVertice;
    struct noFila *proximo;
} noFila;

//Estrutura para a fila
typedef struct {
    noFila *inicio;
    noFila *fim;
} Fila;

//Estrutura para armazenar o caminho
typedef struct nodoCaminho {
    char nomeUsuarioQueSegue[100];
    char nomeUsuarioQueESeguida[100];
    char dataInicio[11];
    char dataFim[11];
    char grauAmizade;
    struct nodoCaminho *proximo;
} nodoCaminho;

//Protótipos das funções da funcionalidade 13
void caminhoMaisCurtoCelebridade(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado, char *nomeUsuarioCelebridade);
Fila* criarFila();
void enfileirar(Fila *fila, int indiceVertice);
int desenfileirar(Fila *fila);
int filaVazia(Fila *fila);
void liberarFila(Fila *fila);
int encontrarIndicePorNome(verticeGrafo *vertices, int numVertices, char *nomeUsuario);
nodoCaminho* reconstruirCaminho(int *predecessores, arestaGrafo **arestasUsadas, verticeGrafo *vertices, int indiceOrigem, int indiceDestino);
void imprimirCaminho(nodoCaminho *caminho);
void liberarCaminho(nodoCaminho *caminho);
int comparaArestasOrdenadas(const void *a, const void *b);

#endif