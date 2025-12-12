//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_7.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"


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

// ======================= FUNÇÕES PARA FUNCIONALIDADE 7: =======================

//função auxiliar para atualizar um registro individual
void atualizarRegistroIndividual(FILE *arqPessoa, long int posRegistro, char *nomeCampoAtualiza, char *valorCampoAtualiza, cabecalhoPessoa *cabecalho, indice *vetorIndice, int idPessoaAtual){
  //posiciona no registro
  fseek(arqPessoa, posRegistro, SEEK_SET);
  
  //lê o registro completo
  char removido;
  int tamRegistroAtual;
  fread(&removido, sizeof(char), 1, arqPessoa);
  fread(&tamRegistroAtual, sizeof(int), 1, arqPessoa);
  
  int idPessoa, idadePessoa, tamNomePessoa, tamNomeUsuario;
  char nomePessoa[100] = "";
  char nomeUsuario[100] = "";
  
  fread(&idPessoa, sizeof(int), 1, arqPessoa);
  fread(&idadePessoa, sizeof(int), 1, arqPessoa);
  fread(&tamNomePessoa, sizeof(int), 1, arqPessoa);
  fread(nomePessoa, sizeof(char), tamNomePessoa, arqPessoa);
  nomePessoa[tamNomePessoa] = '\0';
  fread(&tamNomeUsuario, sizeof(int), 1, arqPessoa);
  fread(nomeUsuario, sizeof(char), tamNomeUsuario, arqPessoa);
  nomeUsuario[tamNomeUsuario] = '\0';

  //Aplica a atualização nos valores
  int novoId = idPessoa;
  int novoTamNomePessoa = tamNomePessoa;
  int novoTamNomeUsuario = tamNomeUsuario;
  int novaIdadePessoa = idadePessoa;
  char novoNomePessoa[100];
  char novoNomeUsuario[100];
  
  strcpy(novoNomePessoa, nomePessoa);
  strcpy(novoNomeUsuario, nomeUsuario);

  if(strcmp(nomeCampoAtualiza, "idPessoa") == 0){
    novoId = atoi(valorCampoAtualiza);
    int pos = buscaBinariaVetorIndice(vetorIndice, cabecalho->quantidadePessoas, idPessoa);
    //vai retornar a posição onde o id esta, porque ele existe, então se pos > 0, id existe e retorna a posição correta
    if(pos >= 0){
      vetorIndice[pos].idPessoa = novoId;
    }
  }
  
  if(strcmp(nomeCampoAtualiza, "idadePessoa") == 0){
    if(strcmp(valorCampoAtualiza, "NULO") == 0){
      novaIdadePessoa = -1;
    } else {
      novaIdadePessoa = atoi(valorCampoAtualiza);
    }
  }
  else if(strcmp(nomeCampoAtualiza, "nomePessoa") == 0){
    if(strcmp(valorCampoAtualiza, "NULO") == 0){
      novoTamNomePessoa = 0;
      novoNomePessoa[0] = '\0';
    } else {
      strcpy(novoNomePessoa, valorCampoAtualiza);
      novoTamNomePessoa = strlen(valorCampoAtualiza);
    }
  }
  else if(strcmp(nomeCampoAtualiza, "nomeUsuario") == 0){
    strcpy(novoNomeUsuario, valorCampoAtualiza);
    novoTamNomeUsuario = strlen(valorCampoAtualiza);
  }
  
  //calcula o novo tamanho do registro
  int novoTamRegistro = 16 + novoTamNomePessoa + novoTamNomeUsuario;
  
  if(novoTamRegistro <= tamRegistroAtual){
    //Caso 1: inserir com lixo
    //o novo registro cabe no espaço atual
    
    fseek(arqPessoa, posRegistro + 5, SEEK_SET); //Pula removido e tamRegistro
    
    //escreve os campos atualizados
    fwrite(&novoId, sizeof(int), 1, arqPessoa);
    fwrite(&novaIdadePessoa, sizeof(int), 1, arqPessoa);
    fwrite(&novoTamNomePessoa, sizeof(int), 1, arqPessoa);
    fwrite(novoNomePessoa, sizeof(char), novoTamNomePessoa, arqPessoa);
    fwrite(&novoTamNomeUsuario, sizeof(int), 1, arqPessoa);
    fwrite(novoNomeUsuario, sizeof(char), novoTamNomeUsuario, arqPessoa);
    
    //preenche o resto com lixo '$'
    int bytesEscritos = 16 + novoTamNomePessoa + novoTamNomeUsuario;
    int bytesLixo = tamRegistroAtual - bytesEscritos;
    
    for(int j = 0; j < bytesLixo; j++){
      char lixo = '$';
      fwrite(&lixo, sizeof(char), 1, arqPessoa);
    }
    
    fflush(arqPessoa);
    
  } else {
    //Caso 2: remover logicamente e inserir novo registro
    //o novo registro não cabe no espaço atual
    
    //marca o registro atual como removido
    fseek(arqPessoa, posRegistro, SEEK_SET);
    char marcaRemovido = '1';
    fwrite(&marcaRemovido, sizeof(char), 1, arqPessoa);
    fflush(arqPessoa);
    
    //remove do índice
    for(int j = 0; j < cabecalho->quantidadePessoas; j++){
      if(vetorIndice[j].idPessoa == idPessoaAtual){
        // Desloca todos os elementos para a esquerda
        for(int k = j; k < cabecalho->quantidadePessoas - 1; k++){
          vetorIndice[k] = vetorIndice[k + 1];
        }
        break;
      }
    }
    
    //atualiza quantidadeRemovidos
    cabecalho->quantidadeRemovidos++;
    
    //insere como NOVO REGISTRO no final
    fseek(arqPessoa, cabecalho->proxByteoffset, SEEK_SET);
    
    char removidoNovo = '0';
    fwrite(&removidoNovo, sizeof(char), 1, arqPessoa);
    fwrite(&novoTamRegistro, sizeof(int), 1, arqPessoa);
    fwrite(&novoId, sizeof(int), 1, arqPessoa);
    fwrite(&novaIdadePessoa, sizeof(int), 1, arqPessoa);
    fwrite(&novoTamNomePessoa, sizeof(int), 1, arqPessoa);
    fwrite(novoNomePessoa, sizeof(char), novoTamNomePessoa, arqPessoa);
    
    fwrite(&novoTamNomeUsuario, sizeof(int), 1, arqPessoa);
    fwrite(novoNomeUsuario, sizeof(char), novoTamNomeUsuario, arqPessoa);
    
    //adiciona ao índice na posição ordenada correta
    int pos = buscaBinariaVetorIndice(vetorIndice, cabecalho->quantidadePessoas, idPessoa);
    //vai retornar um valor negativo, já que a busca binária vai retornar a posição onde o id deveria estar, logo precisamos converter pra positivo e subtrair 1
    //exemplo: retorna -3(deveria estar na posição 2), então pos = -pos -1
    if(pos >= 0){
      // puts("ID ja existe");
    } else{
      pos = -(pos) - 1;
    }

    //desloca para abrir espaço
    for(int j = cabecalho->quantidadePessoas; j > pos; j--){
      vetorIndice[j] = vetorIndice[j-1];
    }
    
    //insere o novo índice
    vetorIndice[pos].idPessoa = idPessoa;
    vetorIndice[pos].byteOffset = cabecalho->proxByteoffset;
    
    //atualiza o cabeçalho
    cabecalho->proxByteoffset += novoTamRegistro + 5;
    
    fflush(arqPessoa);
  }
}