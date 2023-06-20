/**
*@file utilizadores.c
*@author Francisco Rebêlo (a16443@alunos.ipca.pt)
*@brief Local onde estão definidas todas as funções relativas aos utilizadores
*@version 1
*@date 2023/03/09
*@copyright Copyright (c) 2023
*/

#include <stdlib.h>
#include "funcoes.h"


void lerUtilizadores(char* fileName, listaUtilizadores *listaUtilizadores) {
    FILE* file;
    int idUtilizador, permissao, nif;
    double saldo;
    char nome[20];


    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening the file %s.\n", fileName);
        exit(1);
    }

    // Lê as informações de cada usuário no arquivo e cria um novo nó na lista para cada um.
    while (fscanf(file, "%d, %d, %19[^,], %d, %lf", &idUtilizador, &permissao, &nome, &nif, &saldo) == 5) {

        // Cria um novo usuário e insere na lista.
        utilizador *utilizador = malloc(sizeof(utilizador));

        utilizador->idUtilizador = idUtilizador;
        utilizador->permissao = permissao;
        strcpy(utilizador->nome, &nome);
        utilizador->nif = nif;
       // utilizador->saldo = saldo;

        addUtilizador(utilizador, listaUtilizadores);
    }
    fclose(file);
}


void addUtilizador(utilizador *utilizador, listaUtilizadores *list){
    utilizadorNode *current = NULL;

    if(list->head == NULL){
        list->head = createNode(utilizador);
    }
    else {
        current = list->head;
        while (current->seguinte!=NULL){
            current = current->seguinte;
        }
        current->seguinte= createNode(utilizador);
    }
}

/**
 *
 * @return
 */
listaUtilizadores * criarListaUtilizadores(){
    listaUtilizadores * list = malloc(sizeof(listaUtilizadores));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

utilizadorNode* createNode(utilizador *data){

    utilizadorNode *newNode = malloc(sizeof(utilizadorNode));

    if (!newNode) {
        return NULL;
    }
    newNode->utilizador = data;
    newNode->seguinte = NULL;
    return newNode;
}

void apagarUtilizador(int idUtilizador, listaUtilizadores *list){

    utilizadorNode *current = list->head;
    utilizadorNode *previous = current;

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

void alterarUtilizador(listaUtilizadores *list, int idUtilizador, int permissao, char nome[20], int nif, double saldo) {

    utilizadorNode *current = list->head;

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
 *
 * @param list
 */
void printUtilizadores(listaUtilizadores * list) {

    utilizadorNode * current = list->head;

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

void guardaListaUtilizadores(listaUtilizadores *lista) {

    FILE* fp;

    utilizadorNode * current = lista->head;

    if (lista->head == NULL)
        return;
    if ((fp = fopen("C:\\Users\\morei\\Desktop\\EDA\\a16443_Fase1\\saveUtilizadores.bin", "wb")) == NULL)
    {
        return;
    }

    for(; current != NULL; current = current->seguinte) {

        fwrite(&current, sizeof(current), 1, fp);

        fwrite(&(current->utilizador->idUtilizador), sizeof(int), 1, fp);
        fwrite(&(current->utilizador->permissao), sizeof(int), 1, fp);
        fwrite(&(current->utilizador->nome), sizeof(char), sizeof(current->utilizador->nome), fp);
        fwrite(&(current->utilizador->nif), sizeof(int), 1, fp);
        fwrite(&(current->utilizador->saldo), sizeof(double), 1, fp);
    }

    fclose(fp);
}

void atualizaDadosUtilizadores(char* fileName, listaUtilizadores *lista) {

    FILE* fp;

    utilizadorNode * current = lista->head;

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

