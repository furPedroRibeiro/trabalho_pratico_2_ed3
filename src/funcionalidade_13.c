//Aluno 1: Pedro Luis de Alencar Ribeiro Nº USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda Nº USP: 15443221

#include "../headers/funcionalidade_13.h"
#include "../headers/funcionalidade_11.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"

//FUNCIONALIDADE 13: Caminho mais curto até celebridade
void caminhoMaisCurtoCelebridade(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado, char *nomeUsuarioCelebridade) {
    //Remove as aspas do nome da celebridade
    char celebridade[100];
    strcpy(celebridade, nomeUsuarioCelebridade);
    if(celebridade[0] == '"') {
        int len = strlen(celebridade);
        memmove(celebridade, celebridade + 1, len);
        len = strlen(celebridade);
        if(len > 0 && celebridade[len-1] == '"') {
            celebridade[len-1] = '\0';
        }
    }
    
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
    for(int i = 0; i < qtdIndice; i++) {
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
    
    if(resultados == NULL) {
        printf("Falha na execução da funcionalidade.\n");
        free(vetorIndice);
        fclose(arqSegueOrdenado);
        return;
    }
    
    //Conta quantas pessoas existem
    int numVertices = 0;
    resultadoBusca *atual = resultados;
    while(atual != NULL) {
        numVertices++;
        atual = atual->proxResultado;
    }
    
    //Cria o vetor de vértices
    verticeGrafo *vertices = malloc(numVertices * sizeof(verticeGrafo));
    
    //Preenche o vetor de vértices
    atual = resultados;
    int idx = 0;
    while(atual != NULL) {
        vertices[idx].idPessoa = atual->reg->idPessoa;
        strcpy(vertices[idx].nomeUsuario, atual->reg->nomeUsuario);
        vertices[idx].listaAdjacencias = NULL;
        idx++;
        atual = atual->proxResultado;
    }
    
    //Ordena o vetor de vértices por nomeUsuario
    qsort(vertices, numVertices, sizeof(verticeGrafo), comparaVertices11);
    
    //Carrega todo o arquivo segue ordenado
    int qtdRegistrosSegue;
    fseek(arqSegueOrdenado, 1, SEEK_SET);
    fread(&qtdRegistrosSegue, sizeof(int), 1, arqSegueOrdenado);
    
    noSegue *registrosSegue = (noSegue*)calloc(qtdRegistrosSegue, sizeof(noSegue));
    
    fseek(arqSegueOrdenado, 9, SEEK_SET);
    for(int i = 0; i < qtdRegistrosSegue; i++) {
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
    
    //Encontra o índice da celebridade
    int indiceCelebridade = encontrarIndicePorNome(vertices, numVertices, celebridade);
    
    if(indiceCelebridade == -1) {
        printf("Falha na execução da funcionalidade.\n");
        liberarGrafo11(vertices, numVertices);
        liberarListaResultados(resultados);
        free(vetorIndice);
        for(int i = 0; i < qtdRegistrosSegue; i++) {
            free(registrosSegue[i].dataInicioQueSegue);
            free(registrosSegue[i].dataFimQueSegue);
        }
        free(registrosSegue);
        return;
    }
    
    //Executa BFS para cada pessoa que segue a celebridade (diretamente ou não)
    int primeiraImpressao = 1;
    for(int origem = 0; origem < numVertices; origem++) {
        if(origem == indiceCelebridade) continue;
        
        //Inicializa arrays para BFS
        int *distancia = malloc(numVertices * sizeof(int));
        int *predecessores = malloc(numVertices * sizeof(int));
        arestaGrafo **arestasUsadas = malloc(numVertices * sizeof(arestaGrafo*));
        
        for(int i = 0; i < numVertices; i++) {
            distancia[i] = -1;
            predecessores[i] = -1;
            arestasUsadas[i] = NULL;
        }
        
        //BFS
        Fila *fila = criarFila();
        distancia[origem] = 0;
        enfileirar(fila, origem);
        
        while(!filaVazia(fila)) {
            int u = desenfileirar(fila);
            
            if(u == indiceCelebridade) break;
            
            //Percorre as arestas em ordem alfabética
            arestaGrafo *aresta = vertices[u].listaAdjacencias;
            while(aresta != NULL) {
                int v = encontrarIndicePorNome(vertices, numVertices, aresta->nomeUsuarioDestino);
                
                if(v != -1 && distancia[v] == -1) {
                    distancia[v] = distancia[u] + 1;
                    predecessores[v] = u;
                    arestasUsadas[v] = aresta;
                    enfileirar(fila, v);
                }
                
                aresta = aresta->proxAresta;
            }
        }
        
        liberarFila(fila);
        
        //Verifica se encontrou caminho até a celebridade
        if(distancia[indiceCelebridade] == -1) {
            if(!primeiraImpressao) printf("\n");
            printf("NAO SEGUE A CELEBRIDADE\n");
            primeiraImpressao = 0;
        } else {
            //Reconstrói e imprime o caminho
            nodoCaminho *caminho = reconstruirCaminho(predecessores, arestasUsadas, vertices, origem, indiceCelebridade);
            if(!primeiraImpressao) printf("\n");
            imprimirCaminho(caminho);
            liberarCaminho(caminho);
            primeiraImpressao = 0;
        }
        
        free(distancia);
        free(predecessores);
        free(arestasUsadas);
    }
    
    //Libera memória
    liberarGrafo11(vertices, numVertices);
    liberarListaResultados(resultados);
    free(vetorIndice);
    for(int i = 0; i < qtdRegistrosSegue; i++) {
        free(registrosSegue[i].dataInicioQueSegue);
        free(registrosSegue[i].dataFimQueSegue);
    }
    free(registrosSegue);
}

//Funções auxiliares para a fila
Fila* criarFila() {
    Fila *fila = malloc(sizeof(Fila));
    fila->inicio = NULL;
    fila->fim = NULL;
    return fila;
}

void enfileirar(Fila *fila, int indiceVertice) {
    noFila *novo = malloc(sizeof(noFila));
    novo->indiceVertice = indiceVertice;
    novo->proximo = NULL;
    
    if(fila->fim == NULL) {
        fila->inicio = novo;
        fila->fim = novo;
    } else {
        fila->fim->proximo = novo;
        fila->fim = novo;
    }
}

int desenfileirar(Fila *fila) {
    if(fila->inicio == NULL) return -1;
    
    noFila *temp = fila->inicio;
    int valor = temp->indiceVertice;
    fila->inicio = fila->inicio->proximo;
    
    if(fila->inicio == NULL) {
        fila->fim = NULL;
    }
    
    free(temp);
    return valor;
}

int filaVazia(Fila *fila) {
    return fila->inicio == NULL;
}

void liberarFila(Fila *fila) {
    while(!filaVazia(fila)) {
        desenfileirar(fila);
    }
    free(fila);
}

//Encontra o índice de um vértice pelo nome de usuário
int encontrarIndicePorNome(verticeGrafo *vertices, int numVertices, char *nomeUsuario) {
    for(int i = 0; i < numVertices; i++) {
        if(strcmp(vertices[i].nomeUsuario, nomeUsuario) == 0) {
            return i;
        }
    }
    return -1;
}

//Reconstrói o caminho a partir dos predecessores
nodoCaminho* reconstruirCaminho(int *predecessores, arestaGrafo **arestasUsadas, verticeGrafo *vertices, int indiceOrigem, int indiceDestino) {
    nodoCaminho *caminho = NULL;
    nodoCaminho *ultimo = NULL;
    
    int atual = indiceDestino;
    
    while(atual != indiceOrigem) {
        int pred = predecessores[atual];
        arestaGrafo *aresta = arestasUsadas[atual];
        
        nodoCaminho *novo = malloc(sizeof(nodoCaminho));
        strcpy(novo->nomeUsuarioQueSegue, vertices[pred].nomeUsuario);
        strcpy(novo->nomeUsuarioQueESeguida, vertices[atual].nomeUsuario);
        strcpy(novo->dataInicio, aresta->dataInicio);
        strcpy(novo->dataFim, aresta->dataFim);
        novo->grauAmizade = aresta->grauAmizade;
        novo->proximo = NULL;
        
        //Insere no início (para inverter a ordem)
        if(caminho == NULL) {
            caminho = novo;
            ultimo = novo;
        } else {
            novo->proximo = caminho;
            caminho = novo;
        }
        
        atual = pred;
    }
    
    return caminho;
}

//Imprime o caminho
void imprimirCaminho(nodoCaminho *caminho) {
    nodoCaminho *atual = caminho;
    
    while(atual != NULL) {
        printf("%s, %s, ", atual->nomeUsuarioQueSegue, atual->nomeUsuarioQueESeguida);
        
        if(strcmp(atual->dataInicio, "$$$$$$$$$$") == 0) {
            printf("NULO, ");
        } else {
            printf("%s, ", atual->dataInicio);
        }
        
        if(strcmp(atual->dataFim, "$$$$$$$$$$") == 0) {
            printf("NULO, ");
        } else {
            printf("%s, ", atual->dataFim);
        }
        
        if(atual->grauAmizade == '$') {
            printf("NULO\n");
        } else {
            printf("%c\n", atual->grauAmizade);
        }
        
        atual = atual->proximo;
    }
}

//Libera a memória do caminho
void liberarCaminho(nodoCaminho *caminho) {
    while(caminho != NULL) {
        nodoCaminho *proximo = caminho->proximo;
        free(caminho);
        caminho = proximo;
    }
}