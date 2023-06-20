/**
 * @file veiculos.h
 * @author Allan Aleluia & Francisco Reb�lo (a21990@alunos.ipca.pt, a16443@alunos.ipca.pt)
 * @brief Ficheiro header veiculos
 * @version 1
 * @date 2023/05/23
 * @copyright Copyright (c) 2023
 */

#include <stdio.h>
#include <stdbool.h>

#ifndef Z
#define Z

#define MAX_VERTICES 100

#pragma region Structs

/**
 * @brief Struct de veiculo
 */
typedef struct _veiculo
{
    int idVeiculo;
    int tipo;
    int estado;
    int autonomia;
    char geoCodigo[20];
    double preco;
    int peso;
} Veiculo;

/**
 * @brief Guarda o n� do veiculo seguinte
 */
typedef struct _veiculoNode
{
    Veiculo *veiculo;
    struct _veiculoNode* seguinte;
} VeiculoNode;

/**
 * @brief Lista de veiculos
 */
typedef struct _listaVeiculos
{
    struct _veiculoNode *head;
} ListaVeiculos;

/**
 * @brief Vértice do grafo (Array)
 */
typedef struct _vertice
{
    char geoCodigo[20];
    Veiculo* veiculo;
} Vertice;

/**
 * @brief Aresta do grafo (Array)
 */
typedef struct _aresta
{
    int peso;
    int origem;
    int destino;
} Aresta;

/**
 * @brief Struct de grafo (Array)
 */
typedef struct _grafo
{
    Vertice vertices[MAX_VERTICES];
    Aresta arestas[MAX_VERTICES * (MAX_VERTICES - 1) / 2]; // Grafo completo
    int numVertices;
    int numArestas;
} Grafo;

#pragma endregion

#pragma region Funcoes

//---------- VEICULOS -----------
void AddVeiculo(VeiculoNode *veiculoNode, ListaVeiculos *list);

void LerVeiculos(char* fileName, ListaVeiculos *listaVeiculos, Grafo* grafo);

ListaVeiculos * CriarListaVeiculos();

VeiculoNode* CreateNodeVeiculo(Veiculo *data);

void PrintVeiculos(ListaVeiculos *list);


void ApagarVeiculo(int idVeiculo, ListaVeiculos *list);

void AlterarVeiculo(ListaVeiculos *list, int idVeiculo, int tipo, int estado, int autonomia, char geoCodigo[20], double preco);

void GuardaListaVeiculos(ListaVeiculos *lista);

void AtualizaDadosVeiculos(char* fileName, ListaVeiculos *lista);

int QttVeiculosLocalizacao(ListaVeiculos *list, char geoCodigo[20]);

void ListarAutonomiasDecrescente(ListaVeiculos *list);


//---------- GRAFO -----------
Grafo* criarGrafo();

void adicionarVertice(Grafo* grafo, char geoCodigo[20], Veiculo* veiculo);

void adicionarAresta(Grafo* grafo, int peso, int origem, int destino);

void imprimirGrafo(Grafo* grafo);

void listarVeiculosPorTipoERaio(Grafo* grafo, char* clienteGeoCodigo, int tipoVeiculo, int pesoCentro, double raioMaximo);

double calcularDistancia(int pesoCentro, Vertice vertice);

int encontrarVerticeMaisProximo(Grafo* grafo, bool visitado[], int verticeAtual);

void encontrarCaminhoMenorDistancia(Grafo* grafo, int verticeInicial);







#pragma endregion

#endif
