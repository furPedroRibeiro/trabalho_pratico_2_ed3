//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/functions.h"
#include "../headers/utilidades.h"



//FUNCIONALIDADE 1:
void criarIndice(char *nomeArquivoIndice){
  //caminho onde os binário devem ser criados:
  char caminho[100] = "./";
  //concatena o caminho com o nome do arquivo
  strcat(caminho, nomeArquivoIndice);
  //o que deve ser feito aqui é a abertura(criação) do arquivo de índice, que nesse momento deve conter apenas o cabeçalho
  //devemos adicionar o lixo ao cabeçalho também
  char lixo[] = "$$$$$$$$$$$";

  //cria o arquivo com o caminho e nome concatenados em strcat
  FILE *arqIndice = fopen(caminho, "wb");
  if(arqIndice == NULL){
    printf("Falha no processamento do arquivo.");
    return;
  }
  //o arquivo em sua criação, tem status e ele está consistente
  defineStatusArquivo(arqIndice, '1');
  //escreve o cabeçalho no arquivo
  fseek(arqIndice, 1, SEEK_SET); //pula o status
  fwrite(lixo, sizeof(char), strlen(lixo), arqIndice);

  //fecha o arquivo
  fclose(arqIndice);

  //usa binario na tela como especificado no trabalho
  binarioNaTela(nomeArquivoIndice);
}

//FUNCIONALIDADE 2:
void criarArquivoDados(char *nomeArquivoEntrada, char *nomeArquivoDados, char *nomeArquivoIndice){
    //testando existência dos arquivos:
    //arquivo de dados será criado aqui
    //cria e abre arquivo pra escrita
    FILE* arqDados = fopen(nomeArquivoDados, "wb");
    if(arqDados == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    } else{
        
    }
    //primeiro já vamos escrever o status para 0
    defineStatusArquivo(arqDados, '0');


    //abre arquivo de índice pra escrita e leitura
    FILE* arqIndice = fopen(nomeArquivoIndice, "rb+");
    if(arqIndice == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    } else{
        
    }

    //testando se csv existe
    FILE *arqEntrada = fopen(nomeArquivoEntrada, "r");
    //se der erro no processamento do arquivo, aparece a seguinte mensagem:
    if(arqEntrada == NULL){
        puts("Falha no processamento do arquivo.");
        fclose(arqDados);
        fclose(arqIndice);
        return;
    } else{
        
    }

    //chama uma função para criar uma estrutura de dados com todos os registros do arquivo de entrada, e cria também a estrutura de dados do indice. A função já faz as inserções no arquivo de dados e no arquivo de índice
    lerCSV(arqDados, arqIndice, arqEntrada);

    // //cria e abre arquivo pra escrita para verificar o byteoffset que está no cabeçalho
    // FILE* arqDadosLeitura = fopen(caminho, "rb");
    // if(arqDadosLeitura == NULL){
    //   puts("Falha no processamento do arquivo.");
    //   return;
    // } else{
        
    // }
    // long int proxByteoffset;
    // fseek(arqDadosLeitura, 1, SEEK_SET);
    // int quantidadePessoas = 0;
    // fread(&quantidadePessoas, sizeof(int), 1, arqDados);
    // int quantidadeRemovidos = 0;
    // fread(&quantidadeRemovidos, sizeof(int), 1, arqDados);
    // fread(&proxByteoffset, sizeof(long int), 1, arqDados);
    // printf("Proximo Byteoffset disponivel: %ld\n", proxByteoffset);
    // printf("Qtd pessoas: %d\n", quantidadePessoas);
    // printf("Qtd removidos: %d\n", quantidadeRemovidos);
    // fclose(arqDadosLeitura);

    //usa função binário na tela como especificado no trabalho
    binarioNaTela(nomeArquivoDados);
    binarioNaTela(nomeArquivoIndice);
}

//FUNCIONALIDADE 3:
//Definição da variável global
//struct registro_2 reg;

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

//FUNCIONALIDADE 4:
void buscarRegistros(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n){
    //abertura dos arquivos
    FILE *arqPessoa = abrirArquivoComStatus(nomeArquivoPessoa, "rb");
    FILE *arquivoIndice = abrirArquivoComStatus(nomeArquivoIndice, "rb");

    //carrega o arquivo de índice em um vetor
    fseek(arquivoIndice, 0, SEEK_END);
    long sizeIndice = ftell(arquivoIndice) - 12; //remove os bytes do cabeçalho

    //calcula quantos registros de índice existem no arquivo
    int qtdIndice = sizeIndice / (sizeof(int) + sizeof(long int));

    //posiciona o ponteiro depois do cabeçalho
    fseek(arquivoIndice, 12, SEEK_SET);

    //aloca memória
    indice *vetorIndice = malloc(qtdIndice * sizeof(indice));
    
    //carrega o índice completo no vetor
    for(int i = 0; i < qtdIndice; i++){
        fread(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fread(&vetorIndice[i].byteOffset, sizeof(long int), 1, arquivoIndice);
    }
    fclose(arquivoIndice);

    //obtem o tamanho do arquivo pessoa e posiciona o ponteiro apos o cabecalho
    fseek(arqPessoa,0, SEEK_END);
    long sizeDados = ftell(arqPessoa);

    //loop de buscas
    for(int i = 0; i < n; i++){
        int entrada;
        char nomeCampo[100], valorCampo[100];

        //le a linha de busca no formato: número nomeCampo=valorCampo
        scanf("%d", &entrada);
        scanf(" %[^=]", nomeCampo);
        getchar();
        scan_quote_string(valorCampo);

        //executa a busca e obtém a lista de resultados
        resultadoBusca *resultados = buscarRegistrosPorCampo(arqPessoa, vetorIndice, qtdIndice, sizeDados, nomeCampo, valorCampo);
        //nenhum registro encontrado
        if (resultados == NULL) {
            printf("Registro inexistente.\n\n");
        } 
        //imprime todos os registros encontrados
        else{
            resultadoBusca *atual = resultados;
            while(atual != NULL){
                imprimirRegistro(atual->reg->idPessoa, atual->reg->idadePessoa, atual->reg->tamanhoNomePessoa, atual->reg->nome, atual->reg->tamanhoNomeUsuario, atual->reg->nomeUsuario); 
                atual = atual->proxResultado;
            }
        }
        //libera memória da lista
        liberarListaResultados(resultados);
    }
    free(vetorIndice);
    fclose(arqPessoa);
}

//FUNCIONALIDADE 5:
void deletarRegistro(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n){
    //Abertura dos arquivos
    FILE *arqPessoa = abrirArquivoComStatus(nomeArquivoPessoa, "rb+");
    FILE *arquivoIndice = abrirArquivoComStatus(nomeArquivoIndice, "rb+");
    
    //Marca os arquivos como inconsistentes durante a operação
    defineStatusArquivo(arqPessoa, '0');
    defineStatusArquivo(arquivoIndice, '0');
    
    //traz o cabeçalho pra memória primária
    cabecalhoPessoa *cabecalho = malloc(sizeof(cabecalhoPessoa));
    fseek(arqPessoa, 0, SEEK_SET);
    fread(&cabecalho->status[0], sizeof(char), 1, arqPessoa);
    fread(&cabecalho->quantidadePessoas, sizeof(int), 1, arqPessoa);
    fread(&cabecalho->quantidadeRemovidos, sizeof(int), 1, arqPessoa);
    fread(&cabecalho->proxByteoffset, sizeof(long int), 1, arqPessoa);
    cabecalho->status[1] = '\0';
    
    fseek(arquivoIndice, 12, SEEK_SET); // pula o cabeçalho para montar vetor de índices
    indice *vetorIndice = malloc(cabecalho->quantidadePessoas * sizeof(indice));
    for(int i = 0; i < cabecalho->quantidadePessoas; i++){
        fread(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fread(&vetorIndice[i].byteOffset, sizeof(long int), 1, arquivoIndice);
    }

    //Vetor para marcar IDs que devem ser removidos
    int *idsParaRemover = malloc(cabecalho->quantidadePessoas * sizeof(int));
    int qtdIdsParaRemover = 0;
    
    //obtém o tamanho do arquivo pessoa
    fseek(arqPessoa, 0, SEEK_END);
    long sizeDados = ftell(arqPessoa);
    
    //loop de remoções
    for(int i = 0; i < n; i++){
        int entrada;
        char nomeCampo[100], valorCampo[100];
        
        scanf("%d", &entrada);
        scanf(" %[^=]", nomeCampo);
        getchar(); // Consome o '='
        
        //função para tratamento de string, foi necessário criar uma nova função pois como os valores podem
        //ser passados no modelo idadePessoa=NULO ou nomePessoa=NULO, a função scan_quote_string não tratava corretamente
        //pois o valor do campo, ou seja o nulo, era recebido sem aspas 
        //verifica se o valor tem aspas ou não
        char c = getchar();
        if(c == '"'){
            //Valor entre aspas - lê até a próxima aspa
            int j = 0;
            while((c = getchar()) != '"' && c != '\n' && c != EOF){
                valorCampo[j++] = c;
            }
            valorCampo[j] = '\0';
        } else {
            //Valor sem aspas - lê até o fim da linha
            valorCampo[0] = c;
            int j = 1;
            while((c = getchar()) != '\n' && c != EOF){
                valorCampo[j++] = c;
            }
            valorCampo[j] = '\0';
        }
        
        //por segurança, trocamos o que recebemos da função acima por ""
        if(strcmp(valorCampo, "NULO") == 0){
            strcpy(valorCampo, "");
        }
        
        //usa a função de busca buscarRegistrosPorCampo
        resultadoBusca *resultados = buscarRegistrosPorCampo(arqPessoa, vetorIndice, cabecalho->quantidadePessoas, sizeDados, nomeCampo, valorCampo);
        
        //Processa todos os registros encontrados
        if(resultados != NULL){
            resultadoBusca *atual = resultados;
            while(atual != NULL){
                //marca o registro como removido no arquivo de dados
                fseek(arqPessoa, atual->byteOffset, SEEK_SET);
                char removido = '1';
                fwrite(&removido, sizeof(char), 1, arqPessoa);
                fflush(arqPessoa);
                
                //guarda os ids que precisam ser removidos do arquivo de indice
                idsParaRemover[qtdIdsParaRemover++] = atual->reg->idPessoa;
                
                atual = atual->proxResultado;
            }
            
            //Libera a lista de resultados
            liberarListaResultados(resultados);
        }
    }
    
    // Remove todos os IDs marcados do vetor de índices
    for(int i = 0; i < qtdIdsParaRemover; i++){
        for(int j = 0; j < cabecalho->quantidadePessoas; j++){
            if(vetorIndice[j].idPessoa == idsParaRemover[i]){
                for(int k = j; k < cabecalho->quantidadePessoas - 1; k++){
                    vetorIndice[k] = vetorIndice[k + 1];
                }
                cabecalho->quantidadePessoas--;
                break;
            }
        }
    }

    // atualiza cabeçalho do arquivo pessoas
    cabecalho->quantidadeRemovidos += qtdIdsParaRemover;

    //escreve cabeçalho do arquivo pessoas atualizado
    fseek(arqPessoa, 1, SEEK_SET);
    fwrite(&cabecalho->quantidadePessoas, sizeof(int), 1, arqPessoa);
    fwrite(&cabecalho->quantidadeRemovidos, sizeof(int), 1, arqPessoa);
    
    // Reescreve o arquivo de índice
    fclose(arquivoIndice);
    remove(nomeArquivoIndice);

    arquivoIndice = fopen(nomeArquivoIndice, "wb+");    
    // Escreve o cabeçalho do índice
    char statusIndice;
    statusIndice = '0';
    fseek(arquivoIndice, 0, SEEK_SET);
    fwrite(&statusIndice, sizeof(char), 1, arquivoIndice);
    char lixo[12] = "$$$$$$$$$$$";
    fwrite(lixo, sizeof(char), 11, arquivoIndice);
    
    // Escreve os índices após o cabeçalho
    for(int i = 0; i < cabecalho->quantidadePessoas; i++){
        fwrite(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fwrite(&vetorIndice[i].byteOffset, sizeof(long int), 1, arquivoIndice);
    }
    
    // Marca os arquivos como consistentes
    defineStatusArquivo(arqPessoa, '1');
    defineStatusArquivo(arquivoIndice, '1');
    
    free(vetorIndice);
    free(idsParaRemover);
    free(cabecalho);

    fclose(arqPessoa);
    fclose(arquivoIndice);
    
    // Mostra os arquivos binários
    binarioNaTela(nomeArquivoPessoa);
    binarioNaTela(nomeArquivoIndice);
}

//FUNCIONALIDADE 6:
void inserirUnicoRegistro(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n){
    //a inserção será feita no final do arquivo, mas precisamos atualizar também o cabeçalho, então o arquivo será aberto com rb+
    FILE *arqDados = fopen(nomeArquivoPessoa, "rb+");
    if(arqDados == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }
    //definindo status como inconsistente:
    defineStatusArquivo(arqDados, '0');
    fseek(arqDados, 0, SEEK_SET);

    //é necessário ler o cabeçalho do arquivo para atualizar o mesmo depois 
    cabecalhoPessoa* cabecalho = lerCabecalho(arqDados);

    //abrindo arquivo de índice, que também deve ser atualizado:
    FILE *arqIndice = fopen(nomeArquivoIndice, "rb+");
    if(arqIndice == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }
    int numRegAtivos = cabecalho->quantidadePessoas;
    //definindo status como inconsistente:
    defineStatusArquivo(arqDados, '1');
    fseek(arqIndice, 0, SEEK_SET);
    //lê o arquivo de índice para memória primária
    indice *indices = lerArquivoIndice(arqIndice, numRegAtivos, n);
    
    //devemos captar agora n entradas do usuário, e a cada entrada é feita uma inserção no arquivo de dados pessoa
    //um while é usado
    int i = 0;
    int nAtual = n;
    while(i < n){
        //é necessário ler o cabeçalho do arquivo para utilizar as informações contidas nele 
        cabecalhoPessoa* cabecalhoAtual = lerCabecalho(arqDados);
        //vamos captar agora mais uma entrada do usuário chamando uma função, que devolve um array nó com os dados a serem inseridos 
        noRegistroUnico *regUnico = lerEntradaInsercaoUnica();
        //depois da entrada ser captada, devemos fazer a inserção no arquivo de dados pessoa
        insereRegistroUnicoPessoa(arqDados, regUnico, cabecalhoAtual);
        //é necessário também inserir no vetor de índices o novo registro
        insereRegistroUnicoVetorIndice(indices, numRegAtivos + i, regUnico->idPessoa, cabecalhoAtual->proxByteoffset);
        //desalocando cabecalho atual para que ele seja lido de novo no começo do while
        free(cabecalhoAtual);
        free(regUnico->nomePessoa);
        free(regUnico->nomeUsuario);
        free(regUnico);
        i++;
    }
    //puts("\nsaiu do loop\n");
    //essa função armazena novamente o arquivo de índice, agora atualizado
    insereIndice(indices, arqIndice, cabecalho->quantidadePessoas + n);
    //depois de toda a funcionalidade ser executada, basta voltar o status consistente para os 2 arquivos e desalocar o que foi usado de memória, além de fechar os 2 arquivos
    free(cabecalho);
    free(indices);

    char statusConsistente = '1';

    fseek(arqDados, 0, SEEK_SET);
    fwrite(&statusConsistente, 1, sizeof(char), arqDados);
    fseek(arqDados, 0, SEEK_SET);
    
    fseek(arqIndice, 0, SEEK_SET);
    fwrite(&statusConsistente, 1, sizeof(char), arqIndice);
    fseek(arqIndice, 0, SEEK_SET);

    fclose(arqDados);
    fclose(arqIndice);
    
    binarioNaTela(nomeArquivoPessoa);
    binarioNaTela(nomeArquivoIndice);
}

//FUNCIONALIDADE 7:
void atualizarRegistro(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n){
    //Abertura dos arquivos
    FILE *arqPessoa = abrirArquivoComStatus(nomeArquivoPessoa, "rb+");
    FILE *arquivoIndice = abrirArquivoComStatus(nomeArquivoIndice, "rb+");
    
    //Marca os arquivos como inconsistentes
    defineStatusArquivo(arqPessoa, '0');
    defineStatusArquivo(arquivoIndice, '0');
    
    //traz o cabeçalho pra memória primária
    cabecalhoPessoa *cabecalho = malloc(sizeof(cabecalhoPessoa));
    fseek(arqPessoa, 0, SEEK_SET);
    fread(&cabecalho->status[0], sizeof(char), 1, arqPessoa);
    fread(&cabecalho->quantidadePessoas, sizeof(int), 1, arqPessoa);
    fread(&cabecalho->quantidadeRemovidos, sizeof(int), 1, arqPessoa);
    fread(&cabecalho->proxByteoffset, sizeof(long int), 1, arqPessoa);
    cabecalho->status[1] = '\0';
    
    
    //Aloca espaço extra para o vetor
    int capacidadeIndice = cabecalho->quantidadePessoas + n + 10;
    indice *vetorIndice = malloc(capacidadeIndice * sizeof(indice));
    
    //Carrega o índice em memória primária
    fseek(arquivoIndice, 12, SEEK_SET); //pula o cabeçalho
    for(int i = 0; i < cabecalho->quantidadePessoas; i++){
        fread(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fread(&vetorIndice[i].byteOffset, sizeof(long int), 1, arquivoIndice);
    }
    
    //obtém o tamanho do arquivo pessoa
    fseek(arqPessoa, 0, SEEK_END);
    long sizeDados = ftell(arqPessoa);
    
    //Loop de atualizações
    for(int i = 0; i < n; i++){
        int entrada;
        char nomeCampoBusca[100], valorCampoBusca[100];
        char nomeCampoAtualiza[100], valorCampoAtualiza[100];
        //lê a linha de busca e atualização
        scanf("%d", &entrada);
        scanf(" %[^=]", nomeCampoBusca);
        getchar(); // Consome o '='
        
        //verifica se o valor tem aspas ou não
        char c = getchar();
        if(c == '"'){
            //Valor entre aspas - lê até a próxima aspa
            int j = 0;
            while((c = getchar()) != '"' && c != '\n' && c != EOF){
                valorCampoBusca[j++] = c;
            }
            valorCampoBusca[j] = '\0';
        } else {
            //Valor sem aspas - lê até o espaço ou fim da linha
            valorCampoBusca[0] = c;
            int j = 1;
            while((c = getchar()) != ' ' && c != '\n' && c != EOF){
                valorCampoBusca[j++] = c;
            }
            valorCampoBusca[j] = '\0';
        }
        
        scanf(" %[^=]", nomeCampoAtualiza);
        getchar(); // Consome o '='
        
        //verifica se o valor tem aspas ou não (campo de atualização)
        c = getchar();
        if(c == '"'){
            //Valor entre aspas - lê até a próxima aspa
            int j = 0;
            while((c = getchar()) != '"' && c != '\n' && c != EOF){
                valorCampoAtualiza[j++] = c;
            }
            valorCampoAtualiza[j] = '\0';
        } else {
            //Valor sem aspas - lê até o fim da linha
            valorCampoAtualiza[0] = c;
            int j = 1;
            while((c = getchar()) != '\n' && c != EOF){
                valorCampoAtualiza[j++] = c;
            }
            valorCampoAtualiza[j] = '\0';
        }
        
        //Trata valores NULO
        if(strcmp(valorCampoBusca, "NULO") == 0){
            strcpy(valorCampoBusca, "");
        }
        if(strcmp(valorCampoAtualiza, "NULO") == 0){
            strcpy(valorCampoAtualiza, "");
        }

        //usa a função de busca
        resultadoBusca *resultados = buscarRegistrosPorCampo(arqPessoa, vetorIndice, cabecalho->quantidadePessoas, sizeDados, nomeCampoBusca, valorCampoBusca);
        
        //Processa todos os registros encontrados
        if(resultados != NULL){
            resultadoBusca *atual = resultados;
            while(atual != NULL){
                //chama a função que atualiza os registros individualmente
                atualizarRegistroIndividual(arqPessoa, atual->byteOffset, nomeCampoAtualiza, valorCampoAtualiza, cabecalho, vetorIndice, atual->reg->idPessoa);
                atual = atual->proxResultado;
            }
            //Libera a lista de resultados
            liberarListaResultados(resultados);
        }
        //atualiza sizeDados após processar todos os resultados desta iteração
        fseek(arqPessoa, 0, SEEK_END);
        sizeDados = ftell(arqPessoa);
    }

    //reescreve o arquivo de índice
    fclose(arquivoIndice);
    remove(nomeArquivoIndice);
    
    arquivoIndice = fopen(nomeArquivoIndice, "wb+");
    
    //escreve o cabeçalho do índice
    char statusIndice = '0';
    fseek(arquivoIndice, 0, SEEK_SET);
    fwrite(&statusIndice, sizeof(char), 1, arquivoIndice);
    char lixo[12] = "$$$$$$$$$$$";
    fwrite(lixo, sizeof(char), 11, arquivoIndice);
    
    //escreve os índices
    for(int i = 0; i < cabecalho->quantidadePessoas; i++){
        fwrite(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fwrite(&vetorIndice[i].byteOffset, sizeof(long int), 1, arquivoIndice);
    }
    
    //atualiza o cabeçalho do arquivo pessoa
    fseek(arqPessoa, 1, SEEK_SET);
    fwrite(&cabecalho->quantidadePessoas, sizeof(int), 1, arqPessoa);
    fwrite(&cabecalho->quantidadeRemovidos, sizeof(int), 1, arqPessoa);
    fwrite(&cabecalho->proxByteoffset, sizeof(long int), 1, arqPessoa);
    
    //marca os arquivos como consistentes
    defineStatusArquivo(arqPessoa, '1');
    defineStatusArquivo(arquivoIndice, '1');
    
    free(vetorIndice);
    free(cabecalho);
    fclose(arqPessoa);
    fclose(arquivoIndice);
    
    binarioNaTela(nomeArquivoPessoa);
    binarioNaTela(nomeArquivoIndice);
}

// FUNCIONALIDADE 8:
void criaArquivoSegue(char *nomeArquivoEntradaSegue, char *nomeArquivoSaidaSegue){
    //para cada linha lida no arquivo csv, temos uma inserção no arquivo segue
    //abrindo arquivo de entrada csv apenas para leitura
    FILE *arqEntrada = fopen(nomeArquivoEntradaSegue, "r");
    if(arqEntrada == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }
    //abrindo arquivo segue(binário) para escrita+leitura, já que precisamos atualizar o cabeçalho a cada inserção, então o ponteiro do arquivo vai pra lá e pra cá:
    FILE *arqSegue = fopen(nomeArquivoSaidaSegue, "wb+");
    if(arqSegue == NULL){
        puts("Falha no processamento do arquivo.");
        fclose(arqEntrada);
        return;
    }

    char bufferLinha[1024];
    //a leitura no csv vai começar:
    fgets(bufferLinha, sizeof(bufferLinha), arqEntrada); //pula a primeira linha, que é: idPessoaQueSegue,idPessoaQueESeguida,dataInicioQueSegue,dataFimQueSegue,grauAmizade

    //definindo variáveis do cabeçalho:
    char status = '0';
    int quantidadeRegistros = 0;
    int proxRRN = 0;
    int proxByteOffset = 0;

    criaCabecalhoArquivoSegue(arqSegue, status, quantidadeRegistros, proxByteOffset); //função que escreve cabeçalho do arquivo segue

    //pulando o cabeçalho
    proxByteOffset = 9;
    //proxRRN começa em 0:
    proxRRN = 0;

    //lê até a última linha do arquivo csv, o formato de uma linha é: 186,117,03/05/2020,23/09/2020,0
    while(fgets(bufferLinha, sizeof(bufferLinha), arqEntrada) != NULL){
        noSegue *noAtual = malloc(sizeof(noSegue)); //criando um nó para o registro lido do csv
        noAtual->dataInicioQueSegue = malloc(11 * sizeof(char)); //alocando memória para a string de data de inicio
        noAtual->dataFimQueSegue = malloc(11 * sizeof(char)); //alocando memória para a string de data de fim

        //removendo \n do fim da string
        bufferLinha[strcspn(bufferLinha, "\n")] = '\0';

        //criando ponteiro que aponta para o buffer que armazena a linha(necessário para strsep)
        char *str = bufferLinha;

        noAtual->removido[0] = '0'; //quando um registro é criado, ele não está marcado como logicamente removido
        noAtual->removido[1] = '\0';
        //separa a linha lida em campos do registro segue
        char *idQueSegue = meu_strsep(&str, ",");
        char *idQueSeguida = meu_strsep(&str, ",");
        char *dataInicio = meu_strsep(&str, ",");
        char *dataFim = meu_strsep(&str, ",");
        char *grau = meu_strsep(&str, ",");

        noAtual->idPessoaQueSegue = atoi(idQueSegue); //converte id que segue
        noAtual->idPessoaQueESeguida = atoi(idQueSeguida); //converte id que é seguida

        //verifica se data de inicio é diferente de null, se for escreve a data lida, se não escreve lixo
        if(dataInicio != NULL && dataInicio[0] != '\0'){
            strcpy(noAtual->dataInicioQueSegue, dataInicio);
        } else{
            strcpy(noAtual->dataInicioQueSegue, "$$$$$$$$$$");
        }
        //verifica se data de fim é diferente de null, se for escreve a data lida, se não escreve lixo
        if(dataFim != NULL && dataFim[0] != '\0'){
            strcpy(noAtual->dataFimQueSegue, dataFim);
        } else{
            strcpy(noAtual->dataFimQueSegue, "$$$$$$$$$$");
        }
        //se o grau lido for diferente de nulo, escrevemos ele, se não escreve lixo
        if(grau != NULL && grau[0] != '\0'){
            // Remove espaços e \n
            while(*grau && isspace((unsigned char)*grau)) grau++;
            noAtual->grauAmizade[0] = grau[0];
        } else{
            noAtual->grauAmizade[0] = '$';
        }
        noAtual->grauAmizade[1] = '\0';

        // printf("DEBUG - Registro %d:\n", quantidadeRegistros + 1);
        // printf("  removido: '%c'\n", noAtual->removido[0]);
        // printf("  idQueSegue: %d\n", noAtual->idPessoaQueSegue);
        // printf("  idQueSeguida: %d\n", noAtual->idPessoaQueESeguida);
        // printf("  dataInicio: '%.10s'\n", noAtual->dataInicioQueSegue);
        // printf("  dataFim: '%.10s'\n", noAtual->dataFimQueSegue);
        // printf("  grau: '%c'\n", noAtual->grauAmizade[0]);
        // printf("  proxByteOffset: %d\n\n", proxByteOffset);

        fseek(arqSegue, proxByteOffset, SEEK_SET); //posiciona o ponteiro no byteoffset disponível
        //chama a função de inserir no arquivo, passando o nó e o arquivo
        insereArquivoSegue(arqSegue, noAtual);

        //feitas as verificações e leituras:
        quantidadeRegistros++;
        proxRRN++;
        proxByteOffset += 30; //todo registro tem o mesmo tamanho no arquivo binário segue

        //agora atualiza o cabeçalho:
        fseek(arqSegue, 1, SEEK_SET); //posiciona o ponteiro em qtdregistros
        fwrite(&quantidadeRegistros, sizeof(int), 1, arqSegue);
        fwrite(&proxRRN, sizeof(int), 1, arqSegue);
        fflush(arqSegue); //força escrita

        //a cada loop, desaloca a memória do nó e das strings para usar novamente
        free(noAtual->dataInicioQueSegue);
        free(noAtual->dataFimQueSegue);
        free(noAtual);
    }

    //quando acabar, atualiza status, fecha o arquivo e executa binário na tela:

    //printando variáveis do cabeçalho pra ver se deu certo:
    // printf("\n\nquantidade pessoas: %d\nproxRRN: %d\n", quantidadeRegistros, proxRRN);

    defineStatusArquivo(arqSegue, '1');

    fclose(arqSegue);

    binarioNaTela(nomeArquivoSaidaSegue);
}

//FUNCIONALIDADE 9:
void ordenaArquivoSegue(char *nomeArquivoDesordenado, char *nomeArquivoOrdenado){
    //primeiro é necessário trazer o arquivo todo para memória primária
    //abre o arquivo desordenado:
    FILE *arqDesordenado = fopen(nomeArquivoDesordenado, "rb+");
    if(arqDesordenado == NULL){
        puts("Falha no processamento do arquivo.");
        free(arqDesordenado);
        return;
    }
    //escreve o status pra 0
    defineStatusArquivo(arqDesordenado, '0');


    //lê a quantidade de registros para alocar um vetor com o tamanho certo
    int qtdRegistros;
    fseek(arqDesordenado, 1, SEEK_SET);
    fread(&qtdRegistros, sizeof(int), 1, arqDesordenado);

    //debug ->printa se leu a quantidade de registros certa:
    // printf("Qtd registros lido: %d\n", qtdRegistros);
    
    //será criado um vetor da estrutura de dados do tipo noSegue para ler todos os registros
    noSegue *registros = (noSegue*)calloc(qtdRegistros, sizeof(noSegue));
    
    //o nó tem essa estrutura:
    // char removido[2];
    // int idPessoaQueSegue;
    // int idPessoaQueESeguida;
    // char *dataInicioQueSegue;
    // char *dataFimQueSegue;
    // char grauAmizade[2];

    //loop para ler todos os registros do arquivo segue
    fseek(arqDesordenado, 9, SEEK_SET); // coloca o byteoffset no primeiro registro
    for(int i = 0; i < qtdRegistros; i++){
        registros[i].dataInicioQueSegue = malloc(11 * sizeof(char));
        registros[i].dataFimQueSegue = malloc(11 * sizeof(char));

        fread(registros[i].removido, sizeof(char), 1, arqDesordenado);
        fread(&registros[i].idPessoaQueSegue, sizeof(int), 1, arqDesordenado);
        fread(&registros[i].idPessoaQueESeguida, sizeof(int), 1, arqDesordenado);
        fread(registros[i].dataInicioQueSegue, sizeof(char), 10, arqDesordenado);
        registros[i].dataInicioQueSegue[10] = '\0';
        fread(registros[i].dataFimQueSegue, sizeof(char), 10, arqDesordenado);
        registros[i].dataFimQueSegue[10] = '\0';
        fread(registros[i].grauAmizade, sizeof(char), 1, arqDesordenado);
    
        // //printa um nó para ver se deu certo:
        // printf("\nRemovido: '%c'", registros[i].removido[0]);
        // printf("\nId pessoa que segue: %d", registros[i].idPessoaQueSegue);
        // printf("\nId pessoa que e seguida: %d", registros[i].idPessoaQueESeguida);
        // printf("\nData inicio que segue: '%s'", registros[i].dataInicioQueSegue);
        // printf("\nData fim que segue: '%s'", registros[i].dataFimQueSegue);
        // printf("\nRemovido: '%c'", registros[i].grauAmizade[0]);
    }

    //depois de tudo lido, é preciso que a ordenação seja feita para que seja escrito tudo no arquivo de novo
    qsort(registros, qtdRegistros, sizeof(noSegue), comparaParaOrdenar);

    //abre arquivo segue ordenado para escrita:
    FILE *arqOrdenado = fopen(nomeArquivoOrdenado, "wb+");
    if(arqOrdenado == NULL){
        puts("Falha no processamento do arquivo.");
        return;
    }
    escreveSegueOrdenado(arqOrdenado, qtdRegistros, registros);//função que escreve tudo no arquivo de dados segue ordenado

    //atualiza status
    defineStatusArquivo(arqOrdenado, '1');
    defineStatusArquivo(arqDesordenado, '1');

    //fecha arquivos e mostra binário na tela:
    fclose(arqDesordenado); //fecha arquivo desordenado
    fclose(arqOrdenado); //fecha arquivo ordenado
    binarioNaTela(nomeArquivoOrdenado);//binário na tela do arquivo ordenado


    //desalocando memória usada para os nós
    for (int i = 0; i < qtdRegistros; i++)
    {
        free(registros[i].dataInicioQueSegue);
        free(registros[i].dataFimQueSegue);
    }
    free(registros);
}

//FUNCIONALIDADE 10
void juncaoArquivos(char *nomeArquivoPessoa, char *nomeArquivoIndice, char *nomeArquivoOrdenado, int n){
    //Abertura dos arquivos
    FILE *arqPessoa = abrirArquivoComStatus(nomeArquivoPessoa, "rb");
    FILE *arquivoIndice = abrirArquivoComStatus(nomeArquivoIndice, "rb");
    FILE *arqOrdenado = abrirArquivoComStatus(nomeArquivoOrdenado, "rb");
    
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
    
    //Carrega todo o arquivo segue ordenado em memória
    int qtdRegistrosSegue;
    fseek(arqOrdenado, 1, SEEK_SET);
    fread(&qtdRegistrosSegue, sizeof(int), 1, arqOrdenado);
    
    noSegue *registrosSegue = (noSegue*)calloc(qtdRegistrosSegue, sizeof(noSegue));
    
    fseek(arqOrdenado, 9, SEEK_SET);
    for(int i = 0; i < qtdRegistrosSegue; i++){
        registrosSegue[i].dataInicioQueSegue = malloc(11 * sizeof(char));
        registrosSegue[i].dataFimQueSegue = malloc(11 * sizeof(char));
        
        fread(registrosSegue[i].removido, sizeof(char), 1, arqOrdenado);
        fread(&registrosSegue[i].idPessoaQueSegue, sizeof(int), 1, arqOrdenado);
        fread(&registrosSegue[i].idPessoaQueESeguida, sizeof(int), 1, arqOrdenado);
        fread(registrosSegue[i].dataInicioQueSegue, sizeof(char), 10, arqOrdenado);
        registrosSegue[i].dataInicioQueSegue[10] = '\0';
        fread(registrosSegue[i].dataFimQueSegue, sizeof(char), 10, arqOrdenado);
        registrosSegue[i].dataFimQueSegue[10] = '\0';
        fread(registrosSegue[i].grauAmizade, sizeof(char), 1, arqOrdenado);
        registrosSegue[i].grauAmizade[1] = '\0';
    }
    fclose(arqOrdenado);
    
    //Obtém o tamanho do arquivo pessoa
    fseek(arqPessoa, 0, SEEK_END);
    long sizeDados = ftell(arqPessoa);
    
    //Loop de buscas
    for(int i = 0; i < n; i++){
        int entrada;
        char nomeCampo[100], valorCampo[100];
        
        // Lê a linha de busca
        scanf("%d", &entrada);
        scanf(" %[^=]", nomeCampo);
        getchar(); // Consome o '='
        scan_quote_string(valorCampo);
        
        //usa a função de busca 
        resultadoBusca *resultados = buscarRegistrosPorCampo(arqPessoa, vetorIndice, qtdIndice, sizeDados, nomeCampo, valorCampo);
        
        //nenhum registro encontrado
        if(resultados == NULL){
            printf("Registro inexistente.\n");
        }
        else{
            //percorre todos os resultados encontrados
            resultadoBusca *atual = resultados;
            while(atual != NULL){
                //chama a função imprimir junção
                imprimirJuncao(atual->reg->idPessoa, atual->reg->idadePessoa, atual->reg->tamanhoNomePessoa, atual->reg->nome,  atual->reg->tamanhoNomeUsuario, atual->reg->nomeUsuario,  registrosSegue, qtdRegistrosSegue, atual->reg->idPessoa);
                atual = atual->proxResultado;
            }
        }
        
        //Libera memória da lista de resultados
        liberarListaResultados(resultados);
    }
    
    //Libera memória
    free(vetorIndice);
    for(int i = 0; i < qtdRegistrosSegue; i++){
        free(registrosSegue[i].dataInicioQueSegue);
        free(registrosSegue[i].dataFimQueSegue);
    }
    free(registrosSegue);
    fclose(arqPessoa);
}