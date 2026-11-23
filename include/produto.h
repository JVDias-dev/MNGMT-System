#ifndef PRODUTO_H
#define PRODUTO_H

#define MAX_NOME 50
#define NUM_CATEGORIAS 4   /* 0 Console, 1 Jogo, 2 Acessorio, 3 Colecionavel */
#define NUM_FAIXAS 4       /* 0 ate 50, 1 ate 150, 2 ate 400, 3 acima */
#define ARQUIVO_DADOS "data/estoque.dat"

typedef struct {
    int   codigo;
    char  nome[MAX_NOME];
    int   categoria;
    float preco;
    int   quantidade;
} Produto;

typedef struct {
    Produto *itens;
    int total;
    int capacidade;
    int proximo_codigo;
} Estoque;

/* --- Dev A: estoque.c --- */
void     estoque_iniciar(Estoque *e);
void     estoque_liberar(Estoque *e);
int      estoque_adicionar(Estoque *e, const char *nome, int categoria,
                           float preco, int quantidade);   /* retorna codigo gerado, ou -1 em erro */
int      estoque_buscar_codigo(const Estoque *e, int codigo);  /* retorna indice ou -1 */
int      estoque_buscar_nome(const Estoque *e, const char *trecho,
                             int *indices, int max);       /* preenche indices, retorna quantidade */
int      estoque_editar(Estoque *e, int codigo, const char *nome, int categoria,
                        float preco, int quantidade);      /* 0 ok, -1 nao encontrado/invalido */
int      estoque_remover(Estoque *e, int codigo);          /* 0 ok, -1 nao encontrado */

/* --- Dev A: persistencia.c --- */
int persistencia_carregar(Estoque *e);   /* 0 ok (inclui arquivo inexistente = vazio), -1 arquivo corrompido */
int persistencia_salvar(const Estoque *e);

#endif
