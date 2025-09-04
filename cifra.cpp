/**
 * @file cifra.cpp
 * @brief Implementação das funções para cifrar mensagens UTF-8.
 * @author Luiz Henrique Murback Wiedmer
 * @date 2025-09-01
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Tipo de operação
 */
enum op { SUM, SUB };

/**
 * @brief Faz a substituição de cada byte do buffer
 * @param source Origem dos bytes
 * @param n Tamanho de source
 * @param dest Destino dos bytes após a substituição
 * @param keyWord Palavra usada para fazer a cifra
 * @param opr Operação feita em source, pode ser SUM ou SUB
 */
void encodeVigenere(unsigned char* source, size_t n, unsigned char* dest,
                    char* keyWord, enum op opr) {
    unsigned char* decBuffer = (unsigned char*)malloc(n);
    if (!decBuffer) {
        printf("Não conseguiu alocar o buffer de decode\n");
        exit(2);
    }
    size_t keySize = strlen(keyWord);
    if (keySize < n) {
        // Buffer eh chave + texto claro
        memcpy(decBuffer, keyWord, keySize);
        memcpy(decBuffer + keySize, source, n - keySize);
    } else {
        // Buffer eh a igual ou menor que a chave
        memcpy(decBuffer, keyWord, n);
    }
    if (opr == SUM) {
        for (size_t i = 0; i < n; i++) {
            dest[i] = source[i] + decBuffer[i];
        }
    } else {
        for (size_t i = 0; i < n; i++) {
            dest[i] = source[i] - decBuffer[i];
        }
    }
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
        printf("Não deu pra alocar o buffer de leitura\n");
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
    fclose(arq);

    if (!arq) {
        printf("Não deu pra abir o arquivo de leitura\n");
        exit(1);
    }
    unsigned char* arqMem;  // buffer de bytes
    size_t n = readFileToBuffer(arq, arqMem);
    unsigned char* write_buffer = (unsigned char*)malloc(n);
    if (!write_buffer) {
        printf("Não deu pra alocar o buffer de escrita");
        exit(2);
    }
    fclose(arq);
    char* keyWord = argv[3];
    enum op opr;
    if (!strcmp("sum", argv[4])) {
        opr = SUM;
    } else if (!strcmp("sub", argv[4])) {
        opr = SUB;
    } else {
        printf("Uso errado\n");
        exit(5);
    }
    encodeVigenere(arqMem, n, write_buffer, keyWord, opr);

    FILE* arq2 = fopen(argv[2], "w+");
    if (!arq) {
        printf("Não deu pra abrir o arquivo de escrita\n");
        exit(1);
    }
    size_t sizeWritten = fwrite(arqMem, 1, n, arq2);
    if (sizeWritten != n) {
        printf("Não conseguiu escrever tudo\n");
        exit(4);
    }
    free(arqMem);
    fclose(arq2);
    return 0;
}