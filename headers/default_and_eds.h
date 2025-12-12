//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//aqui são declaradas as bibliotecas padrão que serão usadas no projeto todo, evita que em todo .h se definam as mesmas bibliotecas 

#ifndef DEFAULT_H
#define DEFAULT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

//estrutura de dados do tipo lista para armazenar os registros antes de escrever no arquivo de dados que será gerado, a lista possui encadeamento simples porque não há necessidade dos dados serem ordenados nem nada do tipo, a única necessidade é que a lista exista e o encadeamento seja feito de maneira correta
typedef struct registro{
  char removido[2];
  int tamRegistro;
  int idPessoa;
  int idadePessoa;
  int tamanhoNomePessoa;
  char nome[100];
  int tamanhoNomeUsuario;
  char nomeUsuario[100];
  struct registro *proxRegistro;
} registro;


//estrutura de dados do tipo lista duplamente encadeada para armazenar os campos encontrados na busca
typedef struct resultadoBusca{
  registro *reg;
  long int byteOffset;
  struct resultadoBusca *proxResultado;
} resultadoBusca;

//nó que armazenará o registro de cabeçalho:
typedef struct cabecalhoPessoa{
  char status[2];
  int quantidadePessoas;
  int quantidadeRemovidos;
  long int proxByteoffset;
} cabecalhoPessoa;

//define as variáveis para raíz da lista 1 e da lista 2
extern registro* raizListaPessoas;
extern indice* raizListaIndice;

//estrutura para um registro único utilizada na funcionalidade 6
typedef struct noRegistroUnico{
  int idPessoa;
  char *nomePessoa;
  int idadePessoa;
  char *nomeUsuario;
} noRegistroUnico;

//estrutura nó para um registro de segue:
typedef struct noSegue{
  char removido[2];
  int idPessoaQueSegue;
  int idPessoaQueESeguida;
  char *dataInicioQueSegue;
  char *dataFimQueSegue;
  char grauAmizade[2];
} noSegue;

#endif