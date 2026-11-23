# GameStock — Sistema de Estoque para Loja de Games

Sistema de linha de comando em C (C99) para gerenciar o estoque de uma loja de games
(consoles, jogos, acessorios, colecionaveis).

Trabalho em dupla: **Dev A** cuida do CRUD em memoria e persistencia em arquivo;
**Dev B** cuida da interface (menu, telas, validacao) e da inteligencia (relatorio estatistico).

## Compilacao

### Com o stub (antes da integracao com Dev A)

```bash
gcc -Wall -Wextra -std=c99 -o gamestock src/main.c src/menu.c src/relatorio.c src/estoque_stub.c -Iinclude
```

### Com o codigo do Dev A (apos integracao)

Remover `src/estoque_stub.c` e compilar com os fontes reais:

```bash
gcc -Wall -Wextra -std=c99 -o gamestock src/main.c src/menu.c src/relatorio.c src/estoque.c src/persistencia.c -Iinclude
```

Ou utilize o Makefile do Dev A.

## Uso

```bash
./gamestock
```

O programa exibe um menu interativo:

```
===== GAMESTOCK =====
1 - Cadastrar produto
2 - Listar produtos
3 - Buscar (nome ou codigo)
4 - Editar produto
5 - Remover produto
6 - Relatorio estatistico
0 - Sair
```

## Funcionalidades

| Opcao | Funcionalidade | Descricao |
|-------|---------------|-----------|
| 1 | Cadastrar | Adiciona produto com nome, categoria, preco e quantidade |
| 2 | Listar | Exibe todos os produtos em tabela alinhada |
| 3 | Buscar | Busca por trecho do nome (case-insensitive) ou por codigo |
| 4 | Editar | Altera dados de um produto existente |
| 5 | Remover | Remove produto com confirmacao (s/n) |
| 6 | Relatorio | Matriz estatistica Categoria x Faixa de preco |
| 0 | Sair | Salva dados em arquivo e encerra |

## Categorias

| Codigo | Nome          |
|--------|---------------|
| 0      | Console       |
| 1      | Jogo          |
| 2      | Acessorio     |
| 3      | Colecionavel  |

## Faixas de Preco

| Faixa | Intervalo             |
|-------|-----------------------|
| 0     | Ate R$ 50,00          |
| 1     | R$ 50,01 a R$ 150,00  |
| 2     | R$ 150,01 a R$ 400,00 |
| 3     | Acima de R$ 400,00    |

## Exemplo do Relatorio (dados de teste do stub)

```
QUANTIDADE DE PRODUTOS
Categoria      | ate 50 | 50-150 | 150-400 |  +400 | Total
--------------+--------+--------+---------+-------+-------
Console        |      0 |      0 |       1 |     1 |     2
Jogo           |      1 |      1 |       1 |     0 |     3
Acessorio      |      1 |      0 |       0 |     1 |     2
Colecionavel   |      0 |      0 |       1 |     0 |     1
Total          |      2 |      1 |       3 |     2 |     8

VALOR TOTAL EM ESTOQUE
Categoria      |      ate 50 |      50-150 |     150-400 |        +400 |         Total
--------------+-------------+-------------+-------------+-------------+---------------
Console        | R$     0.00 | R$     0.00 | R$   999.50 | R$ 13499.70 | R$   14499.20
Jogo           | R$   798.00 | R$  1798.80 | R$  2399.20 | R$     0.00 | R$    4996.00
Acessorio      | R$   688.50 | R$     0.00 | R$     0.00 | R$  3149.30 | R$    3837.80
Colecionavel   | R$     0.00 | R$     0.00 | R$   759.60 | R$     0.00 | R$     759.60
Total          | R$  1486.50 | R$  1798.80 | R$  4158.30 | R$ 16649.00 | R$   24092.60

> Categoria com maior valor em estoque: Console (R$ 14499.20)
> Faixa de preco mais populosa: 150-400 (3 produto(s))
```

## Estrutura do Projeto

```
gamestock/
├── include/
│   ├── produto.h          <- Contrato (congelado, escrito em conjunto)
│   ├── menu.h             <- Dev B
│   └── relatorio.h        <- Dev B
├── src/
│   ├── main.c             <- Dev B  (ponto de entrada)
│   ├── menu.c             <- Dev B  (telas, entrada e validacao)
│   ├── relatorio.c        <- Dev B  (matriz estatistica)
│   ├── estoque_stub.c     <- Dev B  (temporario, dados de teste)
│   ├── estoque.c          <- Dev A  (CRUD em memoria)
│   └── persistencia.c     <- Dev A  (arquivo .dat)
├── data/
│   └── estoque.dat        <- Gerado em execucao (nao versionar)
├── Makefile               <- Dev A
└── README.md              <- Dev B
```

## Notas de Integracao

1. Remover `src/estoque_stub.c` do projeto.
2. Incluir `src/estoque.c` e `src/persistencia.c` do Dev A na compilacao.
3. Criar o diretorio `data/` antes da primeira execucao (se nao existir).
4. Compilar com `gcc -Wall -Wextra -std=c99` e garantir **zero warnings**.
5. Roteiro de teste:
   - Primeira execucao sem arquivo `data/estoque.dat`.
   - Cadastrar 10+ produtos variados.
   - Editar nome, categoria, preco e quantidade.
   - Remover produto (testar confirmacao s/n).
   - Gerar relatorio e conferir totais.
   - Sair (opcao 0) e reabrir — verificar se os dados persistiram.
   - Testar entradas invalidas: letras no lugar de numero, preco negativo, codigo inexistente.
