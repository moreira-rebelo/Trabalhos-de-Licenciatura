/**
 * @file grafo.c
 * @author Allan Aleluia & Francisco Reb�lo (a21990@alunos.ipca.pt, a16443@alunos.ipca.pt)
 * @brief Local onde est�o definidas todas as fun��es relativas aos grafos
 * @version 1
 * @date 2023/05/23
 * @copyright Copyright (c) 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

#include "../Veiculos/veiculos.h"

#define MAX_VERTICES 100
#define RAIO_MAXIMO 10 // Define o raio máximo
#define CAPACIDADE_CAMINHAO 200 // Define a capacidade máxima do caminhão

/**
 * @brief Criar grafo
 * @return
 */
Grafo* criarGrafo()
{
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = 0;
    grafo->numArestas = 0;
    return grafo;
}

/**
 * @brief Adicionar vértice ao grafo
 * @param grafo
 * @param geoCodigo
 * @param veiculo
 */
void adicionarVertice(Grafo* grafo, char geoCodigo[20], Veiculo* veiculo)
{
    if (grafo->numVertices >= MAX_VERTICES)
    {
        printf("O grafo atingiu o número máximo de vértices.\n");
        return;
    }

    strcpy(grafo->vertices[grafo->numVertices].geoCodigo, geoCodigo);
    grafo->vertices[grafo->numVertices].veiculo = veiculo;
    grafo->numVertices++;
}

/**
 * @brief Adicionar aresta ao grafo
 * @param grafo
 * @param peso
 * @param origem
 * @param destino
 */
void adicionarAresta(Grafo* grafo, int peso, int origem, int destino) //Lista de adjacência
{
    if (grafo->numArestas >= MAX_VERTICES * (MAX_VERTICES - 1) / 2)
    {
        printf("O grafo atingiu o número máximo de arestas.\n");
        return;
    }

    grafo->arestas[grafo->numArestas].peso = peso;
    grafo->arestas[grafo->numArestas].origem = origem;
    grafo->arestas[grafo->numArestas].destino = destino;
    grafo->numArestas++;
}

/**
 * @brief Imprimir grafo
 * @param grafo
 */
void imprimirGrafo(Grafo* grafo)
{
    printf("Vértices:\n");
    for (int i = 0; i < grafo->numVertices; i++)
    {
        printf("  - %s\n", grafo->vertices[i].geoCodigo);
    }

    printf("Arestas:\n");
    for (int i = 0; i < grafo->numArestas; i++)
    {
        printf("  - Origem: %s, Destino: %s, Peso: %d\n",
               grafo->vertices[grafo->arestas[i].origem].geoCodigo,
               grafo->vertices[grafo->arestas[i].destino].geoCodigo,
               grafo->arestas[i].peso);
    }
}

/**
 * @brief Calcula a distância entre dois vertices
 * @param pesoCentro
 * @param vertice
 * @return
 */
double calcularDistancia(int pesoCentro, Vertice vertice)
{
   double distancia = pesoCentro - vertice.veiculo->peso;
   return distancia;
}

/**
 * @brief Listar veiculos por raio
 * @param grafo
 * @param clienteGeoCodigo
 * @param tipoVeiculo
 * @param pesoCentro
 * @param raioMaximo
 */
void listarVeiculosPorTipoERaio(Grafo* grafo, char* clienteGeoCodigo, int tipoVeiculo, int pesoCentro, double raioMaximo)
{
    printf("Meios de mobilidade eletrica do tipo %d no raio de %.2f unidades de %s:\n", tipoVeiculo, raioMaximo, clienteGeoCodigo);

    for (int i = 0; i < grafo->numVertices; i++)
    {
        Vertice vertice = grafo->vertices[i];

        if (vertice.veiculo->tipo == tipoVeiculo)
        {
            double distancia = calcularDistancia(pesoCentro, vertice);

            if (distancia <= raioMaximo)
            {
                printf("- %s (Distancia: %.2f)\n", vertice.geoCodigo, distancia);
            }
        }
    }
}

/**
 * @brief Encontrar o vértice mais próximo
 * @param grafo
 * @param visitado
 * @param verticeAtual
 * @return
 */
int encontrarVerticeMaisProximo(Grafo* grafo, bool visitado[], int verticeAtual)
{
    int numVertices = grafo->numVertices;
    double distanciaMinima = DBL_MAX;
    int verticeMaisProximo = -1;

    for (int i = 0; i < numVertices; i++) //Percorre os vértices
    {
        if (!visitado[i] && i != verticeAtual) //Se não foi visitado e é diferente do vértice atual
        {
            double distancia = calcularDistancia(CAPACIDADE_CAMINHAO, grafo->vertices[i]); //Calcula a distância

            if (distancia < distanciaMinima)
            {
                distanciaMinima = distancia;
                verticeMaisProximo = i;
            }
        }
    }

    return verticeMaisProximo;
}

/**
 * @brief Problema do caixeiro viajante
 * @param grafo
 * @param verticeInicial
 */
void encontrarCaminhoMenorDistancia(Grafo* grafo, int verticeInicial)
{
    int numVertices = grafo->numVertices;
    bool visitado[numVertices];
    int caminho[numVertices];
    int caminhoAtual = 0;
    double distanciaTotal = 0.0;

    for (int i = 0; i < numVertices; i++) //Percorre os vértices
        visitado[i] = false; //Coloca todos os vértices como não visitados

    visitado[verticeInicial] = true; //Coloca o vértice inicial como visitado
    caminho[caminhoAtual++] = verticeInicial; //Adiciona o vertice atual ao final

    while (caminhoAtual < numVertices)
    {
        int verticeAtual = caminho[caminhoAtual - 1];
        int proximoVertice = encontrarVerticeMaisProximo(grafo, visitado, verticeAtual);

        if (proximoVertice == -1)
        {
            // Caso não seja possível encontrar o próximo vértice, encerra o ciclo
            break;
        }

        visitado[proximoVertice] = true; //Coloca o vértice mais próximo como visitado
        caminho[caminhoAtual++] = proximoVertice; //Coloca o caminho atual no local do vértice mais próximo determinado anteriormente

        // Calcular a distância entre verticeAtual e proximoVertice
        double distancia = calcularDistancia(CAPACIDADE_CAMINHAO, grafo->vertices[proximoVertice]); //Calcula a distância entre o local atual e o próximo vértice
        distanciaTotal += distancia; //Soma a distância à dsitância total
    }

    // Completar o ciclo voltando ao vértice inicial
    caminho[caminhoAtual] = verticeInicial;
    distanciaTotal += calcularDistancia(CAPACIDADE_CAMINHAO, grafo->vertices[verticeInicial]);

    // Imprimir o caminho encontrado e a distância total percorrida
    printf("Caminho com menor distância:\n");

    for (int i = 0; i <= caminhoAtual; i++)
    {
        int vertice = caminho[i];
        printf("%s ", grafo->vertices[vertice].geoCodigo);
    }

    printf("\nDistância total: %.2f\n", distanciaTotal);
}




