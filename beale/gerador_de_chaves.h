#ifndef GERADOR_DE_CHAVES
#define GERADOR_DE_CHAVES
#include "lib_lista_char.h"

/**
 * @brief Gera uma lista de chaves a partir de um livro cifra
 * @param n_arq String com o nome do arquivo que serve de livro de cifra
 */
struct lista_char* gera_lista_de_chaves(char* n_arq);

/* Escreve a lista de chaves em um arquivo.
 * Se o arquivo ja exite o conteudo eh apagado
 * e a lista eh escrita por cima, se nao existir
 * ele eh criado e a lista eh escrita nele.
 * Em caso de erro de abertura de arquivo, termina o programa com saída 1.
 * Em caso de erro de fechamento de arquivo, termina o programa com saída 2.*/
void escreve_lista_no_arquivo(char* n_arq, struct lista_char* lista);

/* Transcreve um arquivo com a lista de chaves para uma lista_char e a retorna.
 * Em caso de erro de abertura de arquivo, termina o programa com saída 1.
 * Em caso de erro de fechamento de arquivo, termina o programa com saída 2.*/
struct lista_char* escreve_arquivo_na_lista(char* n_arq);

#endif