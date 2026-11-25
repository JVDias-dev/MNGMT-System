#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "produto.h"

static void teste_adicionar_buscar(void)
{
    Estoque e;
    estoque_iniciar(&e);

    int c1 = estoque_adicionar(&e, "Playstation 5", 0, 3999.90f, 5);
    int c2 = estoque_adicionar(&e, "Zelda Breath of the Wild", 1, 299.0f, 10);
    assert(c1 == 1 && c2 == 2);
    assert(e.total == 2);

    assert(estoque_buscar_codigo(&e, 2) == 1);
    assert(estoque_buscar_codigo(&e, 99) == -1);

    int idx[10];
    assert(estoque_buscar_nome(&e, "zELDA", idx, 10) == 1 && idx[0] == 1);
    assert(estoque_buscar_nome(&e, "station", idx, 10) == 1 && idx[0] == 0);
    assert(estoque_buscar_nome(&e, "xyz", idx, 10) == 0);

    estoque_liberar(&e);
}

static void teste_validacao(void)
{
    Estoque e;
    estoque_iniciar(&e);
    assert(estoque_adicionar(&e, "", 0, 10.0f, 1) == -1);
    assert(estoque_adicionar(&e, "X", 4, 10.0f, 1) == -1);
    assert(estoque_adicionar(&e, "X", -1, 10.0f, 1) == -1);
    assert(estoque_adicionar(&e, "X", 0, 0.0f, 1) == -1);
    assert(estoque_adicionar(&e, "X", 0, -5.0f, 1) == -1);
    assert(estoque_adicionar(&e, "X", 0, 10.0f, -1) == -1);
    assert(e.total == 0);
    estoque_liberar(&e);
}

static void teste_crescimento(void)
{
    Estoque e;
    estoque_iniciar(&e);
    for (int i = 0; i < 100; i++)
        assert(estoque_adicionar(&e, "Item", i % 4, 10.0f + i, i) == i + 1);
    assert(e.total == 100 && e.capacidade >= 100);
    estoque_liberar(&e);
}

static void teste_editar_remover(void)
{
    Estoque e;
    estoque_iniciar(&e);
    estoque_adicionar(&e, "A", 0, 10.0f, 1);
    estoque_adicionar(&e, "B", 1, 20.0f, 2);
    estoque_adicionar(&e, "C", 2, 30.0f, 3);

    assert(estoque_editar(&e, 2, "B2", 3, 25.0f, 7) == 0);
    Produto *p = &e.itens[estoque_buscar_codigo(&e, 2)];
    assert(strcmp(p->nome, "B2") == 0 && p->categoria == 3 && p->quantidade == 7);
    assert(estoque_editar(&e, 99, "Z", 0, 1.0f, 1) == -1);
    assert(estoque_editar(&e, 2, "Z", 9, 1.0f, 1) == -1);

    assert(estoque_remover(&e, 1) == 0);
    assert(e.total == 2 && e.itens[0].codigo == 2 && e.itens[1].codigo == 3);
    assert(estoque_remover(&e, 1) == -1);

    /* codigo removido nao e reutilizado */
    assert(estoque_adicionar(&e, "D", 0, 5.0f, 1) == 4);
    estoque_liberar(&e);
}

static void teste_persistencia(void)
{
    remove(ARQUIVO_DADOS);

    Estoque e;
    estoque_iniciar(&e);
    assert(persistencia_carregar(&e) == 0 && e.total == 0);   /* sem arquivo */

    estoque_adicionar(&e, "Controle", 2, 349.9f, 4);
    estoque_adicionar(&e, "Funko", 3, 89.0f, 2);
    estoque_remover(&e, 1);
    assert(persistencia_salvar(&e) == 0);
    estoque_liberar(&e);

    Estoque f;
    estoque_iniciar(&f);
    assert(persistencia_carregar(&f) == 0);
    assert(f.total == 1 && f.itens[0].codigo == 2);
    assert(strcmp(f.itens[0].nome, "Funko") == 0);
    assert(f.proximo_codigo == 3);
    estoque_liberar(&f);

    /* arquivo corrompido */
    FILE *fp = fopen(ARQUIVO_DADOS, "wb");
    assert(fp != NULL);
    fputs("lixo", fp);
    fclose(fp);
    Estoque g;
    estoque_iniciar(&g);
    assert(persistencia_carregar(&g) == -1);
    estoque_liberar(&g);

    remove(ARQUIVO_DADOS);
}

int main(void)
{
    teste_adicionar_buscar();
    teste_validacao();
    teste_crescimento();
    teste_editar_remover();
    teste_persistencia();
    printf("Todos os testes passaram.\n");
    return 0;
}
