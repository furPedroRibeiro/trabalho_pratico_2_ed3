//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//inclui as bibliotecas padrão 
#include "default_and_eds.h"

#ifndef FUNCIONALIDADE_6_H
#define FUNCIONALIDADE_6_H

//funcionalidade 6
void inserirUnicoRegistro(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n);

// ===================== funções para a funcionalidade 6: =====================

cabecalhoPessoa* lerCabecalho(FILE *nomeArquivo);
noRegistroUnico* lerEntradaInsercaoUnica();
void insereRegistroUnicoPessoa(FILE *nomeArquivoPessoa, noRegistroUnico* regUnico, cabecalhoPessoa* cabecalho);
indice* lerArquivoIndice(FILE *nomeArquivoIndice, int n, int mais_n);
void insereRegistroUnicoVetorIndice(indice* indices, int tamanhoVetor, int idPessoa, long int byteoffset);
void insereIndice(indice* indices, FILE *nomeArquivoIndice, int tamanho);


#endif