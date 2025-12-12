//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

//inclui as bibliotecas padrão 
#include "default.h"

#ifndef AUXILIAR_H
#define AUXILIAR_H

// ====================================================================================

//todas as estruturas de dados são definidas antes das funções:

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

//estrutura de dados do tipo lista duplamente encadeada para armazenar os registros do arquivo de índice antes de inseri-lo, é duplamente encadeada pois na hora da inserção é necessário inserir um registro no meio de outros registros, necessitando assim que seja verificado o nó anterior e o próximo da lista(talvez na implementação da inserção seja observado que a lista não tem necessidade de ser duplamente encadeada, mas para maior eficiência no futuro, é duplamente encadeada desde já) 
typedef struct indice{
  int idPessoa;
  long int byteOffset;
  struct indice *proxIndice;
  struct indice *antIndice;
} indice;

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

typedef struct noRegistroUnico{
  int idPessoa;
  char *nomePessoa;
  int idadePessoa;
  char *nomeUsuario;
} noRegistroUnico;

//define as variáveis para raíz da lista 1 e da lista 2
extern registro* raizListaPessoas;
extern indice* raizListaIndice;


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


// ===================== funções para as funcionalidade 3 e 4 =====================

FILE* abrirArquivoComStatus(const char *nomeArquivo, const char *modo);
void imprimirRegistro(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario);
//busca binaria para encontrar o byteOffset do registro com o id buscado
// long int buscaBinariaIndice(indice *vetor, int tamanho, int idBuscado);
void imprimirRegistroPorByteOffset(FILE *arqPessoa, long int byteOffset);

//para a função 4, busca
resultadoBusca* buscarRegistrosPorCampo(FILE *arqPessoa, indice *vetorIndice, int qtdIndice, long sizeDados, char *nomeCampo, char *valorCampo);
void adicionarResultadoBusca(resultadoBusca **raizLista, resultadoBusca **ultimoResultado, struct registro *reg, long int byteOffset);
void liberarListaResultados(resultadoBusca *raizLista);


// ===================== funções para a funcionalidade 6: =====================

cabecalhoPessoa* lerCabecalho(FILE *nomeArquivo);
noRegistroUnico* lerEntradaInsercaoUnica();
void insereRegistroUnicoPessoa(FILE *nomeArquivoPessoa, noRegistroUnico* regUnico, cabecalhoPessoa* cabecalho);
indice* lerArquivoIndice(FILE *nomeArquivoIndice, int n, int mais_n);
void insereRegistroUnicoVetorIndice(indice* indices, int tamanhoVetor, int idPessoa, long int byteoffset);
void insereIndice(indice* indices, FILE *nomeArquivoIndice, int tamanho);


// ===================== funções para a funcionalidade 7: =====================

void atualizarRegistroIndividual(FILE *arqPessoa, long int posRegistro, char *nomeCampoAtualiza, char *valorCampoAtualiza, cabecalhoPessoa *cabecalho, indice *vetorIndice, int idPessoaAtual);
// int buscaBinariaAtualizar(indice* vetorIndice, int tamanho, int idPessoa);


// ===================== funções para funcionalidade 8: =====================

//estrutura nó para um registro de segue:
typedef struct noSegue{
  char removido[2];
  int idPessoaQueSegue;
  int idPessoaQueESeguida;
  char *dataInicioQueSegue;
  char *dataFimQueSegue;
  char grauAmizade[2];
} noSegue;

void criaCabecalhoArquivoSegue(FILE *arqSegue, char status, int quantidadeRegistros, int proxByteoffset);
void insereArquivoSegue(FILE *arqSegue, noSegue *noAtual);


// ===================== funções para funcionalidade 9: =====================

int comparaParaOrdenar(const void *a, const void *b);
void escreveSegueOrdenado(FILE *arqOrdenado, int sizeArray, noSegue *registros);

// ===================== funções para funcionalidade 10: =====================

long int buscaBinariaSegueModificada(noSegue *registros, int tamanho, int idPessoaBuscado);
void imprimirJuncao(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario, noSegue *registrosSegue, int tamanhoSegue, int idPessoaBuscado);


// ===================== funções de uso geral =====================
int buscaBinariaVetorIndice(indice* indices, int tamanho, int idPessoa);
char *removeEspacosEmBranco(char *campo);
char *removerAspas(char *campo);
void defineStatusArquivo(FILE *arquivo, char status);
//função strsep aqui(não roda em windows a função strsep definida direto pelo GNU(se não me engano))
char* meu_strsep(char** , const char* delim);
void lerValorCampo(char *valorCampo);

#endif