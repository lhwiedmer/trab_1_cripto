#ifndef CODIFICADOR
#define CODIFICADOR
#include "lib_lista_char.h"
#include "lib_lista_pos.h"

/**
 * @brief Codifica uma mensagem presente no arquivo de nome
 * n_arq_orig, e escreve a mensagem codificada
 * no arquivo de nome n_arq_cod.
 * @note A função é um ponto possível de término de processo
 * @param n_arq_orig String com o nome do arquivo de origem
 * @param b_arq_cod String com o nome do arquivo de destino
 * @param lista Lista de chaves
 */
void codifica(char* n_arq_orig, char* n_arq_cod, struct lista_char* lista);

#endif