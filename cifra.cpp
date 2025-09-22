/**
 * @file cifra.cpp
 * @brief Implementação de um programa para cifrar mensagens UTF-8.
 * @author Luiz Henrique Murback Wiedmer
 * @date 2025-09-01
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chrono>

/**
 * @brief Cifra o conteudo de source
 * @param source Origem dos bytes
 * @param n Tamanho de source
 * @param dest Destino dos bytes após a substituição
 * @param keyWord Palavra usada para fazer a cifra
 */
void encodeVigenere(unsigned char* source, size_t n, unsigned char* dest,
                    char* keyWord) {
    unsigned char* decBuffer = (unsigned char*)malloc(n);
    if (!decBuffer) {
        printf("Não conseguiu alocar o buffer de encode\n");
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
    for (size_t i = 0; i < n; i++) {
        dest[i] = source[i] + decBuffer[i];
    }
}

/**
 * @brief Le o conteudo de arqRead e o retorna em um buffer
 * @param arqRead Arquivo com conteudo a ser lido
 * @param size Tamanho do buffer retornado
 * @return Ponteiro para buffer de bytes com o conteúdo
 */
unsigned char* readFileToBuffer(FILE* arqRead, size_t* size) {
    fseek(arqRead, 0, SEEK_END);
    *size = ftell(arqRead);
    unsigned char* arqMem = (unsigned char*)malloc(*size);
    if (!arqMem) {
        printf("Não deu pra alocar o buffer de leitura\n");
        exit(2);
    }
    rewind(arqRead);
    size_t sizeRead = fread(arqMem, 1, *size, arqRead);
    if (sizeRead != *size) {
        printf("SizeRead: %zu\n Size: %zu\n", sizeRead, *size);
        printf("Não conseguiu ler tudo pro buffer\n");
        exit(3);
    }
    return arqMem;
}

void printCorrectUse() {
    printf("Uso correto:\n");
    printf("./cifra <arqClaro> <arqDest> <keyWord>\n");
}

/**
 * 1 - Lê arquivo com nome passado no argv e coloca em um buffer
 * 2 - Faz cifra de substituição
 * 3 - Faz cifra de transposição
 * 4 - Escreve no arquivo cifrado
 * Obs(Deve ser passado no arg, os nome dos arquivos e uma chave)
 */
int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Número de argumentos inválido\n");
        printCorrectUse();
        exit(5);
    }
    FILE* arq;
    arq = fopen(argv[1], "r");
    if (!arq) {
        printf("Não deu pra abir o arquivo de leitura\n");
        exit(1);
    }
    size_t n;
    unsigned char* arqMem = readFileToBuffer(arq, &n);  // buffer de bytes
    unsigned char* write_buffer = (unsigned char*)malloc(n);
    if (!write_buffer) {
        printf("Não deu pra alocar o buffer de escrita");
        exit(2);
    }
    fclose(arq);
    char* keyWord = argv[3];

    auto begin = std::chrono::high_resolution_clock::now();

    encodeVigenere(arqMem, n, write_buffer, keyWord);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);

    free(arqMem);

    FILE* arq2 = fopen(argv[2], "w+");
    if (!arq2) {
        printf("Não deu pra abrir o arquivo de escrita\n");
        exit(1);
    }
    size_t sizeWritten = fwrite(write_buffer, 1, n, arq2);
    if (sizeWritten != n) {
        printf("Não conseguiu escrever tudo\n");
        exit(4);
    }
    free(write_buffer);
    fclose(arq2);
    return 0;
}