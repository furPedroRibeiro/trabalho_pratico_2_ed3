//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221
//aqui estão todas as funções que serão usadas para que as funcionalidades 1 a 10 sejam implementadas

#include "../headers/auxiliar.h"
#include "../headers/utilidades.h"

// Definição(atribuição) das variáveis globais (declaradas em auxiliar.h)
registro* raizListaPessoas = NULL;
indice* raizListaIndice = NULL;

// Variáveis globais internas (static = visíveis só neste arquivo)
static registro* ultimoElemento = NULL;
static char status = '0';
static int quantidadePessoas = 0;
static int quantidadeRemovidos = 0;
static long int proxByteoffset = 17;




// ======================= FUNÇÕES PARA FUNCIONALIDADE 2 =======================

//essa função lê o arquivo csv, faz a inserção no arquivo de dados a cada linha lida do csv, e vai montando a lista duplamente encadeada para o índice ser inserido depois no arquivo de índice
void lerCSV(FILE* arquivoDados, FILE* arquivoIndice, FILE* arquivoEntrada){
  //criando buffer para ler uma linha do arquivo
  char bufferLinha[1024];

  //fgets para ignorar a primeira linha do arquivo csv, que contém os nomes dos campos
  fgets(bufferLinha, sizeof(bufferLinha), arquivoEntrada);

  //função para definir o cabeçalho do arquivo de dados é chamada antes da leitura começar
  criaCabecalhoArquivoDados(arquivoDados, status, quantidadePessoas, quantidadeRemovidos, proxByteoffset);

  //aqui começa a leitura a partir da segunda linha do arquivo de dados
  while(fgets(bufferLinha, sizeof(bufferLinha), arquivoEntrada) != NULL){
    //declarando variável para obter o byteoffset do registro que será inserido nessa iteração
    long int byteoffsetAtual = proxByteoffset;
    //contador de tamanho de registro, tem no mínimo 21 bytes por registro por causa dos campos de tamanho fixo
    int tamRegistroBytes = 16;
    //removendo \n do fim da string
    bufferLinha[strcspn(bufferLinha, "\r\n")] = '\0';

    //criando ponteiro que aponta para o buffer que armazena a linha
    char *str = bufferLinha;

    //a partir daqui a leitura dos campos é feita usando a função meu_strsep, explicada e declarada em auxiliar.h
    char* campoIdPessoa = meu_strsep(&str, ",");  //a leitura de um campo é feita observando o conteúdo de str, ou seja, a string que está em buffer linha    
    char* campoNomePessoa = meu_strsep(&str, ",");
    char* campoIdadePessoa = meu_strsep(&str, ",");
    char* campoNomeUsuario = meu_strsep(&str, ",");

      
    //criando ponteiro para um novo registro
    registro* novoRegistro = calloc(1, sizeof(registro));
    //INSERÇÃO NA LISTA DE REGISTROS: chama função
    criarNoRegistro(novoRegistro, campoIdPessoa, campoIdadePessoa, campoNomePessoa, campoNomeUsuario, tamRegistroBytes);

    //agora é preciso fazer uma inserção no arquivo de dados, para cada leitura é necessário fazer uma inserção no arquivo de dados
    insereRegistro(novoRegistro, arquivoDados, quantidadeRemovidos, proxByteoffset);

    //depois que o registro é inserido, podemos adicionar um nó para o índice na lista de índice, que vai ser criada inteira antes de adicionarmos ela para o arquivo de índice

    //criando ponteiro para um novo registro de indice
    indice* novoRegistroIndice = calloc(1, sizeof(indice));
    //INSERÇÃO NA LISTA DE ÍNDICE: chama função
    criarNoRegistroIndice(novoRegistroIndice, campoIdPessoa, byteoffsetAtual);
  }
  //acabou a leitura do arquivo e a inserção de registros no arquivo de dados, agora é necessário fazer a inserção no arquivo de índice. A lista de índices já estará ordenada e com os byteoffsets certos, já que fizemos uma inserção a cada leitura
  //o arquivo csv pode ser fechado:
  fclose(arquivoEntrada);
  //com o cabeçalho todo reescrito e todos os registros cadastrados, basta atualizar o status novamente para 1, que é consistente
  defineStatusArquivo(arquivoDados, '1');

  fclose(arquivoDados);

  insereRegistroIndice(raizListaIndice, arquivoIndice);

  //função acabou
}

//função que cria cabeçalho do arquivo de dados
void criaCabecalhoArquivoDados(FILE* arquivoDados, char status, int quantidadePessoas, int quantidadeRemovidos, long int proxByteoffsetAtual){
  //garante que o ponteiro está posicionado no começo do arquivo:
  fseek(arquivoDados, 0, SEEK_SET);
  //escreve cabeçalho
  //variável para contar se o tamanho do cabeçalho ta certo:
  int tamCabecalho = 0;
  fwrite(&status, sizeof(char), 1, arquivoDados);
  fwrite(&quantidadePessoas, sizeof(int), 1, arquivoDados);
  fwrite(&quantidadeRemovidos, sizeof(int), 1, arquivoDados);
  fwrite(&proxByteoffsetAtual, sizeof(long int), 1, arquivoDados);
}

//função para inserir um registro no arquivo de dados
void insereRegistro(registro* novoRegistro, FILE* arquivoDados, int quantidadeRemovidos, long int proxByteoffsetAtual){
  //a cada inserção, o cabeçalho é atualizado com a quantidade de pessoas, o número de pessoas removidas e o próximo byte offset disponível, além disso, o status é definido pra 1 no começo da inserção para indicar que o arquivo está inconsistente e depois da inserção ele é 0 para mostrar que está consistente

  //agora reposicionamos o ponteiro do arquivo para escrever o registro, aproveitando que proxByteOffset ainda não foi atualizado
  fseek(arquivoDados, proxByteoffsetAtual, SEEK_SET);

  //inserção:
  int tamanhoRegistro = novoRegistro->tamRegistro;
  int idPessoa = novoRegistro->idPessoa;
  int idadePessoa = novoRegistro->idadePessoa;
  int tamNomePessoa = novoRegistro->tamanhoNomePessoa;
  char *nomePessoa = novoRegistro->nome;
  int tamNomeUsuario = novoRegistro->tamanhoNomeUsuario;
  char *nomeUsuario = novoRegistro->nomeUsuario;

  fwrite(&novoRegistro->removido[0], sizeof(char), 1, arquivoDados);
  fwrite(&tamanhoRegistro, sizeof(int), 1, arquivoDados);
  fwrite(&idPessoa, sizeof(int), 1, arquivoDados);
  fwrite(&idadePessoa, sizeof(int), 1, arquivoDados);
  fwrite(&tamNomePessoa, sizeof(int), 1, arquivoDados);
  if(tamNomePessoa == 0){
    //não escreve nome da pessoa
  } else{
    fwrite(nomePessoa, sizeof(char), tamNomePessoa, arquivoDados);
  }
  fwrite(&tamNomeUsuario, sizeof(int), 1, arquivoDados);
  if(tamNomeUsuario == 0){
    //não escreve nome do usuário
  } else{
    fwrite(nomeUsuario, sizeof(char), tamNomeUsuario, arquivoDados);
  }

  //atualizando byteoffset de modo que o próximo byteoffset livre seja depois do registro que acabou de ser inserido
  proxByteoffset += (novoRegistro->tamRegistro)+5;
  //agora devemos atualizar o registro de cabeçalho:
  quantidadePessoas++;
  //movendo ponteiro do arquivo para o byte 1:
  fseek(arquivoDados, 1, SEEK_SET);
  //para atualizar a quantidade de pessoas, vamos escrever um inteiro
  fwrite(&quantidadePessoas, sizeof(int), 1, arquivoDados);
  fwrite(&quantidadeRemovidos, sizeof(int), 1, arquivoDados);
  fwrite(&proxByteoffset, sizeof(long int), 1, arquivoDados);
  
  if(novoRegistro->proxRegistro != NULL){
    fseek(arquivoDados, proxByteoffset, SEEK_SET);
  } else if(novoRegistro->proxRegistro == NULL){
    //não faz fseek para o prox byte offset disponível
  }
}

void insereRegistroIndice(indice* raizListaIndice, FILE* arquivoIndice){
  //mudando status no cabeçalho:
  defineStatusArquivo(arquivoIndice, '0');

  //volta ponteiro para escrever o primeiro registro de índice
  fseek(arquivoIndice, 12, SEEK_SET);
  //noAuxiliarIndice para receber a raíz da lista de registros de indice
  indice* noAuxiliarIndice = raizListaIndice;

  //loop para inserir todos os nós da lista no arquivo
  while(noAuxiliarIndice != NULL){
    //obtendo informações
    int idPessoa = noAuxiliarIndice->idPessoa;
    long int byteoffset = noAuxiliarIndice->byteOffset;

    //print pra ver se deu certo:
    // printf("\n\nId pessoa: %d byteoffset: %ld ", idPessoa, byteoffset);


    //escreve no arquivo de índice
    fwrite(&idPessoa, sizeof(int), 1, arquivoIndice);
    fwrite(&byteoffset, sizeof(long int), 1, arquivoIndice);

    //atualiza o nó atual
    noAuxiliarIndice = noAuxiliarIndice->proxIndice;
  }

  defineStatusArquivo(arquivoIndice, '1');

  //fechando arquivo de índice
  fclose(arquivoIndice);
}

//essa função cria o nó e adiciona esse nó novo a lista encadeada de registros do arquivo de dados
void criarNoRegistro(registro* novoRegistro, char *campoIdPessoa, char *campoIdadePessoa, char *campoNomePessoa, char *campoNomeUsuario, int tamRegistroBytes){
  //definindo id, não precisa de tratamento pois id não pode ser nulo
  novoRegistro->idPessoa = atoi(campoIdPessoa);
  
  //definindo idade da pessoa
  if(campoIdadePessoa == NULL || campoIdadePessoa[0] == '\0'){
    novoRegistro->idadePessoa = -1;
  } else{
    novoRegistro->idadePessoa = atoi(campoIdadePessoa);
  }

  //definindo nome da pessoa
  if(campoNomePessoa != NULL && campoNomePessoa[0] != '\0'){
    strcpy(novoRegistro->nome, campoNomePessoa);
    //para esse campo, é necessário também armazenar o tamanho do nome da pessoa
    novoRegistro->tamanhoNomePessoa = strlen(campoNomePessoa);
    //incrementa os bytes do nome para o contador do tamanho do registro
    tamRegistroBytes += strlen(campoNomePessoa);
  } else{
    strcpy(novoRegistro->nome, "");
    novoRegistro->tamanhoNomePessoa = 0;
  }

  //definindo nome de usuário da pessoa
  if(campoNomeUsuario != NULL && campoNomeUsuario[0] != '\0'){
    strcpy(novoRegistro->nomeUsuario, campoNomeUsuario);
    //para esse campo, é necessário também armazenar o tamanho do nome do usuario
    novoRegistro->tamanhoNomeUsuario = strlen(campoNomeUsuario);
    //incrementa os bytes do nome para o contador do tamanho do registro
    tamRegistroBytes += strlen(campoNomeUsuario);
  } else{
    strcpy(novoRegistro->nomeUsuario, "");
    novoRegistro->tamanhoNomeUsuario = 0;
  }

  //caso para a raiz da lista ser nula
  if(raizListaPessoas == NULL){
    novoRegistro->removido[0] = '0'; // 0 é não removido
    novoRegistro->tamRegistro = tamRegistroBytes;
    raizListaPessoas = novoRegistro;
    novoRegistro->proxRegistro = NULL;
    ultimoElemento = novoRegistro;

    return;
  }

  //definindo dados que não são lidos do csv
  novoRegistro->removido[0] = '0'; // 0 é não removido
  novoRegistro->tamRegistro = tamRegistroBytes;
  novoRegistro->proxRegistro = NULL; 
  ultimoElemento->proxRegistro = novoRegistro;
  ultimoElemento = novoRegistro; 
}

//essa função cria e adiciona um nó novo a lista duplamente encadeada de registros do arquivo de índice
void criarNoRegistroIndice(indice* novoRegistroIndice, char *campoIdPessoa, long int byteoffset){
  novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
  novoRegistroIndice->byteOffset = byteoffset;
  //a inserção será ordenada, então começamos com um ponteiro auxiliar que aponta para a raiz da lista
  indice* ponteiroAuxiliar = raizListaIndice;

  //se a lista for nula:
  if(raizListaIndice == NULL){
    novoRegistroIndice->antIndice = NULL;
    novoRegistroIndice->proxIndice = NULL;
    novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
    novoRegistroIndice->byteOffset = byteoffset;
    raizListaIndice = novoRegistroIndice;
    return;
  }

  //enquanto o ponteiro auxiliar não for o último, esse loop estará ativo
  while(ponteiroAuxiliar != NULL){
    //declarando e criando ponteiro para o nó anterior e para o próximo nó do ponteiro auxiliar, já que a lista de indice é uma lista duplamente encadeada
    indice *ponteiroAuxiliarProx = ponteiroAuxiliar->proxIndice;
    indice *ponteiroAuxiliarAnt = ponteiroAuxiliar->antIndice;

    //nesse primeiro condicional, se encontra o caso em que o ponteiro é a raíz da lista e o que queremos inserir é menor que a raíz
    if(ponteiroAuxiliarAnt == NULL){
      if(novoRegistroIndice->idPessoa < ponteiroAuxiliar->idPessoa){
        novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
        novoRegistroIndice->byteOffset = byteoffset;
        
        novoRegistroIndice->proxIndice = ponteiroAuxiliar;
        novoRegistroIndice->antIndice = NULL;
        ponteiroAuxiliar->antIndice = novoRegistroIndice;
        raizListaIndice = novoRegistroIndice;
        //a função acaba quando ocorre a inserção
        return;
      } else { 
      }
    }
    //no segundo condicional, se não for a raíz, verificamos se o nó que queremos inserir tem id menor que o nó atual procurado, se sim insere antes disso, se não vai pro próximo; além disso, se for menor, o nó se insere no meio de dois nós, de modo que os ponteiro se ajeitem
    else if(novoRegistroIndice->idPessoa < ponteiroAuxiliar->idPessoa){
      novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
      novoRegistroIndice->byteOffset = byteoffset;
      
      novoRegistroIndice->proxIndice = ponteiroAuxiliar;
      novoRegistroIndice->antIndice = ponteiroAuxiliarAnt;
      ponteiroAuxiliarAnt->proxIndice = novoRegistroIndice;    ponteiroAuxiliar->antIndice = novoRegistroIndice;
      //a função acaba quando ocorre inserção
      return;
    }
    //esse condicional verifica se é o último nó da lista
    else if(ponteiroAuxiliarProx == NULL){
      novoRegistroIndice->idPessoa = atoi(campoIdPessoa);
      novoRegistroIndice->byteOffset = byteoffset;

      novoRegistroIndice->antIndice = ponteiroAuxiliar;
      novoRegistroIndice->proxIndice = NULL;
      ponteiroAuxiliar->proxIndice = novoRegistroIndice;
      //retorna quando ocorre a inserção
    }
    //se não for uma dessas opções, o ponteiro auxiliar passa pro próximo
    ponteiroAuxiliar = ponteiroAuxiliarProx;
  }
}




// ======================= FUNÇÕES PARA FUNCIONALIDADES 3 E 4 =======================

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

//libera a memória alocada pela lista de resultados de busca, ela é necessária pois a lista será usada em muitas funcionalidades 
void liberarListaResultados(resultadoBusca *raizLista){
  resultadoBusca *atual = raizLista;
  while (atual != NULL){
    resultadoBusca *proximo = atual->proxResultado;
    free(atual);
    atual = proximo;
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

//imprimir registro encontrado por byteOffset
void imprimirRegistroPorByteOffset(FILE *arqPessoa, long int byteOffset){
    //posiciona o ponteiro no registro 
    fseek(arqPessoa, byteOffset, SEEK_SET);

    char removido;
    fread(&removido, sizeof(char), 1, arqPessoa);
    if (removido == '1'){
      return; // registro marcado como removido
    }

    //le o tamanho do registro
    int tamRegistro;
    fread(&tamRegistro, sizeof(int), 1, arqPessoa);

    //cria registro local para ler os dados
    registro reg;
    
    //lê cada campo do registro
    fread(&reg.idPessoa, sizeof(int), 1, arqPessoa);
    fread(&reg.idadePessoa, sizeof(int), 1, arqPessoa);
    fread(&reg.tamanhoNomePessoa, sizeof(int), 1, arqPessoa);
    fread(reg.nome, sizeof(char), reg.tamanhoNomePessoa, arqPessoa);
    reg.nome[reg.tamanhoNomePessoa] = '\0';
    fread(&reg.tamanhoNomeUsuario, sizeof(int), 1, arqPessoa);
    fread(reg.nomeUsuario, sizeof(char), reg.tamanhoNomeUsuario, arqPessoa);
    reg.nomeUsuario[reg.tamanhoNomeUsuario] = '\0';

    //usa função auxiliar para imprimir
    imprimirRegistro(reg.idPessoa, reg.idadePessoa, reg.tamanhoNomePessoa, reg.nome, reg.tamanhoNomeUsuario, reg.nomeUsuario);
}




// ======================= FUNÇÕES PARA FUNCIONALIDADE 6: =======================

cabecalhoPessoa* lerCabecalho(FILE *nomeArquivo){
  cabecalhoPessoa *cabecalho = calloc(1, sizeof(cabecalhoPessoa)); //aloca memória para o cabecalho
  
  fseek(nomeArquivo, 0, SEEK_SET); //posiciona para ler status
  fread(&cabecalho->status, sizeof(char), 1, nomeArquivo); //faz a leitura para status
  cabecalho->status[1] = '\0';
  fread(&cabecalho->quantidadePessoas, sizeof(int), 1, nomeArquivo);
  fread(&cabecalho->quantidadeRemovidos, sizeof(int), 1, nomeArquivo);
  fread(&cabecalho->proxByteoffset, sizeof(long int), 1, nomeArquivo);
  return cabecalho; //após ler o cabecalho, retorna para a função
}

noRegistroUnico* lerEntradaInsercaoUnica(){
  noRegistroUnico *regUnico = calloc(1, sizeof(noRegistroUnico));
  regUnico->nomePessoa = malloc(100 * sizeof(char));
  regUnico->nomeUsuario = malloc(100 * sizeof(char));

  //vamos ler uma linha de entrada, usando a mesma lógica usada na main
  //faz a leitura da entrada através de stdin
  int c = 0; 
  int i = 0;
  char bufferEntrada[1024];
  
  // lê até EOF, \0 ou \n — cobre todos os casos possíveis porque a entrada no runcodes é de um jeito e pelo teclado para testes é de outro, então ao invés de usar fgets fizemos a adaptação para todos os tipos de terminação de entrada
  while ((c = getchar()) != EOF && c != '\0' && c != '\n') {
      if (i < sizeof(bufferEntrada) - 1) {
        bufferEntrada[i++] = c;
      } else {
        //limpa o resto da entrada
        while ((c = getchar()) != EOF && c != '\n');
        break; // evita estouro de buffer(seg fault)
      }
  }
  bufferEntrada[i] = '\0'; //coloca o \0 no fim da entrada para usar strtok corretamente

  //lendo qual iteração é
  char *parametro;
  parametro = strtok(bufferEntrada, " ");
  //não usaremos para nada qual iteração é, então já leremos o próximo parâmetro direto

  //lendo idPessoa, não pode ser nulo
  parametro = strtok(NULL, ",");
  regUnico->idPessoa = atoi(parametro);
  //lendo nome da pessoa
  parametro = strtok(NULL, ",");
  char *nomePessoa = removeEspacosEmBranco(parametro);
  if(strcmp(nomePessoa, "NULO") == 0){
    strcpy(regUnico->nomePessoa, "NULO");
  } else{
    strcpy(regUnico->nomePessoa, removerAspas(nomePessoa));
  }
  //lendo idade
  parametro = strtok(NULL, ",");
  char *idadePessoa = removeEspacosEmBranco(parametro);
  if(strcmp(idadePessoa, "NULO") == 0){
    regUnico->idadePessoa = -1;
  } else{
    regUnico->idadePessoa = atoi(idadePessoa);
  }
  //lendo nome de usuário
  parametro = strtok(NULL, ",");
  char *nomeUsuario = removeEspacosEmBranco(parametro);
  strcpy(regUnico->nomeUsuario, removerAspas(nomeUsuario));

  //printa registro único para ver se ta lendo certo:
  //printf("\nID: %d\nNome Pessoa: %s\nIdade pessoa: %d\nNome Usuário: %s\n", regUnico->idPessoa, regUnico->nomePessoa, regUnico->idadePessoa, regUnico->nomeUsuario);

  return regUnico;
}

void insereRegistroUnicoPessoa(FILE *nomeArquivoPessoa, noRegistroUnico* regUnico, cabecalhoPessoa *cabecalho){
  fseek(nomeArquivoPessoa ,cabecalho->proxByteoffset, SEEK_SET); //posiciona ponteiro no próximo byteoffset disponível segundo o cabecalho
  //definindo variáveis que não foram lidas
  char removido = '0';
  int tamNomePessoa;
  int tamNomeUsuario = strlen(regUnico->nomeUsuario);
  if(strcmp(regUnico->nomePessoa, "NULO") == 0){
    tamNomePessoa = 0;
  } else{
    tamNomePessoa = strlen(regUnico->nomePessoa);
  }
  int tamRegistro = tamNomePessoa + tamNomeUsuario + 16;
  fwrite(&removido, sizeof(char), 1, nomeArquivoPessoa);       
  fwrite(&tamRegistro, sizeof(int), 1, nomeArquivoPessoa);   
  fwrite(&regUnico->idPessoa, sizeof(int), 1, nomeArquivoPessoa);
  fwrite(&regUnico->idadePessoa, sizeof(int), 1, nomeArquivoPessoa);
  fwrite(&tamNomePessoa, sizeof(int), 1, nomeArquivoPessoa);
  if(tamNomePessoa > 0){
    fwrite(regUnico->nomePessoa, sizeof(char), tamNomePessoa, nomeArquivoPessoa);
  } else{

  }
  fwrite(&tamNomeUsuario, sizeof(int), 1, nomeArquivoPessoa);
  fwrite(regUnico->nomeUsuario, sizeof(char), tamNomeUsuario, nomeArquivoPessoa);

  //depois que escreve tudo, precisamos atualizar o cabeçalho
  int quantidadePessoas = cabecalho->quantidadePessoas;
  quantidadePessoas++;
  long int proxByteoffset = cabecalho->proxByteoffset + tamRegistro + 5;
  //escreve no cabeçalho:
  fseek(nomeArquivoPessoa, 1, SEEK_SET);
  fwrite(&quantidadePessoas, sizeof(int), 1, nomeArquivoPessoa);
  fseek(nomeArquivoPessoa, 9, SEEK_SET);
  fwrite(&proxByteoffset, sizeof(long int), 1, nomeArquivoPessoa);
}

indice* lerArquivoIndice(FILE *nomeArquivoIndice, int n, int mais_n){
  //o arquivo já está aberto, e n é o número de registros ativos
  //o array para o vetor de indices é criada com 1 espaço a mais porque vamos inserir um novo registro a esse array posteriormente, então precisa ter esse espaço para que ocorra o deslocamento e a inserção correta do novo registroz
  indice *indices = malloc((n+mais_n) * sizeof(indice));
  fseek(nomeArquivoIndice, 12, SEEK_SET); //posiciona ponteiro de leitura no primeiro registro
  for(int i = 0; i < n; i++){
    fread(&indices[i].idPessoa, sizeof(int), 1, nomeArquivoIndice);
    fread(&indices[i].byteOffset, sizeof(long int), 1, nomeArquivoIndice);
  }
  //depois da leitura, retorna o vetor de indices
  return indices;
}

void insereRegistroUnicoVetorIndice(indice* indices, int tamanhoVetor, int idPessoa, long int byteoffset){
  //o conceito de busca binária é utilizado para retornar uma posição válida em que o registro pode ser inserido
  int pos = buscaBinariaVetorIndice(indices, tamanhoVetor, idPessoa);
  //vai retornar um valor negativo, já que a busca binária vai retornar a posição onde o id deveria estar, logo precisamos converter pra positivo e subtrair 1
  //exemplo: retorna -3(deveria estar na posição 2), então pos = -pos -1
  if(pos < 0){
    pos = -(pos) -1;
  }
  //desloca tudo até a posição
  for (int i = tamanhoVetor; i > pos; i--){
    indices[i] = indices[i-1];
  }
  indices[pos].idPessoa = idPessoa;
  indices[pos].byteOffset = byteoffset;
  //feito isso, o vetor está atualizado e 100% pronto para voltar para memória secundária
}

void insereIndice(indice* indices, FILE *nomeArquivoIndice, int tamanho){
  //o indice será todo reescrito em memória secundária, como aqui estamos escrevendo um registro a mais, o tamanho do arquivo anterior pouco nos importa
  //o status já está definido como inconsistente, agora só nos resta pular o cabeçalho e reescrever tudo
  fseek(nomeArquivoIndice, 12, SEEK_SET);
  for(int i = 0; i < tamanho; i++){
    fwrite(&indices[i].idPessoa, sizeof(int), 1, nomeArquivoIndice);
    fwrite(&indices[i].byteOffset, sizeof(long int), 1, nomeArquivoIndice);
  }
  //depois de escrever tudo:
  return;
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




// ======================= FUNÇÕES PARA A FUNCIONALIDADE 8: =======================

void criaCabecalhoArquivoSegue(FILE *arqSegue, char status, int quantidadeRegistros, int proxByteoffset){
  //o arquivo já está aberto, é só posicionar o ponteiro e escrever
  fseek(arqSegue, 0, SEEK_SET);

  fwrite(&status, sizeof(char), 1, arqSegue); //escreve status
  fwrite(&quantidadeRegistros, sizeof(int), 1, arqSegue); //escreve qtd registros
  fwrite(&proxByteoffset, sizeof(int), 1, arqSegue); //escreve proxRRN
}

void insereArquivoSegue(FILE *arqSegue, noSegue *noAtual){
  //essa função vai apenas fazer os fwrite e não retorna nada
  fwrite(&noAtual->removido[0], sizeof(char), 1, arqSegue);
  fwrite(&noAtual->idPessoaQueSegue, sizeof(int), 1, arqSegue);
  fwrite(&noAtual->idPessoaQueESeguida, sizeof(int), 1, arqSegue);
  fwrite(noAtual->dataInicioQueSegue, sizeof(char), 10, arqSegue);
  fwrite(noAtual->dataFimQueSegue, sizeof(char), 10, arqSegue);
  fwrite(&noAtual->grauAmizade[0], sizeof(char), 1, arqSegue);
}




//======================= FUNÇÕESP PARA A FUNCIONALIDADE 9: =======================

int comparaParaOrdenar(const void *a, const void *b){
  const noSegue *noA = (const noSegue*)a;
  const noSegue *noB = (const noSegue*)b;

  if(noA->idPessoaQueSegue == noB->idPessoaQueSegue){
    if(noA->idPessoaQueESeguida != noB->idPessoaQueESeguida){
      return noA->idPessoaQueESeguida - noB->idPessoaQueESeguida;
    } else{
      if(strcmp(noA->dataInicioQueSegue, noB->dataInicioQueSegue) != 0){
        return strcmp(noA->dataInicioQueSegue, noB->dataInicioQueSegue);
      } else{
        if(strcmp(noA->dataFimQueSegue, "$$$$$$$$$$") == 0 && strcmp(noB->dataFimQueSegue, "$$$$$$$$$$") == 0){
          return 0; //campos são iguais
        } else if(strcmp(noA->dataFimQueSegue, "$$$$$$$$$$") == 0){
          return 1; //A deve ir depois de B
        } else if(strcmp(noB->dataFimQueSegue, "$$$$$$$$$$") == 0){
          return -1; //A deve ir antes de B
        }
        return strcmp(noA->dataFimQueSegue, noB->dataFimQueSegue);
      }
    }
  }

  return noA->idPessoaQueSegue - noB->idPessoaQueSegue;
}

void escreveSegueOrdenado(FILE *arqOrdenado, int sizeArray, noSegue *registros){
  fseek(arqOrdenado, 0, SEEK_SET); // garante que o ponteiro de escrita em no byte 0

  //inicializa variáveis do cabeçalho
  char statusInconsistente = '0';
  int qtdRegistros = 0;
  int proxRRN = 0;
  //escreve cabeçalho
  fwrite(&statusInconsistente, sizeof(char), 1, arqOrdenado);
  fwrite(&qtdRegistros, sizeof(int), 1, arqOrdenado);
  fwrite(&proxRRN, sizeof(int), 1, arqOrdenado);

  long int byteoffset = 9; //pula o cabeçalho

  for(int i = 0; i < sizeArray; i++){
    fseek(arqOrdenado, byteoffset, SEEK_SET);
    fwrite(registros[i].removido, sizeof(char), 1, arqOrdenado);
    fwrite(&registros[i].idPessoaQueSegue, sizeof(int), 1, arqOrdenado);
    fwrite(&registros[i].idPessoaQueESeguida, sizeof(int), 1, arqOrdenado);
    fwrite(registros[i].dataInicioQueSegue, sizeof(char), 10, arqOrdenado);
    registros[i].dataInicioQueSegue[10] = '\0';
    fwrite(registros[i].dataFimQueSegue, sizeof(char), 10, arqOrdenado);
    registros[i].dataFimQueSegue[10] = '\0';
    fwrite(registros[i].grauAmizade, sizeof(char), 1, arqOrdenado);
  
    byteoffset = byteoffset + 30;
    proxRRN++;
    qtdRegistros++;

    //escreve as informações no cabeçalho
    fseek(arqOrdenado, 1, SEEK_SET);
    fwrite(&qtdRegistros, sizeof(int), 1, arqOrdenado);
    fwrite(&proxRRN, sizeof(int), 1, arqOrdenado);
  }
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