#ifndef DECODIFICADOR
#define DECODIFICADOR

#include "gerador_de_chaves.h"
#include "lib_lista_char.h"
#include "lib_lista_pos.h"

enum file_flag { BOOK, KEY };

/**
 * @brief Decodifica a mensagem presente no arquivo com nome em n_arq_cod. Se
 * s == BOOK, usa o arquivo com nome em n_arq_l como livro cifra, se s == KEY
 * valor usa o mesmo como arquivo de chaves
 * @note A função é um ponto possível de término de processo
 * @param n_arq_cod Nome do arquivo com a mensagem codificada
 * @param n_arq_l Nome do arquivo de cifra
 * @param n_arq_dec Nome do arquivo no qual será escrita a mensagem decodificada
 */
void decodifica(char* n_arq_cod, char* n_arq_l, char* n_arq_dec, int s);

#endif