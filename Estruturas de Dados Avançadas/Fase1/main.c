/**
*@file main.c
*@author Francisco Rebêlo (a16443@alunos.ipca.pt)
*@brief
*@version 1
*@date 2023/03/09
*
*@copyright Copyright (c) 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "utilizadores.c"
#include "veiculos.c"


int main() {

    listaUtilizadores *lista = criarListaUtilizadores();
    lerUtilizadores("C:\\Users\\morei\\Desktop\\EDA\\a16443_Fase1\\utilizadores.txt", lista);
    printUtilizadores(lista);
    alterarUtilizador(lista, 1, 3, "Angelina", 321654987, 500);
    apagarUtilizador(3, lista);
    printUtilizadores(lista);
    guardaListaUtilizadores(lista);
    atualizaDadosUtilizadores("C:\\Users\\morei\\Desktop\\EDA\\a16443_Fase1\\utilizadores.txt", lista);

    listaVeiculos *listaVeiculos = criarListaVeiculos();
    lerVeiculos("C:\\Users\\morei\\Desktop\\EDA\\a16443_Fase1\\veiculos.txt", listaVeiculos);
    printVeiculos(listaVeiculos);
    alterarVeiculo(listaVeiculos, 1, 2, 1, 12,"pastelaria.refina.eolico", 1 );
    apagarVeiculo(2, listaVeiculos);
    guardaListaVeiculos(listaVeiculos);
    atualizaDadosVeiculos("C:\\Users\\morei\\Desktop\\EDA\\a16443_Fase1\\veiculos.txt", listaVeiculos);
    printf("Total de veiculos: %d\n", qttVeiculosLocalizacao(listaVeiculos, "pastelaria.refina.eolico"));
    listarAutonomiasDecrescente(listaVeiculos);
    return 0;
}
