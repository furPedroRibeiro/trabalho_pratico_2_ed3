//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_10.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"


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


// ======================= FUNÇÕES PARA A FUNCIONALIDADE 10: =======================

//Busca binária modificada que retorna a primeira ocorrência do idPessoaQueSegue
long int buscaBinariaSegueModificada(noSegue *registros, int tamanho, int idPessoaBuscado){
  if(tamanho == 0) return -1;
  
  int inicio = 0;
  int fim = tamanho - 1;
  int resultado = -1;
  
  //Busca binária
  while(inicio <= fim){
    int meio = inicio + (fim - inicio) / 2;
    
    if(registros[meio].idPessoaQueSegue == idPessoaBuscado){
      resultado = meio;
      //continua buscando para a esquerda para achar a primeira ocorrência
      fim = meio - 1;
    }
    else if(registros[meio].idPessoaQueSegue < idPessoaBuscado){
      inicio = meio + 1;
    }
    else{
      fim = meio - 1;
    }
  }
  
  return resultado;
}

//função auxiliar para converter grau de amizade em string
char* interpretaGrauAmizade(char grau){
  if(grau == '0'){
    return "celebridade";
  }
  if(grau == '1'){
    return "amiga de minha amiga";
  }
  if(grau == '2'){
    return "minha amiga";
  }
  return "-";
}

//Função para imprimir a junção completa
void imprimirJuncao(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario, noSegue *registrosSegue, int tamanhoSegue, int idPessoaBuscado){
    
  //Imprime os dados da pessoa
  printf("Dados da pessoa de codigo %d\n", idPessoa);
  
  if(tamNomePessoa > 0){
    printf("Nome: %s\n", nomePessoa);
  } else{
    printf("Nome: -\n");
  }
  
  if(idadePessoa == -1){
    printf("Idade: -\n");
  } else{
    printf("Idade: %d\n", idadePessoa);
  }
  
  if(tamNomeUsuario > 0){
    printf("Usuario: %s\n", nomeUsuario);
  } else {
    printf("Usuario: -\n");
  }
  
  printf("\n");
  
  //Busca binária modificada para encontrar a primeira ocorrência
  long int posicaoInicial = buscaBinariaSegueModificada(registrosSegue, tamanhoSegue, idPessoaBuscado);
  
  // Se não encontrou nenhum registro de segue, retorna
  if(posicaoInicial == -1){
    return;
  }
  
  // Percorre todos os registros com o mesmo idPessoaQueSegue
  int i = posicaoInicial;
  while(i < tamanhoSegue && registrosSegue[i].idPessoaQueSegue == idPessoaBuscado){
    // Pula registros removidos
    if(registrosSegue[i].removido[0] == '1'){
      i++;
      continue;
    }
    
    printf("Segue a pessoa de codigo: %d\n", registrosSegue[i].idPessoaQueESeguida);
    printf("Justificativa para seguir: %s\n", interpretaGrauAmizade(registrosSegue[i].grauAmizade[0]));
    
    // Imprime dataInicioQueSegue
    if(strcmp(registrosSegue[i].dataInicioQueSegue, "$$$$$$$$$$") == 0){
      printf("Começou a seguir em: -\n");
    } else {
      printf("Começou a seguir em: %s\n", registrosSegue[i].dataInicioQueSegue);
    }
    
    // Imprime dataFimQueSegue
    if(strcmp(registrosSegue[i].dataFimQueSegue, "$$$$$$$$$$") == 0){
      printf("Parou de seguir em: -\n");
    } else {
      printf("Parou de seguir em: %s\n", registrosSegue[i].dataFimQueSegue);
    }
    
    printf("\n");
    i++;
  }
  printf("\n");
}

