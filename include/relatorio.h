#ifndef RELATORIO_H
#define RELATORIO_H

#include "produto.h"

/* Retorna a faixa de preco (0 a 3) conforme o valor */
int  faixa_do_preco(float preco);

/* Imprime as matrizes estatisticas do estoque */
void relatorio_matriz(const Estoque *e);

#endif
