/**
 * @file main.c
 * @author Allan Aleluia & Francisco Reb�lo (a21990@alunos.ipca.pt, a16443@alunos.ipca.pt)
 * @brief
 * @version 1
 * @date 2023/03/09
 * @copyright Copyright (c) 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utilizadores/utilizadores.h"
#include "Veiculos/veiculos.h"

int main() {

    //----------- Utilizadores -----------
    ListaUtilizadores *lista = CriarListaUtilizadores();
    LerUtilizadores("../utilizadores.txt", lista);
    PrintUtilizadores(lista);
    AlterarUtilizador(lista, 1, 3, "Angelina", 321654987, 500.0);
    ApagarUtilizador(3, lista);
    PrintUtilizadores(lista);
    GuardaListaUtilizadores(lista);
    AtualizaDadosUtilizadores("../utilizadores.txt", lista);

    //----------- Veiculos -----------
    Grafo *grafo = criarGrafo();
    ListaVeiculos *listaVeiculos = CriarListaVeiculos();
    //Antes de adicionar a aresta, deve ser calculada a distância antre dois geoCódigos de modo a determinar o peso entre dois locais
    adicionarAresta(grafo, 20, 1, 2); //Origem é o número do vértice de origem, destino o número do vértice de destino
    LerVeiculos("../veiculos.txt", listaVeiculos, grafo);
    PrintVeiculos(listaVeiculos);
    AlterarVeiculo(listaVeiculos, 1, 2, 1, 12, "pastelaria.refina.eolico", 1);
    ApagarVeiculo(2, listaVeiculos);
    GuardaListaVeiculos(listaVeiculos);
    AtualizaDadosVeiculos("../veiculos.txt", listaVeiculos);
    printf("Total de veiculos: %d\n", QttVeiculosLocalizacao(listaVeiculos, "pastelaria.refina.eolico"));
    ListarAutonomiasDecrescente(listaVeiculos);

    //----------- Grafo -----------
    imprimirGrafo(grafo);
    listarVeiculosPorTipoERaio(grafo, "pastelaria.refina.eolico", 1, 10, 30 );

    // Libertar memória
    free(grafo);
    free(listaVeiculos);
    free(lista);

    return 0;
}
