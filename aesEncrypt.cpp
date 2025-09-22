/**
 * @file aesEncrypt.cpp
 * @brief Implementação de um programa para cifrar mensagens UTF-8 com AES.
 * @author Luiz Henrique Murback Wiedmer
 * @date 2025-09-21
 * @version 1.0
 */

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chrono>

struct timeval tval_before, tval_after, tval_result;

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

void handleErrors(int n) {
    printf("Houve o erro %d na encriptacao\n", n);
    exit(n);
}

int encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key,
            unsigned char* ciphertext) {
    EVP_CIPHER_CTX* ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors(1);

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
        handleErrors(2);

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors(3);
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors(4);
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

void printCorrectUse() {
    printf("Uso correto:\n");
    printf("./decifra <mensagemCifrada> <arqDest> <keyWord>\n");
}

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
    unsigned char* writeBuffer = (unsigned char*)malloc(n + 16);
    if (!writeBuffer) {
        printf("Não deu pra alocar o buffer de escrita");
        exit(2);
    }
    fclose(arq);

    int keyLen = strlen(argv[3]);
    unsigned char keyWord[16];
    memset(keyWord, 0, 16);
    memcpy(keyWord, argv[3], keyLen);

    auto begin = std::chrono::high_resolution_clock::now();

    size_t nEncrypt = encrypt(arqMem, n, keyWord, writeBuffer);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    printf("Tempo: %.5f seconds.\n", elapsed.count() * 1e-9);

    free(arqMem);

    FILE* arq2 = fopen(argv[2], "w+");
    if (!arq2) {
        printf("Não deu pra abrir o arquivo de escrita\n");
        exit(1);
    }
    size_t sizeWritten = fwrite(writeBuffer, 1, nEncrypt, arq2);
    if (sizeWritten != nEncrypt) {
        printf("Não conseguiu escrever tudo\n");
        exit(4);
    }
    free(writeBuffer);
    fclose(arq2);
    return 0;
}