#include "../headers/geral.h"

// ===================================== funções gerais =====================================

/* Retorna:
  se ID existe: retorna a POSIÇÃO onde está (>= 0)
  se ID não existe: retorna -(posição_inserção + 1)
  exemplo: retorna -3 significa "não existe, deveria estar na posição 2", retorna assim porque se deve estar na posição 0, não posso retornar 0 se não confunde com a posição de um id existente, deve ser um valor negativo sempre que retornar a posição onde deveria estar o id buscado
*/
int buscaBinariaVetorIndice(indice* indices, int tamanho, int idPessoa) {
    int inicio = 0;
    int fim = tamanho - 1;
    
    // caso especial: vetor vazio
    if (tamanho == 0) {
        return -1;  // deveria inserir na posição 0 → retorna -(0+1) = -1
    }
    
    // caso especial: menor que o primeiro
    if (idPessoa < indices[0].idPessoa) {
        return -1;  // deveria inserir na posição 0 → retorna -(0+1) = -1
    }
    
    // caso especial: maior que o último
    if (idPessoa > indices[fim].idPessoa) {
        return -(tamanho + 1);  // deveria inserir na posição 'tamanho'
    }
    
    // busca binaria
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        
        if (indices[meio].idPessoa == idPessoa) {
            return meio;  // id existe, então retorna a posição em que ele esta no vetor
        }
        else if (indices[meio].idPessoa < idPessoa) {
            inicio = meio + 1;
        }
        else {
            fim = meio - 1;
        }
    }
    
    // não encontrou o id requerido, então retorna -(posição_inserção + 1)
    return -(inicio + 1);
}

char *meu_strsep(char** buffer, const char* delim) {
    char* inicio = *buffer;
    char* p;
    
    if (inicio == NULL) {
        return NULL;
    }
    
    p = strpbrk(inicio, delim);
    
    if (p) {
        *p = '\0';
        *buffer = p + 1;
    } else {
        *buffer = NULL;
    }
    
    return inicio;
}

char *removeEspacosEmBranco(char *campo){
  // Remove espaços do início
  while (*campo && isspace((unsigned char)*campo)) {
    campo++;
  }
  // Remove espaços do final
  int len = strlen(campo);
  while (len > 0 && isspace((unsigned char)campo[len - 1])) {
    campo[--len] = '\0';
  }
  return campo;
}

char *removerAspas(char *campo){
  if(campo == NULL || strlen(campo) < 2 || campo[0] != '"') return campo;
  
  int len = strlen(campo); //lê tamanho da string
  // Se começa com aspas, pula
  if (campo[0] == '"') {
    campo++;
    len--;
  }
  
  // Se termina com aspas, remove
  if (len > 0 && campo[len-1] == '"') {
    campo[len-1] = '\0';
  }
  
  return campo;
}

void defineStatusArquivo(FILE *arquivo, char status){
  fseek(arquivo, 0, SEEK_SET);  //posiciona ponteiro no byte 0
  fwrite(&status, sizeof(char), 1, arquivo); //escreve status
}

void lerValorCampo(char *valorCampo) {
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
}

// Função genérica para abrir arquivo, verificar erro e checar status
FILE* abrirArquivoComStatus(const char *nomeArquivo, const char *modo) {
    FILE *arquivo = fopen(nomeArquivo, modo);
    if (arquivo == NULL) {
        puts("Falha no processamento do arquivo.");
        exit(0);
    }

    char status;
    if (fread(&status, sizeof(char), 1, arquivo) != 1) {
        puts("Falha no processamento do arquivo.");
        fclose(arquivo);
        exit(0);
    }

    // Ponteiro do arquivo agora está após o byte do status
    return arquivo;
}

// Funcao auxiliar para imprimir um registro
void imprimirRegistro(int idPessoa, int idadePessoa, int tamNomePessoa, char *nomePessoa, int tamNomeUsuario, char *nomeUsuario){
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
    printf("Usuario: %s\n\n", nomeUsuario);
  } else{
    printf("Usuario: -\n\n");
  }
}

//função responsavel por adicionar um nó no final da lista de resultados de busca
void adicionarResultadoBusca(resultadoBusca **raizLista, resultadoBusca **ultimoResultado, registro *reg, long int byteOffset){
  resultadoBusca *novoResultado = calloc(1, sizeof(resultadoBusca));
  
  //aloca memória para o registro dentro do resultado
  novoResultado->reg = malloc(sizeof(registro));
  
  //copia os campos do registro encontrado
  novoResultado->reg->idPessoa = reg->idPessoa;
  novoResultado->reg->idadePessoa = reg->idadePessoa;
  novoResultado->reg->tamanhoNomePessoa = reg->tamanhoNomePessoa;
  strcpy(novoResultado->reg->nome, reg->nome);
  novoResultado->reg->tamanhoNomeUsuario = reg->tamanhoNomeUsuario;
  strcpy(novoResultado->reg->nomeUsuario, reg->nomeUsuario);
  novoResultado->byteOffset = byteOffset;
  novoResultado->proxResultado = NULL;
  
  //inserção no final da lista
  if(*raizLista == NULL){
    *raizLista = novoResultado;
    *ultimoResultado = novoResultado;
  } else{
    (*ultimoResultado)->proxResultado = novoResultado;
    *ultimoResultado = novoResultado;
  }
}


resultadoBusca* buscarRegistrosPorCampo(FILE *arqPessoa, indice *vetorIndice, int qtdIndice, long sizeDados, char *nomeCampo, char *valorCampo){
  //inicializa a lista de resultados
  resultadoBusca *raizListaResultados = NULL;
  resultadoBusca *ultimoResultado = NULL;
  registro reg;
  
  // Se nomeCampo for NULL, lista todos os registros
  if(nomeCampo == NULL){
    fseek(arqPessoa, 17, SEEK_SET); //pula o cabeçalho
    
    while(ftell(arqPessoa) < sizeDados){
      long int offsetAtual = ftell(arqPessoa);
      
      char removido;
      fread(&removido, sizeof(char), 1, arqPessoa);
      int tamRegistro;
      fread(&tamRegistro, sizeof(int), 1, arqPessoa);
      
      if (removido == '1') {
          fseek(arqPessoa, tamRegistro, SEEK_CUR);
          continue;
      }
      
      //lê os campos do registro
      fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
      fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
      fread(&reg.tamanhoNomePessoa, sizeof(int), 1, arqPessoa);
      fread(reg.nome, sizeof(char), reg.tamanhoNomePessoa, arqPessoa);
      reg.nome[reg.tamanhoNomePessoa] = '\0';        
      fread(&reg.tamanhoNomeUsuario, sizeof(int), 1, arqPessoa);
      fread(reg.nomeUsuario, sizeof(char), reg.tamanhoNomeUsuario, arqPessoa);
      reg.nomeUsuario[reg.tamanhoNomeUsuario] = '\0';
      
      // Adiciona todos os registros não removidos
      adicionarResultadoBusca(&raizListaResultados, &ultimoResultado, &reg, offsetAtual);
      
      // vai para o próximo registro
      fseek(arqPessoa, offsetAtual + 5 + tamRegistro, SEEK_SET);
    }
  }
  else if(strcmp(nomeCampo, "idPessoa") == 0){
    int idBusca = atoi(valorCampo);
    long int offset;
    int posRetornada = buscaBinariaVetorIndice(vetorIndice, qtdIndice, idBusca);
    if(posRetornada < 0){
      offset = -1;
    } else{
      offset = vetorIndice[posRetornada].byteOffset;
    }
    if(offset != -1){
      fseek(arqPessoa, offset, SEEK_SET);
      
      char removido;
      fread(&removido, sizeof(char), 1, arqPessoa);
      
      //adiciona se o registro não estiver removido
      if(removido == '0'){
        int tamRegistro;
        fread(&tamRegistro, sizeof(int), 1, arqPessoa);
        
        fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
        fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
        fread(&reg.tamanhoNomePessoa, sizeof(int), 1, arqPessoa);
        fread(reg.nome, sizeof(char), reg.tamanhoNomePessoa, arqPessoa);
        reg.nome[reg.tamanhoNomePessoa] = '\0';
        fread(&reg.tamanhoNomeUsuario, sizeof(int), 1, arqPessoa);
        fread(reg.nomeUsuario, sizeof(char), reg.tamanhoNomeUsuario, arqPessoa);
        reg.nomeUsuario[reg.tamanhoNomeUsuario] = '\0';
        
        adicionarResultadoBusca(&raizListaResultados, &ultimoResultado, &reg, offset);
      }
    }
  } else{
    fseek(arqPessoa, 17, SEEK_SET); //pula o cabeçalho
    
    while(ftell(arqPessoa) < sizeDados){
      long int offsetAtual = ftell(arqPessoa);
      
      char removido;
      fread(&removido, sizeof(char), 1, arqPessoa);
      int tamRegistro;
      fread(&tamRegistro, sizeof(int), 1, arqPessoa);
      if (removido == '1') {
          fseek(arqPessoa, tamRegistro, SEEK_CUR);
          continue;
      }
      //lê os campos do registro
      fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
      fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
      fread(&reg.tamanhoNomePessoa, sizeof(int), 1, arqPessoa);
      fread(reg.nome, sizeof(char), reg.tamanhoNomePessoa, arqPessoa);
      reg.nome[reg.tamanhoNomePessoa] = '\0';        
      fread(&reg.tamanhoNomeUsuario, sizeof(int), 1, arqPessoa);
      fread(reg.nomeUsuario, sizeof(char), reg.tamanhoNomeUsuario, arqPessoa);
      reg.nomeUsuario[reg.tamanhoNomeUsuario] = '\0';
      
      //compara o campo solicitado com o valor buscado
      int encontrado = 0;
      if(strcmp(nomeCampo, "idadePessoa") == 0){
        if(strlen(valorCampo) == 0){
          if(reg.idadePessoa == -1){
            encontrado = 1;
          }
        } else {
          if(reg.idadePessoa == atoi(valorCampo)){
            encontrado = 1;
          }
        }
      }
      else if(strcmp(nomeCampo, "nomePessoa") == 0){
        if(strlen(valorCampo) == 0){
          if(reg.tamanhoNomePessoa == 0){
            encontrado = 1;
          }
        } else {
          if(strcmp(reg.nome, valorCampo) == 0){
            encontrado = 1;
          }
        }
      }
      else if(strcmp(nomeCampo, "nomeUsuario") == 0){
        if(strcmp(reg.nomeUsuario, valorCampo) == 0){
          encontrado = 1;
          adicionarResultadoBusca(&raizListaResultados, &ultimoResultado, &reg, offsetAtual);
          return raizListaResultados;
        }
      }
      if(encontrado == 1){
        adicionarResultadoBusca(&raizListaResultados, &ultimoResultado, &reg, offsetAtual);
      }
      
      // vai para o próximo registro usando tamRegistro
      fseek(arqPessoa, offsetAtual + 5 + tamRegistro, SEEK_SET);
    }
  }
  
  return raizListaResultados;
}

//libera a memória alocada pela lista de resultados de busca, ela é necessária pois a lista será usada em muitas funcionalidades 
void liberarListaResultados(resultadoBusca *raizLista){
  resultadoBusca *atual = raizLista;
  while (atual != NULL){
    resultadoBusca *proximo = atual->proxResultado;
    free(atual);
    atual = proximo;
  }
}