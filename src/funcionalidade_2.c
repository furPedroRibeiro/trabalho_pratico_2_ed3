//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_2.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"


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