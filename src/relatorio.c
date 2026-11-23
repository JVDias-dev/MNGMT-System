/*
 * relatorio.c — Matriz estatistica Categoria x Faixa de preco.
 *
 * Gera duas matrizes:
 *   1. Quantidade de produtos (contagem) por celula.
 *   2. Valor total em estoque (preco x quantidade) por celula.
 * Cada matriz inclui totais por linha, coluna e geral.
 * Ao final, destaca a categoria com maior valor e a faixa mais populosa.
 */

#include <stdio.h>
#include "relatorio.h"
#include "menu.h"   /* nome_categoria, nome_faixa */

/* ================================================================
 * Classificacao de preco em faixa
 *   0: ate R$ 50      1: R$ 50,01–150
 *   2: R$ 150,01–400  3: acima de R$ 400
 * ================================================================ */

int faixa_do_preco(float preco)
{
    if (preco <= 50.0f)  return 0;
    if (preco <= 150.0f) return 1;
    if (preco <= 400.0f) return 2;
    return 3;
}

/* ================================================================
 * Impressao das matrizes
 * ================================================================ */

void relatorio_matriz(const Estoque *e)
{
    int   qtd[NUM_CATEGORIAS][NUM_FAIXAS];
    float valor[NUM_CATEGORIAS][NUM_FAIXAS];
    int   total_col_qtd[NUM_FAIXAS];
    float total_col_val[NUM_FAIXAS];
    int   total_geral_qtd = 0;
    float total_geral_val = 0.0f;
    int   cat_maior_valor  = 0;
    float maior_valor_cat  = -1.0f;
    int   faixa_mais_pop   = 0;
    int   c, f;

    printf("\n--- Relatorio Estatistico ---\n");

    if (e->total == 0) {
        printf("Estoque vazio. Cadastre produtos para gerar o relatorio.\n");
        return;
    }

    /* Zerar matrizes */
    for (c = 0; c < NUM_CATEGORIAS; c++)
        for (f = 0; f < NUM_FAIXAS; f++) {
            qtd[c][f]   = 0;
            valor[c][f] = 0.0f;
        }
    for (f = 0; f < NUM_FAIXAS; f++) {
        total_col_qtd[f] = 0;
        total_col_val[f] = 0.0f;
    }

    /* Preencher matrizes percorrendo os produtos */
    for (int i = 0; i < e->total; i++) {
        int cat = e->itens[i].categoria;
        int fx  = faixa_do_preco(e->itens[i].preco);
        qtd[cat][fx]++;
        valor[cat][fx] += e->itens[i].preco * (float)e->itens[i].quantidade;
    }

    /* ---- Matriz 1: quantidade de produtos ---- */
    printf("\nQUANTIDADE DE PRODUTOS\n");
    printf("%-14s | %6s | %6s | %7s | %5s | %5s\n",
           "Categoria", "ate 50", "50-150", "150-400", "+400", "Total");
    printf("%-14s-+-%6s-+-%6s-+-%7s-+-%5s-+-%5s\n",
           "--------------", "------", "------", "-------", "-----", "-----");

    for (c = 0; c < NUM_CATEGORIAS; c++) {
        int total_linha = 0;
        for (f = 0; f < NUM_FAIXAS; f++) {
            total_linha     += qtd[c][f];
            total_col_qtd[f] += qtd[c][f];
        }
        printf("%-14s | %6d | %6d | %7d | %5d | %5d\n",
               nome_categoria(c),
               qtd[c][0], qtd[c][1], qtd[c][2], qtd[c][3],
               total_linha);
        total_geral_qtd += total_linha;
    }

    printf("%-14s | %6d | %6d | %7d | %5d | %5d\n",
           "Total",
           total_col_qtd[0], total_col_qtd[1],
           total_col_qtd[2], total_col_qtd[3],
           total_geral_qtd);

    /* ---- Matriz 2: valor total em estoque ---- */
    printf("\nVALOR TOTAL EM ESTOQUE\n");
    printf("%-14s | %11s | %11s | %11s | %11s | %13s\n",
           "Categoria", "ate 50", "50-150", "150-400", "+400", "Total");
    printf("%-14s-+-%11s-+-%11s-+-%11s-+-%11s-+-%13s\n",
           "--------------", "-----------", "-----------",
           "-----------", "-----------", "-------------");

    for (c = 0; c < NUM_CATEGORIAS; c++) {
        float total_linha = 0.0f;
        for (f = 0; f < NUM_FAIXAS; f++) {
            total_linha     += valor[c][f];
            total_col_val[f] += valor[c][f];
        }
        printf("%-14s | R$ %8.2f | R$ %8.2f | R$ %8.2f | R$ %8.2f | R$ %10.2f\n",
               nome_categoria(c),
               (double)valor[c][0], (double)valor[c][1],
               (double)valor[c][2], (double)valor[c][3],
               (double)total_linha);
        total_geral_val += total_linha;

        if (total_linha > maior_valor_cat) {
            maior_valor_cat = total_linha;
            cat_maior_valor = c;
        }
    }

    printf("%-14s | R$ %8.2f | R$ %8.2f | R$ %8.2f | R$ %8.2f | R$ %10.2f\n",
           "Total",
           (double)total_col_val[0], (double)total_col_val[1],
           (double)total_col_val[2], (double)total_col_val[3],
           (double)total_geral_val);

    /* Encontrar faixa mais populosa (pela contagem de produtos) */
    for (f = 1; f < NUM_FAIXAS; f++) {
        if (total_col_qtd[f] > total_col_qtd[faixa_mais_pop])
            faixa_mais_pop = f;
    }

    /* Linha de destaque */
    printf("\n> Categoria com maior valor em estoque: %s (R$ %.2f)\n",
           nome_categoria(cat_maior_valor), (double)maior_valor_cat);
    printf("> Faixa de preco mais populosa: %s (%d produto(s))\n",
           nome_faixa(faixa_mais_pop), total_col_qtd[faixa_mais_pop]);
}
