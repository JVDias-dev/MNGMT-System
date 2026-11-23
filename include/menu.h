#ifndef MENU_H
#define MENU_H

#include "produto.h"

/* Funcoes de entrada segura */
int   ler_inteiro(const char *prompt, int min, int max);
float ler_float(const char *prompt, float min, float max);
void  ler_texto(const char *prompt, char *destino, int tamanho);

/* Menu principal — retorna a opcao escolhida */
int  menu_exibir(void);

/* Telas do menu */
void menu_cadastrar(Estoque *e);
void menu_listar(const Estoque *e);
void menu_buscar(const Estoque *e);
void menu_editar(Estoque *e);
void menu_remover(Estoque *e);

/* Nomes para exibicao (definidos em menu.c) */
const char *nome_categoria(int cat);
const char *nome_faixa(int faixa);

#endif
