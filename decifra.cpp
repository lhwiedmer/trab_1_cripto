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
 * @brief Decifra o conteudo de source
 * @param source Origem dos bytes
 * @param n Tamanho de source
 * @param dest Destino dos bytes após a substituição
 * @param keyWord Palavra usada para fazer a cifra
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
    printf("./decifra <mensagemCifrada> <arqDest> <keyWord>\n");
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
    decodeVigenere(arqMem, n, write_buffer, keyWord);
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