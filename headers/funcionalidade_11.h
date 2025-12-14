//Aluno 1: Pedro Luis de Alencar Ribeiro Nº USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda Nº USP: 15443221

#include "default_and_eds.h"

#ifndef FUNCIONALIDADE_11_H
#define FUNCIONALIDADE_11_H

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

//Protótipos das funções da funcionalidade 11
void gerarGrafo(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado);
int comparaVertices11(const void *a, const void *b);
int comparaArestas11(const void *a, const void *b);
char* buscarNomeUsuarioPorId11(int idPessoa, resultadoBusca *resultados);
void construirListasAdjacencias11(verticeGrafo *vertices, int numVertices, noSegue *registrosSegue, int qtdRegistrosSegue, resultadoBusca *resultados);
void imprimirGrafo11(verticeGrafo *vertices, int numVertices);
void liberarGrafo11(verticeGrafo *vertices, int numVertices);

#endif