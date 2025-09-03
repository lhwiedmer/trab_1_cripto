#include "gerador_de_chaves.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void completa_lista(struct lista_char *lista) {
  short int j = -2;
  unsigned char c = 33;
  struct nodo_char *nodo;
  while (c < 127) {
    nodo = encontra_nodo(c, lista);
    if (nodo->lista_pos->tam == 0) {
      insere_na_lista_pos(j, nodo->lista_pos);
      j--;
    }
    c++;
  }
}

struct lista_char *gera_lista_de_chaves(char *n_arq) {
  FILE *ArqLivro = fopen(n_arq, "r");
  if (ArqLivro == NULL) {
    printf("Erro ao abrir Livro Cifra na funcao gera_lista_de_chaves.\n");
    exit(1);
  }
  char *p = malloc(200);
  char c;
  size_t i = 0;
  struct lista_char *lista = cria_lista_char();
  struct nodo_char *nodo;
  while (!feof(ArqLivro)) {
    fscanf(ArqLivro, "%s", p);
    c = p[0];
    if (c >= 0) {
      if ((c <= 'Z') && (c >= 'A'))
        c = c + 32;
      nodo = encontra_nodo(c, lista);
      insere_na_lista_pos(i, nodo->lista_pos);
      i++;
    } else {
      i++;
    }
  }
  free(p);
  if (fclose(ArqLivro) != 0) {
    printf(
        "Erro ao fechar arquivo Livro Cifra na funcao gera_lista_de_chaves.\n");
    exit(2);
  }
  completa_lista(lista);
  return lista;
}

void escreve_lista_no_arquivo(char *n_arq, struct lista_char *lista) {
  FILE *ArqChaves = fopen(n_arq, "w");
  if (ArqChaves == NULL) {
    printf("Erro ao abrir arquivo de chaves na funcao "
           "escreve_lista_no_arquivo.\n");
    exit(1);
  }
  struct nodo_char *nodo = lista->ini;
  struct nodo_pos *nodo_pos;
  while (nodo != NULL) {
    fprintf(ArqChaves, "%c: ", nodo->c);
    nodo_pos = nodo->lista_pos->ini;
    while (nodo_pos->prox != NULL) {
      fprintf(ArqChaves, "%d ", nodo_pos->pos);
      nodo_pos = nodo_pos->prox;
    }
    fprintf(ArqChaves, "%d", nodo_pos->pos);
    fprintf(ArqChaves, "\n");
    nodo = nodo->prox;
  }
  if (fclose(ArqChaves) != 0) {
    printf("Erro ao fechar arquivo de chaves na funcao "
           "escreve_lista_no_arquivo.\n");
    exit(2);
  }
}

struct lista_char *escreve_arquivo_na_lista(char *n_arq) {
  FILE *ArqChaves = fopen(n_arq, "r");
  if (ArqChaves == NULL) {
    printf("Erro ao abrir arquivo de chaves na funcao "
           "escreve_arquivo_na_lista.\n");
    exit(1);
  }
  struct lista_char *lista = cria_lista_char();
  struct nodo_char *nodo;
  char c;
  int x;
  while (!feof(ArqChaves)) {
    c = getc(ArqChaves);
    nodo = encontra_nodo(c, lista);
    fseek(ArqChaves, 2, SEEK_CUR);
    while ((c != '\n') && (!feof(ArqChaves))) {
      fscanf(ArqChaves, "%d", &x);
      insere_na_lista_pos(x, nodo->lista_pos);
      c = getc(ArqChaves);
    }
  }
  if (fclose(ArqChaves) != 0) {
    printf("Erro ao fechar arquivo de chaves na funcao "
           "escreve_arquivo_na_lista.\n");
    lista = destroi_lista_char(lista);
    exit(2);
  }
  return lista;
}
