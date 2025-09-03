#include "lib_lista_char.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nodo_char *cria_nodo_char(char c) {
  struct nodo_char *nodo = malloc(sizeof(*nodo));
  if (!nodo) {
    printf("Erro de alocacao na funcao cria_nodo_char.\n");
    exit(3);
  }
  nodo->c = c;
  nodo->lista_pos = cria_lista_pos();
  nodo->prox = NULL;
  return nodo;
}

struct lista_char *cria_lista_char() {
  struct lista_char *lista = malloc(sizeof(*lista));
  if (!lista) {
    printf("Erro de alocacao na funcao cria_lista_char.\n");
    exit(3);
  }
  lista->ini = NULL;
  return lista;
}

struct lista_char *destroi_lista_char(struct lista_char *lista) {
  struct nodo_char *nodo;
  while (lista->ini != NULL) {
    nodo = lista->ini;
    lista->ini = lista->ini->prox;
    nodo->lista_pos = destroi_lista_pos(nodo->lista_pos);
    free(nodo);
  }
  free(lista);
  return NULL;
}

struct nodo_char *busca_nodo_sem_criar(char c, struct lista_char *lista) {
  if ((lista->ini == NULL) || (lista->ini->c > c))
    return NULL;
  struct nodo_char *nodo = lista->ini;
  while ((nodo != NULL) && (nodo->c != c))
    nodo = nodo->prox;
  return nodo;
}

struct nodo_char *encontra_nodo(char c, struct lista_char *lista) {
  struct nodo_char *nodo;
  if ((lista->ini == NULL) || (c < lista->ini->c)) {
    struct nodo_char *novo = cria_nodo_char(c);
    if (novo == NULL)
      return NULL;
    novo->prox = lista->ini;
    lista->ini = novo;
    return lista->ini;
  }
  nodo = lista->ini;
  if (nodo->c == c)
    return nodo;
  while ((nodo->prox != NULL) && (nodo->prox->c < c))
    nodo = nodo->prox;
  if ((nodo->prox == NULL) || (nodo->prox->c != c)) {
    struct nodo_char *novo = cria_nodo_char(c);
    if (novo == NULL)
      return NULL;
    novo->prox = nodo->prox;
    nodo->prox = novo;
  }
  return nodo->prox;
}

char busca_char(int pos, struct lista_char *lista) {
  struct nodo_char *nodo = lista->ini;
  while (nodo != NULL) {
    if (pertence_a_lista(pos, nodo->lista_pos))
      return nodo->c;
    nodo = nodo->prox;
  }
  return -1;
}

void imprime_lista(struct lista_char *lista) {
  struct nodo_char *nodo = lista->ini;
  while (nodo != NULL) {
    printf("A c eh %c, e suas posicoes sao:", nodo->c);
    imprime_lista_pos(nodo->lista_pos);
    printf("\n");
    nodo = nodo->prox;
  }
}
