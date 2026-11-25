#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "produto.h"

#define CAPACIDADE_INICIAL 8

void estoque_iniciar(Estoque *e)
{
    e->total = 0;
    e->proximo_codigo = 1;
    e->capacidade = CAPACIDADE_INICIAL;
    e->itens = malloc(e->capacidade * sizeof(Produto));
    if (e->itens == NULL)
        e->capacidade = 0;
}

void estoque_liberar(Estoque *e)
{
    free(e->itens);
    e->itens = NULL;
    e->total = 0;
    e->capacidade = 0;
}

/* Valida os campos de um produto (usado em adicionar e editar). */
static int dados_validos(const char *nome, int categoria, float preco, int quantidade)
{
    if (nome == NULL || nome[0] == '\0')
        return 0;
    if (categoria < 0 || categoria >= NUM_CATEGORIAS)
        return 0;
    if (preco <= 0.0f)
        return 0;
    if (quantidade < 0)
        return 0;
    return 1;
}

/* Copia o nome garantindo o '\0' final (trunca se passar de MAX_NOME-1). */
static void copiar_nome(char *dest, const char *origem)
{
    strncpy(dest, origem, MAX_NOME - 1);
    dest[MAX_NOME - 1] = '\0';
}

int estoque_adicionar(Estoque *e, const char *nome, int categoria,
                      float preco, int quantidade)
{
    if (!dados_validos(nome, categoria, preco, quantidade))
        return -1;

    if (e->total == e->capacidade) {
        int nova = e->capacidade > 0 ? e->capacidade * 2 : CAPACIDADE_INICIAL;
        Produto *p = realloc(e->itens, nova * sizeof(Produto));
        if (p == NULL)
            return -1;
        e->itens = p;
        e->capacidade = nova;
    }

    Produto *novo = &e->itens[e->total];
    novo->codigo = e->proximo_codigo;
    copiar_nome(novo->nome, nome);
    novo->categoria = categoria;
    novo->preco = preco;
    novo->quantidade = quantidade;

    e->total++;
    return e->proximo_codigo++;
}

int estoque_buscar_codigo(const Estoque *e, int codigo)
{
    for (int i = 0; i < e->total; i++) {
        if (e->itens[i].codigo == codigo)
            return i;
    }
    return -1;
}

/* Compara trecho dentro de texto, ignorando maiuscula/minuscula. */
static int contem_ignorando_caixa(const char *texto, const char *trecho)
{
    size_t n = strlen(trecho);
    if (n == 0)
        return 1;
    for (size_t i = 0; texto[i] != '\0'; i++) {
        size_t j = 0;
        while (j < n && texto[i + j] != '\0' &&
               tolower((unsigned char)texto[i + j]) == tolower((unsigned char)trecho[j]))
            j++;
        if (j == n)
            return 1;
    }
    return 0;
}

int estoque_buscar_nome(const Estoque *e, const char *trecho,
                        int *indices, int max)
{
    int achados = 0;
    if (trecho == NULL || indices == NULL)
        return 0;
    for (int i = 0; i < e->total && achados < max; i++) {
        if (contem_ignorando_caixa(e->itens[i].nome, trecho))
            indices[achados++] = i;
    }
    return achados;
}

int estoque_editar(Estoque *e, int codigo, const char *nome, int categoria,
                   float preco, int quantidade)
{
    int i = estoque_buscar_codigo(e, codigo);
    if (i < 0 || !dados_validos(nome, categoria, preco, quantidade))
        return -1;

    copiar_nome(e->itens[i].nome, nome);
    e->itens[i].categoria = categoria;
    e->itens[i].preco = preco;
    e->itens[i].quantidade = quantidade;
    return 0;
}

int estoque_remover(Estoque *e, int codigo)
{
    int i = estoque_buscar_codigo(e, codigo);
    if (i < 0)
        return -1;

    /* desloca os itens seguintes uma posicao para tras */
    memmove(&e->itens[i], &e->itens[i + 1],
            (size_t)(e->total - i - 1) * sizeof(Produto));
    e->total--;
    return 0;
}
