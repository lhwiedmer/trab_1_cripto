/**
 * @file cifra.cpp
 * @brief Implementação das funções para cifrar mensagens UTF-8.
 * @author Luiz Henrique Murback Wiedmer
 * @date 2025-09-01
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Faz a substituição de cada byte do buffer
 * @param source Origem dos bytes
 * @param dest Destino dos bytesapós a substituição
 */
void substituteBytes(unsigned char* source, unsigned char* dest) {
    // A ideia eh substituir os bytes individualmente ao invés de caracter
    // indvidual,
}

/**
 * @brief Le o conteudo de arqRead para um buffer
 * @param Arquivo com conteudo a ser lido
 * @return Ponteiro para string com o conteúdo
 */
size_t readFileToBuffer(FILE* arqRead, unsigned char* arqMem) {
    fseek(arqRead, 0, SEEK_END);
    size_t size = ftell(arqRead);
    arqMem = (unsigned char*)malloc(size);
    if (!arqMem) {
        printf("Não deu pra alocar memória\n");
        exit(2);
    }
    rewind(arqRead);
    size_t sizeRead = fread(arqMem, 1, size, arqRead);
    if (sizeRead != size) {
        printf("SizeRead: %zu\n Size: %zu\n", sizeRead, size);
        printf("Não conseguiu ler tudo pro buffer\n");
        exit(3);
    }
    return sizeRead;
}

/**
 * 1 - Lê arquivo com nome passado no argv e coloca em um buffer
 * 2 - Faz cifra de substituição
 * 3 - Faz cifra de transposição
 * 4 - Escreve no arquivo cifrado
 * Obs(Deve ser passado no arg, os nome dos arquivos e uma chave)
 */
int main(int argc, char** argv) {
    FILE* arq;
    arq = fopen(argv[1], "r");
    if (!arq) {
        printf("Não deu pra abir o arquivo de leitura\n");
        exit(1);
    }
    unsigned char* arqMem;  // buffer de bytes
    size_t sizeRead = readFileToBuffer(arq, arqMem);
    fclose(arq);

    /* Colocar aqui o codigo de cifra em si */

    FILE* arq2 = fopen(argv[2], "w+");
    if (!arq) {
        printf("Não deu pra abrir o arquivo de escrita\n");
        exit(1);
    }
    size_t sizeWritten = fwrite(arqMem, 1, sizeRead, arq2);
    if (sizeWritten != sizeRead) {
        printf("Não conseguiu escrever tudo\n");
        exit(4);
    }
    free(arqMem);
    fclose(arq2);
    return 0;
}