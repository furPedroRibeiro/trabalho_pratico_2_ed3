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


//Estrutura para representar uma aresta do grafo (lista de adjacências)
typedef struct arestaGrafo{
    char nomeUsuarioDestino[100];  //Nome do usuário que é seguido
    char dataInicio[11];            //Data de início que segue
    char dataFim[11];               //Data de fim que segue
    char grauAmizade;               //Grau de amizade
    struct arestaGrafo *proxAresta; //Próxima aresta na lista
} arestaGrafo;

//Estrutura para representar um vértice do grafo
typedef struct verticeGrafo{
    int idPessoa;                   //ID da pessoa
    char nomeUsuario[100];          //Nome do usuário (pessoa que segue)
    arestaGrafo *listaAdjacencias;  //Lista de adjacências (quem essa pessoa segue)
} verticeGrafo;


int comparaVertices(const void *a, const void *b);
int comparaArestas(const void *a, const void *b);
void construirListas(verticeGrafo *vertices, int numVertices, noSegue *registrosSegue, int qtdRegistrosSegue, resultadoBusca *resultados);
char* buscarNomeUsuarioPorId(int idPessoa, resultadoBusca *resultados);
void imprimirGrafo(verticeGrafo *vertices, int numVertices);
void liberarGrafo(verticeGrafo *vertices, int numVertices);

#endif