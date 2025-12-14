//Aluno 1: Pedro Luis de Alencar Ribeiro Nº USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda Nº USP: 15443221

#include "../headers/funcionalidade_11.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"


//FUNCIONALIDADE 11:
void gerarGrafo(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado){
    //Abertura dos arquivos
    FILE *arqPessoa = abrirArquivoComStatus(nomeArquivoPessoa, "rb");
    FILE *arquivoIndice = abrirArquivoComStatus(nomeArquivoIndice, "rb");
    FILE *arqSegueOrdenado = abrirArquivoComStatus(nomeArquivoSegueOrdenado, "rb");
    
    //Carrega o índice em memória
    fseek(arquivoIndice, 0, SEEK_END);
    long sizeIndice = ftell(arquivoIndice) - 12;
    int qtdIndice = sizeIndice / (sizeof(int) + sizeof(long int));
    
    fseek(arquivoIndice, 12, SEEK_SET);
    indice *vetorIndice = malloc(qtdIndice * sizeof(indice));
    for(int i = 0; i < qtdIndice; i++){
        fread(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fread(&vetorIndice[i].byteOffset, sizeof(long int), 1, arquivoIndice);
    }
    fclose(arquivoIndice);
    
    //Obtém o tamanho do arquivo pessoa
    fseek(arqPessoa, 0, SEEK_END);
    long sizeDados = ftell(arqPessoa);
    
    //Busca todos os registros não removidos do arquivo pessoa
    resultadoBusca *resultados = buscarRegistrosPorCampo(arqPessoa, vetorIndice, qtdIndice, sizeDados, NULL, NULL);
    fclose(arqPessoa);
    
    if(resultados == NULL){
        printf("Falha na execução da funcionalidade.\n");
        free(vetorIndice);
        fclose(arqSegueOrdenado);
        return;
    }
    
    //Conta quantas pessoas existem e cria vetor de vértices
    int numVertices = 0;
    resultadoBusca *atual = resultados;
    while(atual != NULL){
        numVertices++;
        atual = atual->proxResultado;
    }
    
    //Cria o vetor de vértices (pessoas que seguem)
    verticeGrafo *vertices = malloc(numVertices * sizeof(verticeGrafo));
    
    //Preenche o vetor de vértices com os nomes de usuário
    atual = resultados;
    int idx = 0;
    while(atual != NULL){
        vertices[idx].idPessoa = atual->reg->idPessoa;
        strcpy(vertices[idx].nomeUsuario, atual->reg->nomeUsuario);
        vertices[idx].listaAdjacencias = NULL;
        idx++;
        atual = atual->proxResultado;
    }
    
    //Ordena o vetor de vértices por nomeUsuario
    qsort(vertices, numVertices, sizeof(verticeGrafo), comparaVertices11);
    
    //Carrega todo o arquivo segue ordenado em memória
    int qtdRegistrosSegue;
    fseek(arqSegueOrdenado, 1, SEEK_SET);
    fread(&qtdRegistrosSegue, sizeof(int), 1, arqSegueOrdenado);
    
    noSegue *registrosSegue = (noSegue*)calloc(qtdRegistrosSegue, sizeof(noSegue));
    
    fseek(arqSegueOrdenado, 9, SEEK_SET);
    for(int i = 0; i < qtdRegistrosSegue; i++){
        registrosSegue[i].dataInicioQueSegue = malloc(11 * sizeof(char));
        registrosSegue[i].dataFimQueSegue = malloc(11 * sizeof(char));
        
        fread(registrosSegue[i].removido, sizeof(char), 1, arqSegueOrdenado);
        fread(&registrosSegue[i].idPessoaQueSegue, sizeof(int), 1, arqSegueOrdenado);
        fread(&registrosSegue[i].idPessoaQueESeguida, sizeof(int), 1, arqSegueOrdenado);
        fread(registrosSegue[i].dataInicioQueSegue, sizeof(char), 10, arqSegueOrdenado);
        registrosSegue[i].dataInicioQueSegue[10] = '\0';
        fread(registrosSegue[i].dataFimQueSegue, sizeof(char), 10, arqSegueOrdenado);
        registrosSegue[i].dataFimQueSegue[10] = '\0';
        fread(registrosSegue[i].grauAmizade, sizeof(char), 1, arqSegueOrdenado);
        registrosSegue[i].grauAmizade[1] = '\0';
    }
    fclose(arqSegueOrdenado);
    
    //Constrói as listas de adjacências
    construirListasAdjacencias11(vertices, numVertices, registrosSegue, qtdRegistrosSegue, resultados);
    
    //Imprime o grafo
    imprimirGrafo11(vertices, numVertices);
    
    //Libera memória
    liberarGrafo11(vertices, numVertices);
    liberarListaResultados(resultados);
    free(vetorIndice);
    for(int i = 0; i < qtdRegistrosSegue; i++){
        free(registrosSegue[i].dataInicioQueSegue);
        free(registrosSegue[i].dataFimQueSegue);
    }
    free(registrosSegue);
}


// ======================= FUNÇÕES PARA A FUNCIONALIDADE 11: =======================

//Função de comparação para ordenar vértices por nomeUsuario
int comparaVertices11(const void *a, const void *b){
    const verticeGrafo *vA = (const verticeGrafo*)a;
    const verticeGrafo *vB = (const verticeGrafo*)b;
    return strcmp(vA->nomeUsuario, vB->nomeUsuario);
}

//Função de comparação para ordenar arestas por nomeUsuario e dataInicio
int comparaArestas11(const void *a, const void *b){
    const arestaGrafo *aA = (const arestaGrafo*)a;
    const arestaGrafo *aB = (const arestaGrafo*)b;
    
    int cmpNome = strcmp(aA->nomeUsuarioDestino, aB->nomeUsuarioDestino);
    if(cmpNome != 0){
        return cmpNome;
    }
    
    //Se nomes iguais, ordena por dataInicio
    return strcmp(aA->dataInicio, aB->dataInicio);
}

//Função que busca o nomeUsuario de uma pessoa pelo seu ID
char* buscarNomeUsuarioPorId11(int idPessoa, resultadoBusca *resultados){
    resultadoBusca *atual = resultados;
    while(atual != NULL){
        if(atual->reg->idPessoa == idPessoa){
            return atual->reg->nomeUsuario;
        }
        atual = atual->proxResultado;
    }
    return NULL;
}

//Função que constrói as listas de adjacências do grafo
void construirListasAdjacencias11(verticeGrafo *vertices, int numVertices, noSegue *registrosSegue, int qtdRegistrosSegue, resultadoBusca *resultados){
    
    //Para cada vértice (pessoa que segue)
    for(int i = 0; i < numVertices; i++){
        int idPessoaQueSegue = vertices[i].idPessoa;
        
        //Conta quantas pessoas esse usuário segue (não removidas)
        int numArestas = 0;
        for(int j = 0; j < qtdRegistrosSegue; j++){
            if(registrosSegue[j].idPessoaQueSegue == idPessoaQueSegue && 
               registrosSegue[j].removido[0] == '0'){
                numArestas++;
            }
        }
        
        if(numArestas == 0){
            vertices[i].listaAdjacencias = NULL;
            continue;
        }
        
        //Aloca vetor temporário para ordenar as arestas
        arestaGrafo *arestasTemp = malloc(numArestas * sizeof(arestaGrafo));
        int idxAresta = 0;
        
        //Preenche o vetor de arestas
        for(int j = 0; j < qtdRegistrosSegue; j++){
            if(registrosSegue[j].idPessoaQueSegue == idPessoaQueSegue && 
               registrosSegue[j].removido[0] == '0'){
                
                //Busca o nomeUsuario da pessoa seguida
                char *nomeUsuarioSeguido = buscarNomeUsuarioPorId11(registrosSegue[j].idPessoaQueESeguida, resultados);
                
                if(nomeUsuarioSeguido != NULL){
                    strcpy(arestasTemp[idxAresta].nomeUsuarioDestino, nomeUsuarioSeguido);
                    strcpy(arestasTemp[idxAresta].dataInicio, registrosSegue[j].dataInicioQueSegue);
                    strcpy(arestasTemp[idxAresta].dataFim, registrosSegue[j].dataFimQueSegue);
                    arestasTemp[idxAresta].grauAmizade = registrosSegue[j].grauAmizade[0];
                    arestasTemp[idxAresta].proxAresta = NULL;
                    idxAresta++;
                }
            }
        }
        
        numArestas = idxAresta; //Atualiza caso algum nome não foi encontrado
        
        //Ordena as arestas por nomeUsuario e dataInicio
        qsort(arestasTemp, numArestas, sizeof(arestaGrafo), comparaArestas11);
        
        //Constrói a lista encadeada de arestas
        for(int k = 0; k < numArestas; k++){
            arestaGrafo *novaAresta = malloc(sizeof(arestaGrafo));
            strcpy(novaAresta->nomeUsuarioDestino, arestasTemp[k].nomeUsuarioDestino);
            strcpy(novaAresta->dataInicio, arestasTemp[k].dataInicio);
            strcpy(novaAresta->dataFim, arestasTemp[k].dataFim);
            novaAresta->grauAmizade = arestasTemp[k].grauAmizade;
            novaAresta->proxAresta = NULL;
            
            //Adiciona na lista
            if(vertices[i].listaAdjacencias == NULL){
                vertices[i].listaAdjacencias = novaAresta;
            } else {
                //Encontra o último nó da lista
                arestaGrafo *ultimo = vertices[i].listaAdjacencias;
                while(ultimo->proxAresta != NULL){
                    ultimo = ultimo->proxAresta;
                }
                ultimo->proxAresta = novaAresta;
            }
        }
        
        free(arestasTemp);
    }
}

void imprimirGrafo11(verticeGrafo *vertices, int numVertices){
    for(int i = 0; i < numVertices; i++){
        if(vertices[i].listaAdjacencias == NULL){
            continue;
        }
        
        arestaGrafo *aresta = vertices[i].listaAdjacencias;
        while(aresta != NULL){
            printf("%s, %s, ", vertices[i].nomeUsuario, aresta->nomeUsuarioDestino);
            
            if(strcmp(aresta->dataInicio, "$$$$$$$$$$") == 0){
                printf("NULO, ");
            } else {
                printf("%s, ", aresta->dataInicio);
            }
            
            if(strcmp(aresta->dataFim, "$$$$$$$$$$") == 0){
                printf("NULO, ");
            } else {
                printf("%s, ", aresta->dataFim);
            }
            
            if(aresta->grauAmizade == '$'){
                printf("NULO\n");
            } else {
                printf("%c\n", aresta->grauAmizade);
            }
            
            aresta = aresta->proxAresta;
        }
        
        printf("\n");
    }
}

void liberarGrafo11(verticeGrafo *vertices, int numVertices){
    for(int i = 0; i < numVertices; i++){
        arestaGrafo *aresta = vertices[i].listaAdjacencias;
        while(aresta != NULL){
            arestaGrafo *proxima = aresta->proxAresta;
            free(aresta);
            aresta = proxima;
        }
    }
    free(vertices);
}