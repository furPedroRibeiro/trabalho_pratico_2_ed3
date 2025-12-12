//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_9.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"


//FUNCIONALIDADE 9:
void ordenaArquivoSegue(char *nomeArquivoDesordenado, char *nomeArquivoOrdenado){
    //primeiro é necessário trazer o arquivo todo para memória primária
    //abre o arquivo desordenado:
    FILE *arqDesordenado = fopen(nomeArquivoDesordenado, "rb");
    if(arqDesordenado == NULL){
        puts("Falha no processamento do arquivo.");
        free(arqDesordenado);
        return;
    }


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
