//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_3.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"

//FUNCIONALIDADE 3:

void listarRegistros(char *nomeArquivoEntrada) {
    FILE *arqPessoa = abrirArquivoComStatus(nomeArquivoEntrada, "rb");
    
    //Tamanho do arquivo
    fseek(arqPessoa, 0, SEEK_END);
    long sizeDados = ftell(arqPessoa);
    
    //Buscar todos os registros - enviamos para a função modular de busca imprimir todos os registros 
    //os campos são enviados como nulo pois não temos como objetivo fazer uma busca específica, e sim printar todos os resultados
    resultadoBusca *resultados = buscarRegistrosPorCampo(arqPessoa, NULL, 0, sizeDados, NULL, NULL);
    
    //Percorrer a lista de resultados e imprimir
    resultadoBusca *atual = resultados;
    while (atual != NULL) {
        imprimirRegistro(atual->reg->idPessoa, atual->reg->idadePessoa, atual->reg->tamanhoNomePessoa, atual->reg->nome, atual->reg->tamanhoNomeUsuario, atual->reg->nomeUsuario);
        atual = atual->proxResultado;
    }
    
    // Liberar memória da lista
    liberarListaResultados(resultados);
    fclose(arqPessoa);
}

