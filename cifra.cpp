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
 * @param[in]  source Origem dos bytes
 * @param[in]  n Tamanho de source
 * @param[in]  keyWord Palavra usada para fazer a cifra
 * @return String com o conteúdo do bloco criptografado.
 */
std::string encodeRailChunk(const unsigned char* source_chunk,
                            size_t chunk_size, size_t rail_size) {
    std::string encoded_chunk(chunk_size, 0);
    size_t period = 2 * (rail_size - 1);
    size_t dest_idx = 0;

    for (size_t rail = 0; rail < rail_size; rail++) {
        size_t i = rail;
        while (i < chunk_size) {
            encoded_chunk[dest_idx++] = source_chunk[i];

            /* Os trilhos do meio têm dois caracteres por ciclo */
            if (rail != FIRST_RAIL && rail != LAST_RAIL) {
                size_t second_pos = i + period - 2 * rail;
                if (second_pos < chunk_size) {
                    encoded_chunk[dest_idx++] = source_chunk[second_pos];
                }
            }
            i += period;
        }
    }
    return encoded_chunk;
}

/**
 * @brief Cifra o conteúdo de source com o método de trilhos.
 * @param[in]  source Origem dos bytes
 * @param[in]  n Tamanho de source
 * @param[in]  keyWord Palavra usada para fazer a cifra
 * @return String com o conteúdo de source criptografado.
 */
std::string encodeRail(const unsigned char* source, size_t source_size,
                       char* keyWord) {
    size_t key_size = strlen(keyWord);
    size_t rail_size = std::max(key_size, MIN_RAIL_SIZE);
    std::string dest;
    dest.reserve(source_size);
    size_t offset = 0;

    while (offset < source_size) {
        size_t chunk_size = std::min(BUFF_SIZE, source_size - offset);
        std::string encoded_chunk =
            encodeRailChunk(source + offset, chunk_size, rail_size);
        dest.append(encoded_chunk);

        // MIN_RAIL_SIZE < first word length < MAX_RAIL_SIZE
        size_t word_len = first_word(encoded_chunk).size();
        rail_size = std::min(MAX_RAIL_SIZE, std::max(word_len, MIN_RAIL_SIZE));

        offset += chunk_size;
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
    unsigned char* write_buffer = (unsigned char*)malloc(n);
    if (!write_buffer) {
        printf("Não deu pra alocar o buffer de escrita");
        exit(2);
    }

    char* keyWord = argv[3];
    auto begin = std::chrono::high_resolution_clock::now();
    std::string railResult = encodeRail(arqMem, n, keyWord);
    free(arqMem);
    encodeVigenere(reinterpret_cast<const unsigned char*>(railResult.c_str()),
                   n, write_buffer, keyWord);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    printf("Tempo: %.5f seconds.\n", elapsed.count() * 1e-9);
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