#include <stdio.h>
#include <stdlib.h>
#include "decodificador.h"

void decodifica(char* n_arq_cod, char* n_arq_l, char* n_arq_dec, int s) {
	struct lista_char* lista;
	if (s == 0)
		lista = gera_lista_de_chaves(n_arq_l);
	else
		lista = escreve_arquivo_na_lista(n_arq_l);
	FILE* ArqCod = fopen(n_arq_cod, "r");
	if (ArqCod == NULL) {
		printf("Erro ao abrir arquivo de mensagem codificada na funcao decodifica.\n");
		lista = destroi_lista_char(lista);
		exit(1);
	}
	FILE* ArqDec = fopen(n_arq_dec, "w");
	if (ArqDec == NULL) {
		printf("Erro ao abrir arquivo para a escrever a mensagem decodificada na funcao decodifica.\n");
		lista = destroi_lista_char(lista);
		exit(1);
	}
	char c;
	fscanf(ArqCod, "%d", &s);
	while (!feof(ArqCod)) {
		switch (s) {
		case -1:
			fprintf(ArqDec, " ");
			break;
		default:
			c = busca_char(s, lista);
			if (c == -1) {
				fprintf(ArqDec, "?");
				printf("Uma chave desconhecida foi encontrada, isso provavelmente ocorreu porque o livro cifra ou arquivo de chaves dado nao eh o correto.\n");
				printf("O caracter ? será colocado no lugar dela.\n");
			}
			else
				fprintf(ArqDec, "%c", c);
			break;
		}
		fscanf(ArqCod, "%d", &s);
	}
	if (fclose(ArqCod) != 0) {
		printf("Erro ao fechar arquivo codificado na funcao decodifica.\n");
		lista = destroi_lista_char(lista);
		exit(2);
	}
	if (fclose(ArqDec) != 0) {
		printf("Erro ao fechar arquivo decodificado na funcao decodifica.\n");
		lista = destroi_lista_char(lista);
		exit(2);
	}
	destroi_lista_char(lista);
}