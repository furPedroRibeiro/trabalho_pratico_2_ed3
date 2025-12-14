//Aluno 1: Pedro Luis de Alencar Ribeiro Nº USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda Nº USP: 15443221

#include "../headers/funcionalidade_13.h"
#include "../headers/funcionalidade_11.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"

//FUNCIONALIDADE 13:
void encontrarCaminhosMaisCurtos(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoSegueOrdenado, char *nomeUsuarioCelebridade){
    //Remove as aspas do nome da celebridade
    char celebridade[100];
    strcpy(celebridade, nomeUsuarioCelebridade);
    
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
    
    //Constrói grafo considerando APENAS relacionamentos ativos (dataFim = NULO)
    construirGrafoAtivo(vertices, numVertices, registrosSegue, qtdRegistrosSegue, resultados);
    
    //Encontra o índice da celebridade
    int indiceCelebridade = buscarIndicePorNome(vertices, numVertices, celebridade);
    
    if(indiceCelebridade == -1){
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
    
    //Executa BFS para cada pessoa que segue alguém (tem relacionamentos ativos)
    for(int origem = 0; origem < numVertices; origem++){
        if(origem == indiceCelebridade) continue;
        
        //Verifica se a pessoa segue alguém (tem pelo menos um relacionamento ativo)
        if(vertices[origem].listaAdjacencias == NULL) continue;
        
        //Inicializa arrays para BFS
        int *distancia = malloc(numVertices * sizeof(int));
        int *predecessores = malloc(numVertices * sizeof(int));
        arestaGrafo **arestasUsadas = malloc(numVertices * sizeof(arestaGrafo*));
        int *visitado = malloc(numVertices * sizeof(int));
        
        for(int i = 0; i < numVertices; i++){
            distancia[i] = -1;
            predecessores[i] = -1;
            arestasUsadas[i] = NULL;
            visitado[i] = 0;
        }
        
        //BFS usando fila para garantir que processa nível por nível
        fila *f = criarFila();
        distancia[origem] = 0;
        visitado[origem] = 1;
        enfileirar(f, origem);
        
        int encontrou = 0;
        
        while(!filaVazia(f) && !encontrou){
            //Pega o tamanho atual da fila (todos os vértices deste nível)
            int tamanhoNivel = 0;
            noFila *temp = f->inicio;
            while(temp != NULL){
                tamanhoNivel++;
                temp = temp->prox;
            }
            
            //Coleta todos os vértices do nível atual
            int *verticesNivel = malloc(tamanhoNivel * sizeof(int));
            for(int i = 0; i < tamanhoNivel; i++){
                verticesNivel[i] = desenfileirar(f);
            }
            
            //Ordena os vértices do nível por índice (ordem alfabética)
            for(int i = 0; i < tamanhoNivel - 1; i++){
                for(int j = i + 1; j < tamanhoNivel; j++){
                    if(verticesNivel[i] > verticesNivel[j]){
                        int aux = verticesNivel[i];
                        verticesNivel[i] = verticesNivel[j];
                        verticesNivel[j] = aux;
                    }
                }
            }
            
            //Processa cada vértice do nível em ordem alfabética
            for(int i = 0; i < tamanhoNivel && !encontrou; i++){
                int u = verticesNivel[i];
                
                if(u == indiceCelebridade){
                    encontrou = 1;
                    break;
                }
                
                //Processa todos os vizinhos de u
                arestaGrafo *aresta = vertices[u].listaAdjacencias;
                while(aresta != NULL){
                    int v = buscarIndicePorNome(vertices, numVertices, aresta->nomeUsuarioDestino);
                    
                    if(v != -1 && !visitado[v]){
                        visitado[v] = 1;
                        distancia[v] = distancia[u] + 1;
                        predecessores[v] = u;
                        arestasUsadas[v] = aresta;
                        enfileirar(f, v);
                        
                        if(v == indiceCelebridade){
                            encontrou = 1;
                            break;
                        }
                    }
                    
                    aresta = aresta->proxAresta;
                }
            }
            
            free(verticesNivel);
        }
        
        liberarFila(f);
        
        //Verifica se encontrou caminho até a celebridade
        if(distancia[indiceCelebridade] == -1){
            printf("NAO SEGUE A CELEBRIDADE\n");
        } else {
            //Reconstrói e imprime o caminho
            imprimirCaminho(predecessores, arestasUsadas, vertices, origem, indiceCelebridade);
        }
        
        //Sempre imprime linha em branco após cada resultado
        printf("\n");
        
        free(distancia);
        free(predecessores);
        free(arestasUsadas);
        free(visitado);
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


// ======================= FUNÇÕES PARA A FUNCIONALIDADE 13: =======================

//Constrói grafo considerando APENAS relacionamentos ativos (dataFim = NULO)
void construirGrafoAtivo(verticeGrafo *vertices, int numVertices, noSegue *registrosSegue, int qtdRegistrosSegue, resultadoBusca *resultados){
    
    for(int i = 0; i < numVertices; i++){
        int idPessoaQueSegue = vertices[i].idPessoa;
        
        //Conta relacionamentos ativos
        int numArestas = 0;
        for(int j = 0; j < qtdRegistrosSegue; j++){
            if(registrosSegue[j].idPessoaQueSegue == idPessoaQueSegue && 
               registrosSegue[j].removido[0] == '0' &&
               strcmp(registrosSegue[j].dataFimQueSegue, "$$$$$$$$$$") == 0){
                numArestas++;
            }
        }
        
        if(numArestas == 0){
            vertices[i].listaAdjacencias = NULL;
            continue;
        }
        
        //Cria array temporário para ordenar
        arestaGrafo *arestasTemp = malloc(numArestas * sizeof(arestaGrafo));
        int idxAresta = 0;
        
        for(int j = 0; j < qtdRegistrosSegue; j++){
            if(registrosSegue[j].idPessoaQueSegue == idPessoaQueSegue && 
               registrosSegue[j].removido[0] == '0' &&
               strcmp(registrosSegue[j].dataFimQueSegue, "$$$$$$$$$$") == 0){
                
                char *nomeUsuarioSeguido = buscarNomeUsuarioPorId(registrosSegue[j].idPessoaQueESeguida, resultados);
                
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
        
        numArestas = idxAresta;
        
        //Ordena as arestas
        qsort(arestasTemp, numArestas, sizeof(arestaGrafo), comparaArestas);
        
        //Constrói lista encadeada
        for(int k = 0; k < numArestas; k++){
            arestaGrafo *novaAresta = malloc(sizeof(arestaGrafo));
            strcpy(novaAresta->nomeUsuarioDestino, arestasTemp[k].nomeUsuarioDestino);
            strcpy(novaAresta->dataInicio, arestasTemp[k].dataInicio);
            strcpy(novaAresta->dataFim, arestasTemp[k].dataFim);
            novaAresta->grauAmizade = arestasTemp[k].grauAmizade;
            novaAresta->proxAresta = NULL;
            
            if(vertices[i].listaAdjacencias == NULL){
                vertices[i].listaAdjacencias = novaAresta;
            } else {
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

//Funções auxiliares para a fila
fila* criarFila(){
    fila *f = malloc(sizeof(fila));
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

void enfileirar(fila *f, int indiceVertice){
    noFila *novo = malloc(sizeof(noFila));
    novo->idxVertice = indiceVertice;
    novo->prox = NULL;
    
    if(f->fim == NULL){
        f->inicio = novo;
        f->fim = novo;
    } else {
        f->fim->prox = novo;
        f->fim = novo;
    }
}

int desenfileirar(fila *f){
    if(f->inicio == NULL) return -1;
    
    noFila *temp = f->inicio;
    int valor = temp->idxVertice;
    f->inicio = f->inicio->prox;
    
    if(f->inicio == NULL){
        f->fim = NULL;
    }
    
    free(temp);
    return valor;
}

int filaVazia(fila *f){
    return f->inicio == NULL;
}

void liberarFila(fila *f){
    while(!filaVazia(f)){
        desenfileirar(f);
    }
    free(f);
}

//Encontra o índice de um vértice pelo nome de usuário
int buscarIndicePorNome(verticeGrafo *vertices, int numVertices, char *nomeUsuario){
    for(int i = 0; i < numVertices; i++){
        if(strcmp(vertices[i].nomeUsuario, nomeUsuario) == 0){
            return i;
        }
    }
    return -1;
}

//Alias para compatibilidade com outras funcionalidades
int encontrarIndicePorNome(verticeGrafo *vertices, int numVertices, char *nomeUsuario){
    return buscarIndicePorNome(vertices, numVertices, nomeUsuario);
}

//Imprime o caminho reconstruindo a partir dos predecessores
void imprimirCaminho(int *predecessores, arestaGrafo **arestasUsadas, verticeGrafo *vertices, int indiceOrigem, int indiceDestino){
    //Reconstrói o caminho do destino para a origem
    int *pilhaCaminho = malloc(1000 * sizeof(int));
    int tamPilha = 0;
    
    int idx = indiceDestino;
    while(idx != indiceOrigem){
        pilhaCaminho[tamPilha++] = idx;
        idx = predecessores[idx];
    }
    pilhaCaminho[tamPilha++] = indiceOrigem;
    
    //Imprime o caminho da origem para o destino
    for(int i = tamPilha - 1; i > 0; i--){
        int u = pilhaCaminho[i];
        int v = pilhaCaminho[i - 1];
        arestaGrafo *aresta = arestasUsadas[v];
        
        printf("%s, %s, ", vertices[u].nomeUsuario, vertices[v].nomeUsuario);
        
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
    }
    
    free(pilhaCaminho);
}

//Função de comparação para ordenar arestas
int comparaArestasOrdenadas(const void *a, const void *b){
    const arestaOrdenada *aA = (const arestaOrdenada*)a;
    const arestaOrdenada *aB = (const arestaOrdenada*)b;
    
    //Primeiro ordena por nome de usuário
    int cmpNome = strcmp(aA->nomeUsuarioDestino, aB->nomeUsuarioDestino);
    if(cmpNome != 0){
        return cmpNome;
    }
    
    //Se nomes iguais, ordena por dataInicio
    int cmpDataInicio = strcmp(aA->dataInicio, aB->dataInicio);
    if(cmpDataInicio != 0){
        return cmpDataInicio;
    }
    
    //Se dataInicio igual, ordena por dataFim
    return strcmp(aA->dataFim, aB->dataFim);
}

//Função auxiliar para comparar duas arestas do grafo
int comparaArestasGrafo(arestaGrafo *a, arestaGrafo *b){
    //Compara nome do destino
    int cmpNome = strcmp(a->nomeUsuarioDestino, b->nomeUsuarioDestino);
    if(cmpNome != 0) return cmpNome;
    
    //Compara data início
    int cmpDataInicio = strcmp(a->dataInicio, b->dataInicio);
    if(cmpDataInicio != 0) return cmpDataInicio;
    
    //Compara data fim
    return strcmp(a->dataFim, b->dataFim);
}