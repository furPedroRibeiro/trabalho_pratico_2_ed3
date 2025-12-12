//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//inclui as bibliotecas padrão 
#include "default_and_eds.h"

#ifndef GERAL_H
#define GERAL_H

// ===================== funções de uso geral =====================
int buscaBinariaVetorIndice(indice* indices, int tamanho, int idPessoa);
char *removeEspacosEmBranco(char *campo);
char *removerAspas(char *campo);
void defineStatusArquivo(FILE *arquivo, char status);
//função strsep aqui(não roda em windows a função strsep definida direto pelo GNU(se não me engano))
char* meu_strsep(char** , const char* delim);
void lerValorCampo(char *valorCampo);
FILE* abrirArquivoComStatus(const char *nomeArquivo, const char *modo);
void imprimirRegistro(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario);
resultadoBusca* buscarRegistrosPorCampo(FILE *arqPessoa, indice *vetorIndice, int qtdIndice, long sizeDados, char *nomeCampo, char *valorCampo);
void liberarListaResultados(resultadoBusca *raizLista);
void adicionarResultadoBusca(resultadoBusca **raizLista, resultadoBusca **ultimoResultado, struct registro *reg, long int byteOffset);

#endif