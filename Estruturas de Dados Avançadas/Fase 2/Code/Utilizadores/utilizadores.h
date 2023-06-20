/**
 * @file utilizadores.h
 * @author Allan Aleluia & Francisco Reb�lo (a21990@alunos.ipca.pt, a16443@alunos.ipca.pt)
 * @brief Ficheiro header utilizadores
 * @version 1
 * @date 2023/05/23
 * @copyright Copyright (c) 2023
 */

#include <stdio.h>

#ifndef Y
#define Y

#pragma region Structs

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
} Utilizador;

/**
 * @brief Guarda o número de utilizador seguinte
 */
typedef struct _utilizadorNode
{
    Utilizador *utilizador;
    struct _utilizadorNode *seguinte;
} UtilizadorNode;

/**
 * @brief Lista de utilizadores
 */
typedef struct _listaUtilizadores
{
    struct _utilizadorNode *head;
} ListaUtilizadores;

#pragma endregion

#pragma region Funcoes

void AddUtilizador(Utilizador *utilizador, ListaUtilizadores *list);

ListaUtilizadores *CriarListaUtilizadores();

UtilizadorNode *CreateNode(Utilizador *data);

void PrintUtilizadores(ListaUtilizadores *list);

void LerUtilizadores(char *filename, ListaUtilizadores *listaUtilizadores);

void ApagarUtilizador(int idUtilizador, ListaUtilizadores *list);

void AlterarUtilizador(ListaUtilizadores *list, int idUtilizador, int permissao, char nome[20], int nif, double saldo);

void GuardaListaUtilizadores(ListaUtilizadores *listaUtilizadores);

void AtualizaDadosUtilizadores(char *fileName, ListaUtilizadores *lista);

#pragma endregion

#endif
