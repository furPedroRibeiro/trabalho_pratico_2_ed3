//Aluno 1: Pedro Luis de Alencar Ribeiro Nº USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda Nº USP: 15443221

#include "default_and_eds.h"
#include "geral.h"

#ifndef FUNCIONALIDADE_12_H
#define FUNCIONALIDADE_12_H

void gerarGrafoTransposto(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado);
void construirListasAdjacenciasTranspostas(verticeGrafo *vertices, int numVertices, noSegue *registrosSegue, int qtdRegistrosSegue, resultadoBusca *resultados);

#endif