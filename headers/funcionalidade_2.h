//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//inclui as bibliotecas padrão 
#include "default_and_eds.h"

#ifndef FUNCIONALIDADE_2_H
#define FUNCIONALIDADE_2_H


//funcionalidade 2
void criarArquivoDados(char *nomeArquivoEntrada, char *nomeArquivoSaida, char *nomeArquivoIndice);

//====================================================================================
//funções começam aqui:
// ===================== funções para as funcionalidades 1 e 2 =====================

//FUNÇÃO DE LER O ARQUIVO CSV
void lerCSV(FILE* arquivoEntrada, FILE* arquivoDados, FILE* arquivoIndice);
void criaCabecalhoArquivoDados(FILE *arqDados, char status, int quantidadePessoas, int quantidadeRemovidos, long int proxByteoffset);
//função para inserir um registro no arquivo de dados, se der errado retorna 0
void insereRegistro(registro* novoRegistro, FILE* arquivoDados, int quantidadeRemovidos, long int proxByteoffset);
//função para inserir os registros de índice no arquivo de índice
void insereRegistroIndice(indice* raizListaIndice, FILE* arquivoIndice);
//FUNÇÃO PARA CRIAR UM NÓ DE REGISTRO E ADICIONÁ-LO A LISTA DE REGISTROS, ESSA FUNÇÃO É CHAMADA DENTRO DE LERCSV()
void criarNoRegistro(registro* novoRegistro, char *campoIdPessoa, char *campoIdadePessoa, char *campoNomePessoa, char *campoNomeUsuario, int tamRegistroBytes);
//Função para criar um nó e adicioná-lo a lista de registros do arquivo de indice, não retorna nada
void criarNoRegistroIndice(indice* novoRegistroIndice, char *campoIdPessoa, long int byteoffset);

#endif