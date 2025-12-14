//Aluno 1: Pedro Luis de Alencar Ribeiro Nº USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda Nº USP: 15443221

#include "default_and_eds.h"
#include "funcionalidade_11.h"  //Usa as mesmas estruturas de dados

#ifndef FUNCIONALIDADE_12_H
#define FUNCIONALIDADE_12_H

//Protótipos das funções da funcionalidade 12
void gerarGrafoTransposto(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado);
int comparaVertices12(const void *a, const void *b);
int comparaArestas12(const void *a, const void *b);
char* buscarNomeUsuarioPorId12(int idPessoa, resultadoBusca *resultados);
void construirListasAdjacenciasTranspostas(verticeGrafo *vertices, int numVertices, noSegue *registrosSegue, int qtdRegistrosSegue, resultadoBusca *resultados);
void imprimirGrafo12(verticeGrafo *vertices, int numVertices);
void liberarGrafo12(verticeGrafo *vertices, int numVertices);

#endif