//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_6.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"

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
    defineStatusArquivo(arqDados, '0');
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

    defineStatusArquivo(arqDados, '1');
    defineStatusArquivo(arqIndice, '1');

    fclose(arqDados);
    fclose(arqIndice);
    
    binarioNaTela(nomeArquivoPessoa);
    binarioNaTela(nomeArquivoIndice);
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