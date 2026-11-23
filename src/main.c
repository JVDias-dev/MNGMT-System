/*
 * main.c — Ponto de entrada do GameStock.
 *
 * Fluxo:  iniciar estoque -> carregar arquivo -> loop do menu -> salvar -> sair
 */

#include <stdio.h>
#include "produto.h"
#include "menu.h"
#include "relatorio.h"

int main(void)
{
    Estoque estoque;
    int opcao;

    estoque_iniciar(&estoque);

    /* Carregar dados (stub pre-carrega produtos de teste) */
    if (persistencia_carregar(&estoque) == -1) {
        char resp[8];
        printf("Aviso: arquivo de dados corrompido ou ilegivel.\n");
        ler_texto("Deseja comecar com estoque vazio? (s/n): ", resp, 8);
        if (resp[0] != 's' && resp[0] != 'S') {
            estoque_liberar(&estoque);
            printf("Encerrando.\n");
            return 1;
        }
        /* Reiniciar com estoque vazio */
        estoque_liberar(&estoque);
        estoque_iniciar(&estoque);
    }

    /* Loop principal */
    do {
        opcao = menu_exibir();
        switch (opcao) {
            case 1: menu_cadastrar(&estoque);   break;
            case 2: menu_listar(&estoque);      break;
            case 3: menu_buscar(&estoque);      break;
            case 4: menu_editar(&estoque);      break;
            case 5: menu_remover(&estoque);     break;
            case 6: relatorio_matriz(&estoque); break;
            case 0: break;
        }
    } while (opcao != 0);

    /* Salvar e liberar */
    if (persistencia_salvar(&estoque) != 0)
        printf("Aviso: erro ao salvar os dados!\n");

    estoque_liberar(&estoque);
    printf("Ate logo!\n");
    return 0;
}
