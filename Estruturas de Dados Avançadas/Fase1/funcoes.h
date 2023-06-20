#/**
*@file funcoes.h
*@author Francisco Reb�lo (a16443@alunos.ipca.pt)
*@brief Ficheiro header
*@version 1
*@date 2023/03/09
*
*@copyright Copyright (c) 2023
*/

#include <stdio.h>
#ifndef X
#define X

//------------------------------------------------------- STRUCTS -------------------------------------------------------
#pragma region Structs Utilizadores

/**
* @brief Struct de utilizador
*/
typedef struct _utilizador
{
    int idUtilizador;
    int permissao;
    int nif;
    char nome[20];
    double saldo;
} utilizador;

/**
* @brief Guarda o n� de utilizador seguinte
*/
typedef struct _utilizadorNode
{
    utilizador *utilizador;
    struct _utilizadorNode* seguinte;
} utilizadorNode;

/**
* @brief Lista de utilizadores
*/
typedef struct _listaUtilizadores
{
  struct _utilizadorNode *head;
} listaUtilizadores;

#pragma endregion

#pragma region Structs Veiculos

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
} veiculo;

/**
* @brief Guarda o n� do veiculo seguinte
*/
typedef struct _veiculoNode
{
    veiculo *veiculo;
    struct _veiculoNode* seguinte;
} veiculoNode;

/**
* @brief Lista de veiculos
*/
typedef struct _listaVeiculos
{
    struct _veiculoNode *head;
} listaVeiculos;

#pragma endregion

//------------------------------------------------------- FUN��ES -------------------------------------------------------
#pragma region Fun��es Utilizadores
/**
* @brief Fun��o para ler utilizadores de um ficheiro txt
* @param filename Caminho do ficheiro
*/
listaUtilizadores* LerUtilizadores(char* filename);

/**
* @brief Fun��o para adicionar um utilizador a uma lista de utilizadores
* @param list
* @param utilizador
*/
void addUtilizador(utilizador *utilizador, listaUtilizadores *list);

/**
* @brief Fun��o para criar uma lista de utilizadores
*/
listaUtilizadores * criarListaUtilizadores();

/**
* @brief Fun��o para criar um n� que aponta para o pr�ximo utilizador
* @param data
*/
utilizadorNode* createNode(utilizador *data);

/**
* @brief Fun��o para imprimir a lista de utilizadores
* @param list
*/
void printUtilizadores(listaUtilizadores *list);

/**
 * @brief L� os utilizadores do ficheiro txt
 * @param filename
 * @param listaUtilizadores
 */
void lerUtilizadores(char* filename, listaUtilizadores *listaUtilizadores);

/**
 * @brief Fun��o que apaga um utilizador de uma lista
 * @param idUtilizador
 * @param list
 */
void apagarUtilizador(int idUtilizador, listaUtilizadores *list);

/**
 * @brief Fun��o que altera dados de um utilizador numa lista
 * @param list
 * @param idUtilizador
 * @param permissao
 * @param nome
 * @param nif
 * @param saldo
 */
void alterarUtilizador(listaUtilizadores *list, int idUtilizador, int permissao, char nome[20], int nif, double saldo);

/**
 * @brief Fun��o para guardar a lista de utilizadores num ficheiro bin�rio
 * @param listaUtilizadores
 */
void guardaListaUtilizadores(listaUtilizadores *lista);

/**
 * @brief Fun��o para atualizar os utilizadores no ficheiro txt
 * @param fileName
 * @param listaUtilizadores
 */
void atualizaDadosUtilizadores(char* fileName, listaUtilizadores *lista);

#pragma endregion

#pragma region Fun��es Veiculos

/**
* @brief Fun��o para ler veiculos de um ficheiro txt
* @param filename Caminho do ficheiro
*/
listaUtilizadores* LerVeiculos(char* filename);

/**
* @brief Fun��o para adicionar um veiculo a uma lista de veiculos
* @param list
* @param veiculo
*/
void addVeiculo(veiculo *veiculo, listaVeiculos *list);

/**
* @brief Fun��o para criar uma lista de veiculos
*/
listaVeiculos * criarListaVeiculos();

/**
* @brief Fun��o para criar um n� que aponta para o pr�ximo veiculo
* @param data
*/
veiculoNode* createNodeVeiculo(veiculo *data);

/**
* @brief Fun��o para imprimir a lista de veiculos
* @param list
*/
void printVeiculos(listaVeiculos *list);

/**
 * @brief L� os veiculos do ficheiro txt
 * @param filename
 * @param listaVeiculos
 */
void lerVeiculos(char* filename, listaVeiculos *listaVeiculos);

/**
 * @brief Fun��o que apaga um veiculo de uma lista
 * @param idVeiculo
 * @param list
 */
void apagarVeiculo(int idVeiculo, listaVeiculos *list);

/**
 * @brief Fun��o que altera dados de um veiculo numa lista
 * @param list
 * @param idVeiculo
 * @param tipo
 * @param estado
 * @param autonomia
 * @param geoCodigo
 * @param preco
 */
void alterarVeiculo(listaVeiculos *list, int idVeiculo, int tipo, int estado, int autonomia, char geoCodigo[20], double preco);

/**
 * @brief Fun��o para guardar a lista de utilizadores num ficheiro bin�rio
 * @param listaVeiculos
 */
void guardaListaVeiculos(listaVeiculos *lista);

/**
 * @brief Fun��o para atualizar os veiculos no ficheiro txt
 * @param fileName
 * @param listaVeiculos
 */
void atualizaDadosVeiuclos(char* fileName, listaVeiculos *lista);

/**
 * @brief Conta o n�mero de ve�culos numa determinada localiza��o
 * @param list
 * @param geoCodigo
 * @return
 */
int qttVeiculosLocalizacao(listaVeiculos *list, char geoCodigo[20]);

/**
 * @brief Lista a autonomia dos veiculos do menorr para o maior
 * @param list
 */
void listarAutonomiasDecrescente (listaVeiculos *list);

#pragma endregion

#endif