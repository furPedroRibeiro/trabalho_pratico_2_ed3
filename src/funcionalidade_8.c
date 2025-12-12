//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_8.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"


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