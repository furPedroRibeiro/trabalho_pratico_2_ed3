//Aluno 1: Pedro Luis de Alencar Ribeiro Nº USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda Nº USP: 15443221

#include "../headers/funcionalidade_14.h"
#include "../headers/funcionalidade_11.h"
#include "../headers/funcionalidade_13.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"


//FUNCIONALIDADE 14:
void comprimentoCaminhoFofoca(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado, char *nomeUsuarioQueGerouFofoca){
    //Remove as aspas do nome do usuário
    char usuario[100];
    strcpy(usuario, nomeUsuarioQueGerouFofoca);
    
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
    
    //Busca todos os registros não removidos
    resultadoBusca *resultados = buscarRegistrosPorCampo(arqPessoa, vetorIndice, qtdIndice, sizeDados, NULL, NULL);
    fclose(arqPessoa);
    
    if(resultados == NULL){
        printf("Falha na execução da funcionalidade.\n");
        free(vetorIndice);
        fclose(arqSegueOrdenado);
        return;
    }
    
    //Conta quantas pessoas existem
    int numVertices = 0;
    resultadoBusca *atual = resultados;
    while(atual != NULL){
        numVertices++;
        atual = atual->proxResultado;
    }
    
    //Cria o vetor de vértices
    verticeGrafo *vertices = malloc(numVertices * sizeof(verticeGrafo));
    
    //Preenche o vetor de vértices
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
    qsort(vertices, numVertices, sizeof(verticeGrafo), comparaVertices);
    
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
    construirListas(vertices, numVertices, registrosSegue, qtdRegistrosSegue, resultados);
    
    //Encontra o índice do usuário que gerou a fofoca
    int indiceOrigem = encontrarIndicePorNome(vertices, numVertices, usuario);
    
    if(indiceOrigem == -1){
        printf("Falha na execução da funcionalidade.\n");
        liberarGrafo(vertices, numVertices);
        liberarListaResultados(resultados);
        free(vetorIndice);
        for(int i = 0; i < qtdRegistrosSegue; i++){
            free(registrosSegue[i].dataInicioQueSegue);
            free(registrosSegue[i].dataFimQueSegue);
        }
        free(registrosSegue);
        return;
    }
    
    //Executa BFS para encontrar o menor caminho de volta à origem
    int comprimento = bfsCiclo(vertices, numVertices, indiceOrigem);
    
    if(comprimento == -1){
        printf("A FOFOCA NAO RETORNOU\n");
    } else {
        printf("%d\n", comprimento);
    }
    
    //Libera memória
    liberarGrafo(vertices, numVertices);
    liberarListaResultados(resultados);
    free(vetorIndice);
    for(int i = 0; i < qtdRegistrosSegue; i++){
        free(registrosSegue[i].dataInicioQueSegue);
        free(registrosSegue[i].dataFimQueSegue);
    }
    free(registrosSegue);
}


// ======================= FUNÇÕES PARA A FUNCIONALIDADE 14: =======================

//BFS para encontrar ciclo - retorna o comprimento ou -1 se não houver ciclo
int bfsCiclo(verticeGrafo *vertices, int numVertices, int indiceOrigem){
    int *distancia = malloc(numVertices * sizeof(int));
    int *visitado = malloc(numVertices * sizeof(int));
    
    for(int i = 0; i < numVertices; i++){
        distancia[i] = -1;
        visitado[i] = 0;
    }
    
    Fila *fila = criarFila();
    
    //Começa pelos vizinhos da origem (não pela origem)
    arestaGrafo *aresta = vertices[indiceOrigem].listaAdjacencias;
    int primeiraIteracao = 1;
    
    while(aresta != NULL){
        int v = encontrarIndicePorNome(vertices, numVertices, aresta->nomeUsuarioDestino);
        
        if(v != -1 && !visitado[v]){
            distancia[v] = 1;
            visitado[v] = 1;
            enfileirar(fila, v);
        }
        
        aresta = aresta->proxAresta;
    }
    
    int menorCiclo = -1;
    
    while(!filaVazia(fila)){
        int u = desenfileirar(fila);
        
        //Percorre as arestas em ordem alfabética
        arestaGrafo *arestaAtual = vertices[u].listaAdjacencias;
        
        while(arestaAtual != NULL){
            int v = encontrarIndicePorNome(vertices, numVertices, arestaAtual->nomeUsuarioDestino);
            
            if(v != -1){
                //Se encontrou a origem, temos um ciclo
                if(v == indiceOrigem){
                    int ciclo = distancia[u] + 1;
                    if(menorCiclo == -1 || ciclo < menorCiclo){
                        menorCiclo = ciclo;
                    }
                    //Encontrou o primeiro ciclo, retorna
                    free(distancia);
                    free(visitado);
                    liberarFila(fila);
                    return menorCiclo;
                }
                
                //Se ainda não visitou, adiciona à fila
                if(!visitado[v]){
                    distancia[v] = distancia[u] + 1;
                    visitado[v] = 1;
                    enfileirar(fila, v);
                }
            }
            
            arestaAtual = arestaAtual->proxAresta;
        }
    }
    
    free(distancia);
    free(visitado);
    liberarFila(fila);
    
    return menorCiclo;
}