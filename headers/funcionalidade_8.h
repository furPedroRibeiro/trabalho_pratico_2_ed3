//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//inclui as bibliotecas padrão 
#include "default_and_eds.h"

#ifndef FUNCIONALIDADE_8_H
#define FUNCIONALIDADE_8_H

//funcionalidade 8:
void criaArquivoSegue(char *nomeArquivoEntradaSegue, char *nomeArquivoSaidaSegue);

// ===================== funções para funcionalidade 8: =====================

void criaCabecalhoArquivoSegue(FILE *arqSegue, char status, int quantidadeRegistros, int proxByteoffset);
void insereArquivoSegue(FILE *arqSegue, noSegue *noAtual);

#endif