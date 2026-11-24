#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "produto.h"

#ifdef _WIN32
#include <direct.h>
#define CRIAR_PASTA(p) _mkdir(p)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define CRIAR_PASTA(p) mkdir((p), 0755)
#endif

#define ARQUIVO_TEMP ARQUIVO_DADOS ".tmp"
#define PASTA_DADOS  "data"

/* Cabecalho gravado antes do array de produtos. */
typedef struct {
    int total;
    int proximo_codigo;
} Cabecalho;

int persistencia_carregar(Estoque *e)
{
    FILE *f = fopen(ARQUIVO_DADOS, "rb");
    if (f == NULL)
        return (errno == ENOENT) ? 0 : -1;   /* sem arquivo = estoque vazio */

    Cabecalho c;
    if (fread(&c, sizeof(c), 1, f) != 1 || c.total < 0 || c.proximo_codigo < 1) {
        fclose(f);
        return -1;
    }

    int capacidade = c.total > 8 ? c.total : 8;
    Produto *itens = malloc((size_t)capacidade * sizeof(Produto));
    if (itens == NULL) {
        fclose(f);
        return -1;
    }

    if (c.total > 0 && fread(itens, sizeof(Produto), (size_t)c.total, f) != (size_t)c.total) {
        free(itens);
        fclose(f);
        return -1;
    }
    fclose(f);

    /* so troca o estoque depois de ler tudo com sucesso */
    free(e->itens);
    e->itens = itens;
    e->total = c.total;
    e->capacidade = capacidade;
    e->proximo_codigo = c.proximo_codigo;
    return 0;
}

int persistencia_salvar(const Estoque *e)
{
    CRIAR_PASTA(PASTA_DADOS);   /* ignora erro: pode ja existir */

    FILE *f = fopen(ARQUIVO_TEMP, "wb");
    if (f == NULL)
        return -1;

    Cabecalho c = { e->total, e->proximo_codigo };
    int ok = fwrite(&c, sizeof(c), 1, f) == 1;
    if (ok && e->total > 0)
        ok = fwrite(e->itens, sizeof(Produto), (size_t)e->total, f) == (size_t)e->total;
    if (fclose(f) != 0)
        ok = 0;

    if (!ok) {
        remove(ARQUIVO_TEMP);
        return -1;
    }

#ifdef _WIN32
    remove(ARQUIVO_DADOS);   /* no Windows rename falha se o destino existe */
#endif
    if (rename(ARQUIVO_TEMP, ARQUIVO_DADOS) != 0) {
        remove(ARQUIVO_TEMP);
        return -1;
    }
    return 0;
}
