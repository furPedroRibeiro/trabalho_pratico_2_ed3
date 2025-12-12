//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_4.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"


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