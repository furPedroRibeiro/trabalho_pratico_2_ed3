//Aluno 1: Pedro Luis de Alencar Ribeiro N° USP: 15590852
//Aluno 2: Bianca Duarte Batista Lacerda N° USP: 15443221

#include "../headers/funcionalidade_5.h"
#include "../headers/utilidades.h"
#include "../headers/geral.h"


//FUNCIONALIDADE 5:
void deletarRegistro(char *nomeArquivoPessoa, char *nomeArquivoIndice, int n){
    //Abertura dos arquivos
    FILE *arqPessoa = abrirArquivoComStatus(nomeArquivoPessoa, "rb+");
    FILE *arquivoIndice = abrirArquivoComStatus(nomeArquivoIndice, "rb+");
    
    //Marca os arquivos como inconsistentes durante a operação
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
    
    fseek(arquivoIndice, 12, SEEK_SET); // pula o cabeçalho para montar vetor de índices
    indice *vetorIndice = malloc(cabecalho->quantidadePessoas * sizeof(indice));
    for(int i = 0; i < cabecalho->quantidadePessoas; i++){
        fread(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fread(&vetorIndice[i].byteOffset, sizeof(long int), 1, arquivoIndice);
    }

    //Vetor para marcar IDs que devem ser removidos
    int *idsParaRemover = malloc(cabecalho->quantidadePessoas * sizeof(int));
    int qtdIdsParaRemover = 0;
    
    //obtém o tamanho do arquivo pessoa
    fseek(arqPessoa, 0, SEEK_END);
    long sizeDados = ftell(arqPessoa);
    
    //loop de remoções
    for(int i = 0; i < n; i++){
        int entrada;
        char nomeCampo[100], valorCampo[100];
        
        scanf("%d", &entrada);
        scanf(" %[^=]", nomeCampo);
        getchar(); // Consome o '='
        
        //função para tratamento de string, foi necessário criar uma nova função pois como os valores podem
        //ser passados no modelo idadePessoa=NULO ou nomePessoa=NULO, a função scan_quote_string não tratava corretamente
        //pois o valor do campo, ou seja o nulo, era recebido sem aspas 
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
        
        //por segurança, trocamos o que recebemos da função acima por ""
        if(strcmp(valorCampo, "NULO") == 0){
            strcpy(valorCampo, "");
        }
        
        //usa a função de busca buscarRegistrosPorCampo
        resultadoBusca *resultados = buscarRegistrosPorCampo(arqPessoa, vetorIndice, cabecalho->quantidadePessoas, sizeDados, nomeCampo, valorCampo);
        
        //Processa todos os registros encontrados
        if(resultados != NULL){
            resultadoBusca *atual = resultados;
            while(atual != NULL){
                //marca o registro como removido no arquivo de dados
                fseek(arqPessoa, atual->byteOffset, SEEK_SET);
                char removido = '1';
                fwrite(&removido, sizeof(char), 1, arqPessoa);
                fflush(arqPessoa);
                
                //guarda os ids que precisam ser removidos do arquivo de indice
                idsParaRemover[qtdIdsParaRemover++] = atual->reg->idPessoa;
                
                atual = atual->proxResultado;
            }
            
            //Libera a lista de resultados
            liberarListaResultados(resultados);
        }
    }
    
    // Remove todos os IDs marcados do vetor de índices
    for(int i = 0; i < qtdIdsParaRemover; i++){
        for(int j = 0; j < cabecalho->quantidadePessoas; j++){
            if(vetorIndice[j].idPessoa == idsParaRemover[i]){
                for(int k = j; k < cabecalho->quantidadePessoas - 1; k++){
                    vetorIndice[k] = vetorIndice[k + 1];
                }
                cabecalho->quantidadePessoas--;
                break;
            }
        }
    }

    // atualiza cabeçalho do arquivo pessoas
    cabecalho->quantidadeRemovidos += qtdIdsParaRemover;

    //escreve cabeçalho do arquivo pessoas atualizado
    fseek(arqPessoa, 1, SEEK_SET);
    fwrite(&cabecalho->quantidadePessoas, sizeof(int), 1, arqPessoa);
    fwrite(&cabecalho->quantidadeRemovidos, sizeof(int), 1, arqPessoa);
    
    // Reescreve o arquivo de índice
    fclose(arquivoIndice);
    remove(nomeArquivoIndice);

    arquivoIndice = fopen(nomeArquivoIndice, "wb+");    
    // Escreve o cabeçalho do índice
    char statusIndice;
    statusIndice = '0';
    fseek(arquivoIndice, 0, SEEK_SET);
    fwrite(&statusIndice, sizeof(char), 1, arquivoIndice);
    char lixo[12] = "$$$$$$$$$$$";
    fwrite(lixo, sizeof(char), 11, arquivoIndice);
    
    // Escreve os índices após o cabeçalho
    for(int i = 0; i < cabecalho->quantidadePessoas; i++){
        fwrite(&vetorIndice[i].idPessoa, sizeof(int), 1, arquivoIndice);
        fwrite(&vetorIndice[i].byteOffset, sizeof(long int), 1, arquivoIndice);
    }
    
    // Marca os arquivos como consistentes
    defineStatusArquivo(arqPessoa, '1');
    defineStatusArquivo(arquivoIndice, '1');
    
    free(vetorIndice);
    free(idsParaRemover);
    free(cabecalho);

    fclose(arqPessoa);
    fclose(arquivoIndice);
    
    // Mostra os arquivos binários
    binarioNaTela(nomeArquivoPessoa);
    binarioNaTela(nomeArquivoIndice);
}