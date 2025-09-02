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
        printf("Não deu pra abir o arquivo\n");
        exit(1);
    }
    fseek(arq, 0, SEEK_END);
    size_t size = ftell(arq);
    char* arqMem = (char*)malloc(size);
    if (!arqMem) {
        printf("Não deu pra alocar memória\n");
        exit(2);
    }
    rewind(arq);
    size_t sizeRead = fread(arqMem, 1, size, arq);
    if (sizeRead != size) {
        printf("SizeRead: %zu\n Size: %zu\n", sizeRead, size);
        printf("Não conseguiu ler tudo pro buffer\n");
        exit(3);
    }
    fclose(arq);
    FILE* arq2 = fopen(argv[2], "w+");
    if (!arq) {
        printf("Não deu pra abrir o arquivo 2\n");
        exit(1);
    }
    size_t sizeWritten = fwrite(arqMem, 1, size, arq2);
    if (sizeWritten != size) {
        printf("Não conseguiu ecrever tudo\n");
        exit(4);
    }
    free(arqMem);
    fclose(arq2);
    return 0;
}