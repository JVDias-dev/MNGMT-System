/*
 * menu.c — Telas do menu, leitura e validacao de entrada.
 *
 * Toda interacao com o usuario (printf / fgets) fica aqui.
 * As funcoes de estoque nao imprimem nada; as mensagens sao nossas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

/* ================================================================
 * Nomes para exibicao
 * ================================================================ */

const char *nome_categoria(int cat)
{
    static const char *nomes[] = {
        "Console", "Jogo", "Acessorio", "Colecionavel"
    };
    if (cat >= 0 && cat < NUM_CATEGORIAS)
        return nomes[cat];
    return "Desconhecida";
}

const char *nome_faixa(int faixa)
{
    static const char *nomes[] = {
        "ate 50", "50-150", "150-400", "+400"
    };
    if (faixa >= 0 && faixa < NUM_FAIXAS)
        return nomes[faixa];
    return "Desconhecida";
}

/* ================================================================
 * Funcoes de entrada segura
 * Usam fgets + strtol/strtof, repetindo ate entrada valida.
 * Nunca usam scanf solto.
 * ================================================================ */

int ler_inteiro(const char *prompt, int min, int max)
{
    char buf[64];
    long valor;
    char *fim;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, (int)sizeof(buf), stdin))
            continue;

        /* Se a linha nao coube no buffer, descartar o restante */
        {
            size_t len = strlen(buf);
            if (len > 0 && buf[len - 1] != '\n') {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
            }
        }

        valor = strtol(buf, &fim, 10);

        /* Pular espacos apos o numero */
        while (*fim == ' ' || *fim == '\t')
            fim++;

        if (fim == buf || (*fim != '\n' && *fim != '\0')) {
            printf("  Entrada invalida. Digite um numero inteiro.\n");
            continue;
        }
        if (valor < min || valor > max) {
            printf("  Valor fora do intervalo (%d a %d).\n", min, max);
            continue;
        }
        return (int)valor;
    }
}

float ler_float(const char *prompt, float min, float max)
{
    char buf[64];
    float valor;
    char *fim;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, (int)sizeof(buf), stdin))
            continue;

        {
            size_t len = strlen(buf);
            if (len > 0 && buf[len - 1] != '\n') {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
            }
        }

        valor = strtof(buf, &fim);

        while (*fim == ' ' || *fim == '\t')
            fim++;

        if (fim == buf || (*fim != '\n' && *fim != '\0')) {
            printf("  Entrada invalida. Digite um numero.\n");
            continue;
        }
        if (valor < min || valor > max) {
            printf("  Valor fora do intervalo (%.2f a %.2f).\n",
                   (double)min, (double)max);
            continue;
        }
        return valor;
    }
}

void ler_texto(const char *prompt, char *destino, int tamanho)
{
    while (1) {
        printf("%s", prompt);
        if (!fgets(destino, tamanho, stdin))
            continue;

        size_t len = strlen(destino);

        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
            len--;
        } else {
            /* Linha maior que o buffer: descartar o restante */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;
        }

        if (len == 0) {
            printf("  Texto nao pode ser vazio.\n");
            continue;
        }
        return;
    }
}

/* ================================================================
 * Menu principal
 * ================================================================ */

int menu_exibir(void)
{
    printf("\n===== GAMESTOCK =====\n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Buscar (nome ou codigo)\n");
    printf("4 - Editar produto\n");
    printf("5 - Remover produto\n");
    printf("6 - Relatorio estatistico\n");
    printf("0 - Sair\n");
    return ler_inteiro("Opcao: ", 0, 6);
}

/* ================================================================
 * Helpers de exibicao de tabela
 * ================================================================ */

static void imprimir_cabecalho_tabela(void)
{
    printf("%-6s | %-30s | %-12s | %10s | %5s\n",
           "Cod", "Nome", "Categoria", "Preco", "Qtd");
    printf("-------+--------------------------------+--------------+"
           "------------+------\n");
}

static void imprimir_produto(const Produto *p)
{
    printf("%-6d | %-30s | %-12s | R$ %7.2f | %5d\n",
           p->codigo, p->nome, nome_categoria(p->categoria),
           (double)p->preco, p->quantidade);
}

/* ================================================================
 * Telas do CRUD
 * ================================================================ */

void menu_cadastrar(Estoque *e)
{
    char nome[MAX_NOME];
    int categoria, quantidade, codigo;
    float preco;

    printf("\n--- Cadastrar Produto ---\n");
    ler_texto("Nome: ", nome, MAX_NOME);

    printf("Categorias: ");
    for (int i = 0; i < NUM_CATEGORIAS; i++)
        printf("%d-%s  ", i, nome_categoria(i));
    printf("\n");
    categoria = ler_inteiro("Categoria: ", 0, NUM_CATEGORIAS - 1);

    preco = ler_float("Preco (R$): ", 0.01f, 99999.99f);
    quantidade = ler_inteiro("Quantidade: ", 0, 99999);

    codigo = estoque_adicionar(e, nome, categoria, preco, quantidade);
    if (codigo >= 0)
        printf("Produto cadastrado com sucesso! Codigo: %d\n", codigo);
    else
        printf("Erro ao cadastrar produto.\n");
}

void menu_listar(const Estoque *e)
{
    printf("\n--- Lista de Produtos ---\n");
    if (e->total == 0) {
        printf("Estoque vazio.\n");
        return;
    }
    imprimir_cabecalho_tabela();
    for (int i = 0; i < e->total; i++)
        imprimir_produto(&e->itens[i]);
    printf("\nTotal: %d produto(s)\n", e->total);
}

void menu_buscar(const Estoque *e)
{
    int tipo;

    printf("\n--- Buscar Produto ---\n");
    printf("1 - Por nome\n");
    printf("2 - Por codigo\n");
    tipo = ler_inteiro("Opcao: ", 1, 2);

    if (tipo == 1) {
        char trecho[MAX_NOME];
        int indices[100];
        int encontrados;

        ler_texto("Trecho do nome: ", trecho, MAX_NOME);
        encontrados = estoque_buscar_nome(e, trecho, indices, 100);

        if (encontrados == 0) {
            printf("Nenhum produto encontrado.\n");
        } else {
            printf("%d produto(s) encontrado(s):\n", encontrados);
            imprimir_cabecalho_tabela();
            for (int i = 0; i < encontrados; i++)
                imprimir_produto(&e->itens[indices[i]]);
        }
    } else {
        int codigo = ler_inteiro("Codigo: ", 1, 99999);
        int idx = estoque_buscar_codigo(e, codigo);

        if (idx < 0) {
            printf("Produto com codigo %d nao encontrado.\n", codigo);
        } else {
            imprimir_cabecalho_tabela();
            imprimir_produto(&e->itens[idx]);
        }
    }
}

void menu_editar(Estoque *e)
{
    int codigo, idx, resultado;
    char nome[MAX_NOME];
    int categoria, quantidade;
    float preco;

    printf("\n--- Editar Produto ---\n");
    codigo = ler_inteiro("Codigo do produto: ", 1, 99999);

    idx = estoque_buscar_codigo(e, codigo);
    if (idx < 0) {
        printf("Produto com codigo %d nao encontrado.\n", codigo);
        return;
    }

    printf("Produto atual:\n");
    imprimir_cabecalho_tabela();
    imprimir_produto(&e->itens[idx]);

    /* O usuario re-informa todos os campos */
    printf("\nDigite os novos dados:\n");
    ler_texto("Novo nome: ", nome, MAX_NOME);

    printf("Categorias: ");
    for (int i = 0; i < NUM_CATEGORIAS; i++)
        printf("%d-%s  ", i, nome_categoria(i));
    printf("\n");
    categoria = ler_inteiro("Nova categoria: ", 0, NUM_CATEGORIAS - 1);

    preco = ler_float("Novo preco (R$): ", 0.01f, 99999.99f);
    quantidade = ler_inteiro("Nova quantidade: ", 0, 99999);

    resultado = estoque_editar(e, codigo, nome, categoria, preco, quantidade);
    if (resultado == 0)
        printf("Produto atualizado com sucesso!\n");
    else
        printf("Erro ao editar produto.\n");
}

void menu_remover(Estoque *e)
{
    int codigo, idx;
    char resp[8];

    printf("\n--- Remover Produto ---\n");
    codigo = ler_inteiro("Codigo do produto: ", 1, 99999);

    idx = estoque_buscar_codigo(e, codigo);
    if (idx < 0) {
        printf("Produto com codigo %d nao encontrado.\n", codigo);
        return;
    }

    printf("Produto a remover:\n");
    imprimir_cabecalho_tabela();
    imprimir_produto(&e->itens[idx]);

    ler_texto("Confirma remocao? (s/n): ", resp, 8);
    if (resp[0] == 's' || resp[0] == 'S') {
        if (estoque_remover(e, codigo) == 0)
            printf("Produto removido com sucesso!\n");
        else
            printf("Erro ao remover produto.\n");
    } else {
        printf("Remocao cancelada.\n");
    }
}
