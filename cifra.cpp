/**
 * @file cifra.cpp
 * @brief Implementação de um programa para cifrar mensagens UTF-8.
 * @author Luiz Henrique Murback Wiedmer
 * @author Vinicius Evair da Silva
 * @date 2025-09-01
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <string>

#include "commons.hpp"

/**
 * @brief Cifra o conteudo de source com o método de Vigenere
 * @param[in]  source Origem dos bytes
 * @param[in]  n Tamanho de source
 * @param[in]  keyWord Palavra usada para fazer a cifra
 * @param[out] dest Destino dos bytes após a substituição
 */
void encodeVigenere(const unsigned char* source, size_t n, unsigned char* dest,
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
 * @brief Cifra um bloco de texto com o método de trilhos.
 * @param[in]  sourceChunk Origem dos bytes
 * @param[in]  chunkSize Tamanho de sourceChunk
 * @param[in]  railSize Tamanho do trilho
 * @return String com o conteúdo do bloco criptografado.
 */
std::string encodeRailChunk(const unsigned char* sourceChunk, size_t chunkSize,
                            size_t railSize) {
    std::string encodedChunk(chunkSize, 0);
    size_t period = 2 * (railSize - 1);
    size_t destIdx = 0;

    for (size_t rail = 0; rail < railSize; rail++) {
        size_t i = rail;
        while (i < chunkSize) {
            encodedChunk[destIdx++] = sourceChunk[i];

            /* Os trilhos do meio têm dois caracteres por ciclo */
            if (rail != FIRST_RAIL && rail != LAST_RAIL) {
                size_t secondPos = i + period - 2 * rail;
                if (secondPos < chunkSize) {
                    encodedChunk[destIdx++] = sourceChunk[secondPos];
                }
            }
            i += period;
        }
    }
    return encodedChunk;
}

/**
 * @brief Cifra o conteúdo de source com o método de trilhos.
 * @param[in]  source Origem dos bytes
 * @param[in]  source_size Tamanho de source
 * @param[in]  keyWord Palavra usada para fazer a cifra
 * @return String com o conteúdo de source criptografado.
 */
std::string encodeRail(const unsigned char* source, size_t source_size,
                       char* keyWord) {
    size_t keySize = strlen(keyWord);
    size_t railSize = std::max(keySize, MIN_RAIL_SIZE);
    std::string dest;
    dest.reserve(source_size);
    size_t offset = 0;

    while (offset < source_size) {
        size_t chunkSize = std::min(BUFF_SIZE, source_size - offset);
        std::string encodedChunk =
            encodeRailChunk(source + offset, chunkSize, railSize);
        dest.append(encodedChunk);

        // MIN_RAIL_SIZE < first word length < MAX_RAIL_SIZE
        size_t wordLen = first_word(encodedChunk).size();
        railSize = std::min(MAX_RAIL_SIZE, std::max(wordLen, MIN_RAIL_SIZE));

        offset += chunkSize;
    }

    return dest;
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
        printCorrectUse(ENCODE);
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
    fclose(arq);
    unsigned char* writeBuffer = (unsigned char*)malloc(n);
    if (!writeBuffer) {
        printf("Não deu pra alocar o buffer de escrita");
        exit(2);
    }
    char* keyWord = argv[3];
    auto begin = std::chrono::high_resolution_clock::now();
    std::string railResult;
    for (int i = 0; i < 10; i++) {
        railResult = encodeRail(arqMem, n, keyWord);
    }
    encodeVigenere(reinterpret_cast<const unsigned char*>(railResult.c_str()),
                   n, writeBuffer, keyWord);

    unsigned char* source = writeBuffer;
    unsigned char* dest = arqMem;

    for (int i = 0; i < 8; i++) {
        encodeVigenere(source, n, dest, keyWord);
        unsigned char* temp = source;
        source = dest;
        dest = temp;
    }
    encodeVigenere(source, n, dest, keyWord);

    free(source);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("Tempo: %.5f seconds.\n", elapsed.count() * 1e-9);

    FILE* arq2 = fopen(argv[2], "w+");
    if (!arq2) {
        printf("Não deu pra abrir o arquivo de escrita\n");
        exit(1);
    }
    size_t sizeWritten = fwrite(dest, 1, n, arq2);
    if (sizeWritten != n) {
        printf("Não conseguiu escrever tudo\n");
        exit(4);
    }
    free(dest);
    fclose(arq2);
    return 0;
}