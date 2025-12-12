//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//inclui as bibliotecas padrão 
#include "default_and_eds.h"

#ifndef FUNCIONALIDADE_7_H
#define FUNCIONALIDADE_7_H

//funcionalidade 7
void atualizarRegistro(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n);

// ===================== funções para a funcionalidade 7: =====================

void atualizarRegistroIndividual(FILE *arqPessoa, long int posRegistro, char *nomeCampoAtualiza, char *valorCampoAtualiza, cabecalhoPessoa *cabecalho, indice *vetorIndice, int idPessoaAtual);

#endif