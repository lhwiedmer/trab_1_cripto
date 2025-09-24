/**
 * @file cifra.cpp
 * @brief Implementação de um programa para decifrar mensagens UTF-8 cifradas
 * por "cifra".
 * @author Luiz Henrique Murback Wiedmer
 * @author Vinicius Evair da Silva
 * @date 2025-09-01
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chrono>
#include <cstddef>

#include "commons.hpp"

/**
 * @brief Decifra o conteudo de source
 * @param[in]  source Origem dos bytes
 * @param[in]  n Tamanho de source
 * @param[out] dest Destino dos bytes após a substituição
 * @param[in]  keyWord Palavra usada para fazer a cifra
 */
void decodeVigenere(unsigned char* source, size_t n, unsigned char* dest,
                    char* keyWord) {
    unsigned char* decBuffer = (unsigned char*)malloc(n);
    if (!decBuffer) {
        printf("Não conseguiu alocar o buffer de decode\n");
        exit(2);
    }
    size_t keySize = strlen(keyWord);
    // Faz operação inversa pra recuperar
    for (size_t i = 0; (i < keySize) && (i < n); i++) {
        dest[i] = source[i] - keyWord[i];
    }
    if (keySize < n) {
        for (size_t i = keySize; i < n; i++) {
            dest[i] = source[i] - dest[i - keySize];
        }
    }
}

/**
 * @brief Decifra um bloco de texto com o método de trilhos.
 * @param[in]  sourceChunk Origem dos bytes
 * @param[in]  chunkSize Tamanho de source
 * @param[in]  keyWord Palavra usada para decodificar.
 * @return String com o conteúdo do bloco descriptografado.
 */
std::string decodeRailChunk(const unsigned char* sourceChunk, size_t chunkSize,
                            size_t railSize) {
    std::string decodedChunk(chunkSize, 0);
    size_t period = 2 * (railSize - 1);
    size_t sourceIdx = 0;

    for (size_t rail = 0; rail < railSize; rail++) {
        size_t i = rail;
        while (i < chunkSize) {
            decodedChunk[i] = sourceChunk[sourceIdx++];

            /* Os trilhos do meio têm dois caracteres por ciclo */
            if (rail != FIRST_RAIL && rail != LAST_RAIL) {
                size_t secondPos = i + period - 2 * rail;
                if (secondPos < chunkSize) {
                    decodedChunk[secondPos] = sourceChunk[sourceIdx++];
                }
            }
            i += period;
        }
    }

    return decodedChunk;
}

/**
 * @brief Decifra o conteúdo de source com o método de trilhos.
 * @param[in]  source Origem dos bytes
 * @param[in]  sourceSize Tamanho de source
 * @param[in]  keyWord Palavra usada para decodificar.
 * @return String com o conteúdo de source descriptografado.
 */
std::string decodeRail(const unsigned char* source, size_t sourceSize,
                       char* keyWord) {
    size_t keySize = strlen(keyWord);
    size_t railSize = std::max(keySize, MIN_RAIL_SIZE);
    std::string dest;
    dest.reserve(sourceSize);
    size_t offset = 0;

    while (offset < sourceSize) {
        size_t chunkSize = std::min(BUFF_SIZE, sourceSize - offset);

        // MIN_RAIL_SIZE < first word length < MAX_RAIL_SIZE
        size_t wordLen = first_word(source + offset, chunkSize).size();
        size_t nextRailSize =
            std::min(MAX_RAIL_SIZE, std::max(wordLen, MIN_RAIL_SIZE));

        std::string decodedChunk =
            decodeRailChunk(source + offset, chunkSize, railSize);
        dest.append(decodedChunk);

        railSize = nextRailSize;
        offset += chunkSize;
    }

    return dest;
}

/**
 * 1 - Lê arquivo com nome passado no argv e coloca em um buffer
 * 2 - Faz decifra de transposição
 * 3 - Faz de cifra de substituição
 * 4 - Escreve no arquivo de testo claro
 * Obs(Deve ser passado no arg, os nome dos arquivos e uma chave)
 */
int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Número de argumentos inválido\n");
        printCorrectUse(DECODE);
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

    unsigned char* source = arqMem;
    unsigned char* dest = writeBuffer;

    for (int i = 0; i < 9; i++) {
        decodeVigenere(source, n, dest, keyWord);
        unsigned char* temp = source;
        source = dest;
        dest = temp;
    }
    decodeVigenere(source, n, dest, keyWord);

    free(source);
    std::string railResult = decodeRail(dest, n, keyWord);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("Tempo: %.5f seconds.\n", elapsed.count() * 1e-9);

    FILE* arq2 = fopen(argv[2], "w+");
    if (!arq2) {
        printf("Não deu pra abrir o arquivo de escrita\n");
        exit(1);
    }
    size_t sizeWritten = fwrite(railResult.data(), 1, railResult.size(), arq2);
    if (sizeWritten != railResult.size()) {
        printf("Não conseguiu escrever tudo\n");
        exit(4);
    }
    fclose(arq2);
    return 0;
}