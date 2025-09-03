#include <stdio.h>
#include <stdlib.h>
#include "lib_lista_pos.h"

struct nodo_pos* cria_nodo_pos(int pos) {
	struct nodo_pos* nodo = malloc(sizeof(*nodo));
	if (!nodo) {
		printf("Erro de alocacao na funcao cria_nodo_pos\n");
		exit(3);
	}
	nodo->pos = pos;
	nodo->prox = NULL;
	return nodo;
}

struct lista_pos* cria_lista_pos() {
	struct lista_pos* lista = malloc(sizeof(*lista));
	if (!lista) {
		printf("Erro de alocacao na funcao cria_lista_pos\n");
		exit(3);
	}
	lista->ini = NULL;
	lista->tam = 0;
	return lista;
}

struct lista_pos* destroi_lista_pos(struct lista_pos* lista) {
	struct nodo_pos* nodo;
	if (lista_pos_vazia(lista)) {
		free(lista);
		return lista;
	}
	nodo = lista->ini;
	while (nodo->prox != NULL) {
		nodo = nodo->prox;
		free(lista->ini);
		lista->ini = nodo;
	}
	free(lista->ini);
	free(lista);
	return NULL;
}

int lista_pos_vazia(struct lista_pos* lista) {
	if (lista->tam == 0)
		return 1;
	return 0;
}

int pertence_a_lista(int pos, struct lista_pos* lista) {
	struct nodo_pos* nodo = lista->ini;
	while (nodo != NULL) {
		if (nodo->pos == pos)
			return 1;
		nodo = nodo->prox;
	}
	return 0;
}

void insere_na_lista_pos(int pos, struct lista_pos* lista){
	struct nodo_pos* nodo = cria_nodo_pos(pos);
	if (lista_pos_vazia(lista)) {
		lista->ini = nodo;
	}
	else {
		nodo->prox = lista->ini;
		lista->ini = nodo;
	}
	lista->tam++;
}

void imprime_lista_pos(struct lista_pos* lista) {
	struct nodo_pos* nodo = lista->ini; 
	while (nodo != NULL) {
		printf("%d ", nodo->pos);
		nodo = nodo->prox;
	}
}

int aleat_pos(struct lista_pos* lista) {
	if (lista->tam == 1) {
		return lista->ini->pos;
	}
	int i = (rand() % (lista->tam - 1)) + 1;
	int j = 1;
	struct nodo_pos* nodo = lista->ini;
	while (j < i) {
		nodo = nodo->prox;
		j++;
	}
	return nodo->pos;
}