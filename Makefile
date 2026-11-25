CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

# Fontes do Dev B (so entram quando existirem)
SRC_B  = $(wildcard src/main.c src/menu.c src/relatorio.c)
SRC_A  = src/estoque.c src/persistencia.c

all: gamestock

gamestock: $(SRC_A) $(SRC_B)
	$(CC) $(CFLAGS) -o $@ $^

teste: tests/teste_estoque.c $(SRC_A)
	$(CC) $(CFLAGS) -o teste_estoque $^
	./teste_estoque

clean:
	rm -f gamestock gamestock.exe teste_estoque teste_estoque.exe data/estoque.dat data/estoque.dat.tmp

.PHONY: all teste clean
