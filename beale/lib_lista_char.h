#ifndef LIB_LISTA_CHAR
#define LIB_LISTA_CHAR
#include "lib_lista_pos.h"

struct nodo_char{
	char c;
	struct lista_pos* lista_pos;
	struct nodo_char* prox;
};

struct lista_char{
	struct nodo_char* ini;
};

/* Aloca dinamicamente o espaco de um nodo_char
 * e insere o valor passado por parametro em c. 
 * Retorna o nodo, e em caso de falha termina o programa com saida 3. */
struct nodo_char* cria_nodo_char(char c);

/* Aloca dinamicamente o espaco de uma lista_char.
 * Retorna a lista, e em caso de falha termina o programa com saida 3. */
struct lista_char* cria_lista_char();

/* Destroi a lista_char e retorna NULL. */
struct lista_char* destroi_lista_char(struct lista_char* lista);

/* Retorna ponteiro para o nodo com o char buscado. 
 * O nodo eh criado e inserido em ordem caso ele nao exista. */
struct nodo_char* encontra_nodo(char c, struct lista_char* lista);

/* Retorna ponteiro para o nodo com o char buscado.
 * Retorna NULL caso o nodo nao exista. */
struct nodo_char* busca_nodo_sem_criar(char c, struct lista_char* lista);

/* Retorna o caracter respectivo ao int pos. 
 * Caso esse caracter nao exista retorna o int -1. */
char busca_char(int pos, struct lista_char* lista);

/* Imprime a lista de caracteres e cada lista de posicoes
 * ao lado do caractere. */
void imprime_lista(struct lista_char* lista);

#endif