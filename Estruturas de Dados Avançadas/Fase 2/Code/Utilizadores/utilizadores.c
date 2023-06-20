/**
 * @file utilizadores.c
 * @author Allan Aleluia & Francisco Reb�lo (a21990@alunos.ipca.pt, a16443@alunos.ipca.pt)
 * @brief Local onde est�o definidas todas as fun��es relativas aos utilizadores
 * @version 1
 * @date 2023/03/09
 * @copyright Copyright (c) 2023
 */

#include <stdlib.h>
#include <string.h>
#include "../Utilizadores/utilizadores.h"

#define _CRT_SECURE_NO_WARNINGS

/**
 * @brief Lê os utilizadores do ficheiro txt
 * @param filename Caminho do ficheiro
 * @param listaUtilizadores Ponteiro para a lista de utilizadores
 */
int LerUtilizadores(char* fileName, ListaUtilizadores *listaUtilizadores) {
    FILE* file;
    int idUtilizador, permissao, nif;
    double saldo;
    char nome[20];


    file = fopen(fileName, "r");
    if (file == NULL) {
        return 0;

    }

    // L� as informa��es de cada usu�rio no arquivo e cria um novo n� na lista para cada um.
    while (fscanf(file, "%d, %d, %19[^,], %d, %lf", &idUtilizador, &permissao, &nome, &nif, &saldo) == 5) {

        // Cria um novo usu�rio e insere na lista.
        Utilizador *utilizador = malloc(sizeof(Utilizador));

        utilizador->idUtilizador = idUtilizador;
        utilizador->permissao = permissao;
        strcpy(utilizador->nome, &nome);
        utilizador->nif = nif;
        // utilizador->saldo = saldo;

        AddUtilizador(utilizador, listaUtilizadores);
    }
    fclose(file);
}

/**
 * @brief Função para adicionar um utilizador a uma lista de utilizadores
 * @param utilizador Utilizador a ser adicionado
 * @param list Lista de utilizadores
 */
void AddUtilizador(Utilizador *utilizador, ListaUtilizadores *list){
    UtilizadorNode *current = NULL;

    if(list->head == NULL){

        list->head = CreateNode(utilizador);
    }
    else {
        current = list->head;
        while (current->seguinte!=NULL){
            current = current->seguinte;
        }
        current->seguinte= CreateNode(utilizador);
    }
}

/**
 * @brief Função para criar uma lista de utilizadores
 * @return Ponteiro para a lista de utilizadores criada
 */
ListaUtilizadores * CriarListaUtilizadores(){
    ListaUtilizadores * list = malloc(sizeof(ListaUtilizadores));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

/**
 * @brief Função para criar um nó que aponta para o próximo utilizador
 * @param data Utilizador a ser armazenado no nó
 * @return Ponteiro para o nó criado
 */
UtilizadorNode* CreateNode(Utilizador *data){

    UtilizadorNode *newNode = malloc(sizeof(UtilizadorNode));

    if (!newNode) {
        return NULL;
    }
    newNode->utilizador = data;
    newNode->seguinte = NULL;
    return newNode;
}

/**
 * @brief Função que apaga um utilizador de uma lista
 * @param idUtilizador ID do utilizador a ser apagado
 * @param list Lista de utilizadores
 */
void ApagarUtilizador(int idUtilizador, ListaUtilizadores *list){

    UtilizadorNode *current = list->head;
    UtilizadorNode *previous = current;

    while(current != NULL){

        if(current->utilizador->idUtilizador == idUtilizador){

            previous->seguinte = current->seguinte;

            if(current == list->head)
                list->head = current->seguinte;
            free(current);
            return;
        }
        previous = current;
        current = current->seguinte;
    }
}

/**
 * @brief Função que altera dados de um utilizador numa lista
 * @param list Lista de utilizadores
 * @param idUtilizador ID do utilizador a ser alterado
 * @param permissao Nova permissao
 * @param nome Novo nome
 * @param nif Novo NIF
 * @param saldo Novo saldo
 */
void AlterarUtilizador(ListaUtilizadores *list, int idUtilizador, int permissao, char nome[20], int nif, double saldo) {

    UtilizadorNode *current = list->head;

    if(list->head == NULL)
        return;

    for(; current != NULL; current = current->seguinte) {

        if(current->utilizador->idUtilizador == idUtilizador) {

            current->utilizador->idUtilizador = idUtilizador;
            current->utilizador->permissao = permissao;
            strcpy(current->utilizador->nome, &nome);
            current->utilizador->nif = nif;
            current->utilizador->saldo = saldo;
        }
    }
}

/**
 * @brief Função para imprimir a lista de utilizadores
 * @param list Lista de utilizadores a ser impressa
 */
void PrintUtilizadores(ListaUtilizadores * list) {

    UtilizadorNode * current = list->head;

    if(list->head == NULL) {
        printf("Erro");
        return;
    }

    for(; current != NULL; current = current->seguinte) {
        printf("User ID: %d\n", current->utilizador->idUtilizador);
        printf("Permissao: %d\n", current->utilizador->permissao);
        printf("Nome: %s\n", current->utilizador->nome);
        printf("Nif: %d\n", current->utilizador->nif);
        printf("Saldo: %0.2f\n\n\n", current->utilizador->saldo);
    }
}

/**
 * @brief Função para guardar a lista de utilizadores num ficheiro binário
 * @param listaUtilizadores Lista de utilizadores a ser guardada
 */
void GuardaListaUtilizadores(ListaUtilizadores *lista) {

    FILE* fp;

    UtilizadorNode * current = lista->head;

    if (lista->head == NULL)
        return;
    if ((fp = fopen("../saveUtilizadores.bin", "wb")) == NULL)
    {
        return;
    }

    for(; current != NULL; current = current->seguinte) {

        fwrite(current, sizeof(current), 1, fp);

    }

    fclose(fp);
}

/**
 * @brief Função para atualizar os utilizadores no ficheiro txt
 * @param fileName Nome do ficheiro
 * @param listaUtilizadores Lista de utilizadores
 */
void AtualizaDadosUtilizadores(char* fileName, ListaUtilizadores *lista) {

    FILE* fp;

    UtilizadorNode * current = lista->head;

    if (lista->head == NULL) {
        return;
    }

    if ((fp = fopen(fileName, "w")) == NULL) {  // Abrir arquivo em modo escrita ("w")
        return;
    }

    for(; current != NULL; current = current->seguinte) {

        fwrite(&(current->utilizador->idUtilizador), sizeof(int), 1, fp);
        fwrite(&(current->utilizador->permissao), sizeof(int), 1, fp);
        fwrite(&(current->utilizador->nome), sizeof(char), sizeof(current->utilizador->nome), fp);
        fwrite(&(current->utilizador->nif), sizeof(int), 1, fp);
        fwrite(&(current->utilizador->saldo), sizeof(double), 1, fp);
    }
    fclose(fp);
}
