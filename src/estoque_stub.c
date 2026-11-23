/*
 * estoque_stub.c — Implementacao TEMPORARIA das funcoes do Dev A.
 *
 * CRUD completo em memoria (sem arquivo) + dados de teste pre-carregados.
 * Permite ao Dev B testar menus, listagem, busca, edicao, remocao e relatorio
 * antes de integrar com o codigo real do Dev A.
 *
 * Na integracao: apagar este arquivo e compilar com estoque.c + persistencia.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "produto.h"

#define CAPACIDADE_INICIAL 16

/* ================================================================
 * Inicializacao / liberacao
 * ================================================================ */

void estoque_iniciar(Estoque *e)
{
    e->capacidade     = CAPACIDADE_INICIAL;
    e->itens          = malloc((size_t)e->capacidade * sizeof(Produto));
    e->total          = 0;
    e->proximo_codigo = 1;
}

void estoque_liberar(Estoque *e)
{
    free(e->itens);
    e->itens      = NULL;
    e->total      = 0;
    e->capacidade = 0;
}

/* ================================================================
 * CRUD em memoria
 * ================================================================ */

static int garantir_capacidade(Estoque *e)
{
    if (e->total >= e->capacidade) {
        int nova_cap = e->capacidade * 2;
        Produto *novo = realloc(e->itens, (size_t)nova_cap * sizeof(Produto));
        if (!novo) return -1;
        e->itens      = novo;
        e->capacidade = nova_cap;
    }
    return 0;
}

int estoque_adicionar(Estoque *e, const char *nome, int categoria,
                      float preco, int quantidade)
{
    Produto *p;

    if (!nome || categoria < 0 || categoria >= NUM_CATEGORIAS ||
        preco <= 0 || quantidade < 0)
        return -1;

    if (garantir_capacidade(e) != 0)
        return -1;

    p = &e->itens[e->total];
    p->codigo = e->proximo_codigo++;
    strncpy(p->nome, nome, MAX_NOME - 1);
    p->nome[MAX_NOME - 1] = '\0';
    p->categoria  = categoria;
    p->preco      = preco;
    p->quantidade = quantidade;
    e->total++;

    return p->codigo;
}

int estoque_buscar_codigo(const Estoque *e, int codigo)
{
    int i;
    for (i = 0; i < e->total; i++) {
        if (e->itens[i].codigo == codigo)
            return i;
    }
    return -1;
}

/* Busca case-insensitive por trecho do nome */
static int contem_trecho(const char *texto, const char *trecho)
{
    size_t len_t = strlen(texto);
    size_t len_q = strlen(trecho);
    size_t i, j;

    if (len_q > len_t) return 0;

    for (i = 0; i <= len_t - len_q; i++) {
        int achou = 1;
        for (j = 0; j < len_q; j++) {
            if (tolower((unsigned char)texto[i + j]) !=
                tolower((unsigned char)trecho[j])) {
                achou = 0;
                break;
            }
        }
        if (achou) return 1;
    }
    return 0;
}

int estoque_buscar_nome(const Estoque *e, const char *trecho,
                        int *indices, int max)
{
    int encontrados = 0;
    int i;
    for (i = 0; i < e->total && encontrados < max; i++) {
        if (contem_trecho(e->itens[i].nome, trecho))
            indices[encontrados++] = i;
    }
    return encontrados;
}

int estoque_editar(Estoque *e, int codigo, const char *nome, int categoria,
                   float preco, int quantidade)
{
    int idx;

    if (categoria < 0 || categoria >= NUM_CATEGORIAS ||
        preco <= 0 || quantidade < 0)
        return -1;

    idx = estoque_buscar_codigo(e, codigo);
    if (idx < 0) return -1;

    strncpy(e->itens[idx].nome, nome, MAX_NOME - 1);
    e->itens[idx].nome[MAX_NOME - 1] = '\0';
    e->itens[idx].categoria  = categoria;
    e->itens[idx].preco      = preco;
    e->itens[idx].quantidade = quantidade;

    return 0;
}

int estoque_remover(Estoque *e, int codigo)
{
    int idx = estoque_buscar_codigo(e, codigo);
    int i;

    if (idx < 0) return -1;

    /* Deslocar elementos para preencher a posicao removida */
    for (i = idx; i < e->total - 1; i++)
        e->itens[i] = e->itens[i + 1];
    e->total--;

    return 0;
}

/* ================================================================
 * Persistencia stub
 * ================================================================ */

int persistencia_carregar(Estoque *e)
{
    /*
     * Dados de teste que cobrem todas as categorias e faixas de preco.
     * Permite validar listagem, busca, edicao, remocao e relatorio.
     *
     *  Produto                  | Cat (idx) | Preco    | Qtd | Faixa
     *  -------------------------+-----------+----------+-----+------
     *  PlayStation 5            | Console 0 | 4499.90  |   3 |  3
     *  Nintendo Switch Lite     | Console 0 |  199.90  |   5 |  2
     *  The Last of Us Part II   | Jogo    1 |  149.90  |  12 |  1
     *  God of War Ragnarok      | Jogo    1 |  299.90  |   8 |  2
     *  FIFA 25                  | Jogo    1 |   39.90  |  20 |  0
     *  Controle DualSense       | Acess.  2 |  449.90  |   7 |  3
     *  Cabo HDMI 2.1            | Acess.  2 |   45.90  |  15 |  0
     *  Boneco Mario             | Colec.  3 |  189.90  |   4 |  2
     */
    estoque_adicionar(e, "PlayStation 5",         0, 4499.90f,  3);
    estoque_adicionar(e, "Nintendo Switch Lite",  0,  199.90f,  5);
    estoque_adicionar(e, "The Last of Us Part II",1,  149.90f, 12);
    estoque_adicionar(e, "God of War Ragnarok",   1,  299.90f,  8);
    estoque_adicionar(e, "FIFA 25",               1,   39.90f, 20);
    estoque_adicionar(e, "Controle DualSense",    2,  449.90f,  7);
    estoque_adicionar(e, "Cabo HDMI 2.1",         2,   45.90f, 15);
    estoque_adicionar(e, "Boneco Mario",          3,  189.90f,  4);

    return 0;  /* stub nunca falha */
}

int persistencia_salvar(const Estoque *e)
{
    (void)e;   /* Stub: nao salva nada em disco */
    return 0;
}
