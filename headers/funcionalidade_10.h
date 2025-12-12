//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//inclui as bibliotecas padrão 
#include "default_and_eds.h"

#ifndef FUNCIONALIDADE_10_H
#define FUNCIONALIDADE_10_H

//funcionalidade 10:
void juncaoArquivos(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoOrdenado, int n);

// ===================== funções para funcionalidade 10: =====================

long int buscaBinariaSegueModificada(noSegue *registros, int tamanho, int idPessoaBuscado);
void imprimirJuncao(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario, noSegue *registrosSegue, int tamanhoSegue, int idPessoaBuscado);

#endif