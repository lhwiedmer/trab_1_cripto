#ifndef LIB_LISTA_POS
#define LIB_LISTA_POS
#include <stdio.h>

struct nodo_pos{
	int pos;
	struct nodo_pos* prox;
};

/* Lista com as posicoes dos caracteres o livro cifra. */
struct lista_pos{
	struct nodo_pos* ini;
	int tam;
};

/* Aloca dinamicamente o espaco de um nodo_pos, 
 * insere um valor passado por parametro em pos, 
 * e retorna o nodo. Em caso de falha termina o programa com saida 3. */
struct nodo_pos* cria_nodo_pos(int pos);

/* Aloca dinamicamente o espaco de um lista_pos
 * e a retorna. Em caso de falha termina o programa com saida 3. */
struct lista_pos* cria_lista_pos();

/* Destroi a lista_pos e retorna NULL.*/
struct lista_pos* destroi_lista_pos(struct lista_pos* lista);

/* Retorna 1 se a lista estiver vazia 
 * e 0 caso nao esteja. */
int lista_pos_vazia(struct lista_pos* lista);

/* Retorna 1 se pos pertence a lista, e 0 caso nao pertenca. */
int pertence_a_lista(int pos, struct lista_pos* lista);

/* Insere um inteiro no inicio da lista_pos. */
void insere_na_lista_pos(int pos, struct lista_pos* lista);

/* Imprime na tela a lista_pos passada no parametro lista. */
void imprime_lista_pos(struct lista_pos* lista);

/* Retorna um nodo escolhido aleatoriamente na lista_pos
 * passada no parametro lista. */
int aleat_pos(struct lista_pos* lista);

#endif
