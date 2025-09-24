#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "commons.hpp"

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

void printCorrectUse(bool encode) {
    printf("Uso correto:\n");
    if (encode) {
        printf("./cifra <arqClaro> <arqDest> <keyWord>\n");
    } else {
        printf("./decifra <mensagemCifrada> <arqDest> <keyWord>\n");
    }
}

std::string first_word(const std::string& s) {
    auto it = std::find_if_not(s.begin(), s.end(), ::isspace);
    if (it == s.end()) return "";

    auto start = it;
    auto end = std::find_if(it, s.end(), ::isspace);

    return std::string(start, end);
}

std::string first_word(const unsigned char* buf, size_t len) {
    const char* whitespace = " \t\n\r\v\f";
    size_t i = 0;
    while (i < len && strchr(whitespace, buf[i])) {
        ++i;
    }
    size_t start = i;
    while (i < len && !strchr(whitespace, buf[i])) {
        ++i;
    }
    return std::string(reinterpret_cast<const char*>(buf + start), i - start);
}
