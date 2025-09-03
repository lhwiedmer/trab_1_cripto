#include <stdio.h>
#include <stdlib.h>
#include "codificador.h"

void codifica(char* n_arq_orig, char* n_arq_cod, struct lista_char* lista) {
	FILE* ArqOrig = fopen(n_arq_orig, "r");
	if (ArqOrig == NULL) {
		printf("Erro ao abrir arquivo original na funcao codifica.\n");
		exit(1);
	}
	FILE* ArqCod = fopen(n_arq_cod, "w");
	if (ArqOrig == NULL) {
		printf("Erro ao abrir o arquivo para escrever a mensagem codificada na funcao codifica.\n");
		exit(1);
	}
	char c = getc(ArqOrig);
	struct nodo_char* nodo;
	while (!feof(ArqOrig)) {
		if ((c == ' ') || ((c >= '\t') && (c <= '\v'))) {
			fprintf(ArqCod, "%d ", -1);
		}
		else {
			if (c < 0) {
				printf("Caracter fora da tabela ASCII presente na mensagem a ser codificada, operacao impossivel.\n");
				lista = destroi_lista_char(lista);
				exit(5);
			}
			nodo = busca_nodo_sem_criar(c, lista);
			fprintf(ArqCod, "%d ", aleat_pos(nodo->lista_pos));
		}
		c = getc(ArqOrig);
	}
	if (fclose(ArqCod) != 0) {
		printf("Erro ao fechar arquivo codificado na funcao codifica.\n");
		lista = destroi_lista_char(lista);
		exit(2);
	}
	if (fclose(ArqOrig) != 0) {
		printf("Erro ao fechar arquivo original na funcao codifica.\n");
		lista = destroi_lista_char(lista);
		exit(2);
	}
}
