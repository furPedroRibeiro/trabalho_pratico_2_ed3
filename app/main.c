//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_1.h"
#include "../headers/funcionalidade_2.h"
#include "../headers/funcionalidade_3.h"
#include "../headers/funcionalidade_4.h"
#include "../headers/funcionalidade_5.h"
#include "../headers/funcionalidade_6.h"
#include "../headers/funcionalidade_7.h"
#include "../headers/funcionalidade_8.h"
#include "../headers/funcionalidade_9.h"
#include "../headers/funcionalidade_10.h"

//aqui roda a main, que lê a entrada e depois roda um switch case para selecionar a funcionalidade a ser executada

int main(){
  //variáveis que podem ser usadas para chamadas e execução de funções:
  //guarda o ponteiro para o nome de arquivo de entrada
  char *nomeArquivoEntradaPessoa;
  //guarda o ponteiro para o nome de arquivo de índice
  char *nomeArquivoIndicePessoa;
  //guarda o ponteiro para o nome de arquivo de saída
  char *nomeArquivoSaidaPessoa;

  //guarda o ponteiro para o nome de arquivo de entrada
  char *nomeArquivoEntradaSegue;
  //guarda o ponteiro para o nome de arquivo de saída
  char *nomeArquivoSaidaSegue;
  //guarda o nome do arquivo ordenado de segue
  char *nomeArquivoOrdenadoSegue;

  //primeiro deve-se receber a entrada do usuário para saber a saída desejada e qual função executar
  
  //variável responsável por armazenar o parâmetro inicial no primeiro uso de strtok
  char* parametro;
  int codFuncionalidade;
  //faz a leitura da entrada através de stdin
  int c = 0; 
  int i = 0;
  char bufferEntrada[1024];

  // lê até EOF, \0 ou \n — cobre todos os casos possíveis porque a entrada no runcodes é de um jeito e pelo teclado para testes é de outro, então ao invés de usar fgets fizemos a adaptação para todos os tipos de terminação de entrada
  while ((c = getchar()) != EOF && c != '\0' && c != '\n') {
      if (i < sizeof(bufferEntrada) - 1) {
        bufferEntrada[i++] = c;
      } else {
        break; // evita estouro de buffer(seg fault)
      }
  }
  bufferEntrada[i] = '\0'; //coloca o \0 no fim da entrada para usar strtok corretamente


  //salvando primeiro parâmetro da entrada
  parametro = strtok(bufferEntrada, " ");
  //atribuindo o código da funcionalidade como inteiro para codFuncionalidade
  codFuncionalidade = atoi(parametro);

  //switch para executar a funcionalidade correta requisitada pelo usuário
  switch(codFuncionalidade){
    //funcionalidade 1: criar arquivo de index apenas com cabeçalho
    case 1:
      //obtem o nome do arquivo de indice passado como parâmetro
      nomeArquivoIndicePessoa = strtok(NULL, " ");
      criarIndice(nomeArquivoIndicePessoa);

      break;
    //funcionalidade 2: faz a leitura do arquivo csv, atualiza o arquivo de índice e e cria o arquivo binário de dados 
    case 2:
      //obtem o nome do arquivo de entrada passado como parametro
      nomeArquivoEntradaPessoa = strtok(NULL, " ");
      //obtem o nome do arquivo de saida passado como parametro
      nomeArquivoSaidaPessoa = strtok(NULL, " ");
      //obtem o nome do arquivo de índice passado como parametro
      nomeArquivoIndicePessoa = strtok(NULL, " ");

      //chama a funcionalidade 2
      criarArquivoDados(nomeArquivoEntradaPessoa, nomeArquivoSaidaPessoa, nomeArquivoIndicePessoa);

      break;
    //funcionalidade 3: printa na tela todos os registros que vão ser lidos do arquivo de dados
    case 3: 
      //obtem o nome do arquivo de entrada passado como parametro
      nomeArquivoEntradaPessoa = strtok(NULL, " ");
      //chama a funcionalidade 3
      listarRegistros(nomeArquivoEntradaPessoa);
      break;
    case 4:
      //obtem o nome do arquivo de entrada passado como parametro
      nomeArquivoEntradaPessoa = strtok(NULL, " ");
      //obtem o nome do arquivo de indice passado como parametro
      nomeArquivoIndicePessoa = strtok(NULL, " ");
      //obtem o parametro n
      parametro = strtok(NULL, " ");
      int n = atoi(parametro);
      buscarRegistros(nomeArquivoEntradaPessoa, nomeArquivoIndicePessoa, n);
      break;
    case 5:
      //obtem o nome do arquivo de entrada passado como parametro
      nomeArquivoEntradaPessoa = strtok(NULL, " ");
      //obtem o nome do arquivo de indice passado como parametro
      nomeArquivoIndicePessoa = strtok(NULL, " ");
      //obtem o parametro n
      parametro = strtok(NULL, " ");
      int m = atoi(parametro);
      deletarRegistro(nomeArquivoEntradaPessoa, nomeArquivoIndicePessoa, m);
      break;
    case 6:
      //obtém nome do arquivo binário de pessoas gerado anteriormente:
      nomeArquivoEntradaPessoa = strtok(NULL, " ");
      //obtém nome do arquivo binário de índice gerado anteriormente:
      nomeArquivoIndicePessoa = strtok(NULL, " ");
      //obtém n que diz quantas vezes a inserção vai acontecer
      char *numLoop = strtok(NULL, " ");
      int p = atoi(numLoop);
      //chama funcionalidade 6:
      inserirUnicoRegistro(nomeArquivoEntradaPessoa, nomeArquivoIndicePessoa, p);
      break;
    case 7:
      //obtém nome do arquivo binário de pessoas gerado anteriormente:
      nomeArquivoEntradaPessoa = strtok(NULL, " ");
      //obtém nome do arquivo binário de índice gerado anteriormente:
      nomeArquivoIndicePessoa = strtok(NULL, " ");
      //obtém n que diz quantas vezes a inserção vai acontecer
      parametro = strtok(NULL, " ");
      int q = atoi(parametro);
      //chama a funcionalidade 7
      atualizarRegistro(nomeArquivoEntradaPessoa, nomeArquivoIndicePessoa, q);
      break;
    case 8:
      //obtém nome do arquivo de entrada(csv)
      nomeArquivoEntradaSegue = strtok(NULL, " ");
      //obtém nome do arquivo de saída(bin)
      nomeArquivoSaidaSegue = strtok(NULL, " ");
      //chama funcionalidade 8:
      criaArquivoSegue(nomeArquivoEntradaSegue, nomeArquivoSaidaSegue);
      break;
    case 9:
      //obtém nome do arquivo desordenado
      nomeArquivoEntradaSegue = strtok(NULL, " ");
      //obtém nome do arquivo ordenado
      nomeArquivoOrdenadoSegue = strtok(NULL, " ");
      //chama func 9
      ordenaArquivoSegue(nomeArquivoEntradaSegue, nomeArquivoOrdenadoSegue);
      break;
    case 10:
      //obtém nome do arquivo de entrada pessoa(bin)
      nomeArquivoEntradaPessoa = strtok(NULL, " ");
      //obtém nome do arquivo de índice passado como parâmetro
      nomeArquivoIndicePessoa = strtok(NULL, " ");
      //obtém nome do arquivo ordenado de segue
      nomeArquivoOrdenadoSegue = strtok(NULL, " ");
      //obtém n que diz quantas vezes a busca vai acontecer
      parametro = strtok(NULL, " ");
      int r = atoi(parametro);
      //chama a funcionalidade 10
      juncaoArquivos(nomeArquivoEntradaPessoa, nomeArquivoIndicePessoa, nomeArquivoOrdenadoSegue, r);
      break;
    //caso o usuário digite alguma opção que não é de 1 a 10
    default: 
      printf("Opcao invalida");
  }
  
  return 0;
}