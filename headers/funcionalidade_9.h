//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//inclui as bibliotecas padrão 
#include "default_and_eds.h"

#ifndef FUNCIONALIDADE_9_H
#define FUNCIONALIDADE_9_H

//funcionalidade 9:
void ordenaArquivoSegue(char *nomeArquivoDesordenado, char *nomeArquivoOrdenado);

// ===================== funções para funcionalidade 9: =====================

int comparaParaOrdenar(const void *a, const void *b);
void escreveSegueOrdenado(FILE *arqOrdenado, int sizeArray, noSegue *registros);

#endif