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

void lerVeiculos(char* fileName, listaVeiculos *listaVeiculos) {
    FILE* file;
    int idVeiculo, tipo, estado, autonomia;
    double preco;
    char geoCodigo[20];


    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening the file %s.\n", fileName);
        exit(1);
    }

    // Lê as informações de cada usuário no arquivo e cria um novo nó na lista para cada um.
    while (fscanf(file, "%d, %d, %d, %d, %19[^,], %lf", &idVeiculo, &tipo, &estado, &autonomia, &geoCodigo, &preco) == 5) {

        // Cria um novo usuário e insere na lista.
        veiculo *veiculo = malloc(sizeof(veiculo));

        veiculo->idVeiculo = idVeiculo;
        veiculo->tipo = tipo;
        veiculo->estado = estado;
        veiculo->autonomia = autonomia;
        strcpy(veiculo->geoCodigo, &geoCodigo);
        veiculo->preco = preco;

        addVeiculo(veiculo, listaVeiculos);
    }
    fclose(file);
}


void addVeiculo(veiculo *veiculo, listaVeiculos *list){

    veiculoNode *current = NULL;

    if(list->head == NULL){

        list->head = createNodeVeiculo(veiculo);
    }
    else {
        current = list->head;
        while (current->seguinte!=NULL){
            current = current->seguinte;
        }
        current->seguinte= createNode(veiculo);
    }
}

listaVeiculos * criarListaVeiculos(){
    listaVeiculos * list = malloc(sizeof(listaVeiculos));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

veiculoNode* createNodeVeiculo(veiculo *data){

    veiculoNode *newNode = malloc(sizeof(veiculoNode));

    if (!newNode) {
        return NULL;
    }
    newNode->veiculo = data;
    newNode->seguinte = NULL;
    return newNode;
}

void apagarVeiculo(int idVeiculo, listaVeiculos *list){

    veiculoNode *current = list->head;
    veiculoNode *previous = current;

    while(current != NULL){

        if(current->veiculo->idVeiculo == idVeiculo){

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

void alterarVeiculo(listaVeiculos *list, int idVeiculo, int tipo, int estado, int autonomia, char geoCodigo[20], double preco) {

    veiculoNode *current = list->head;

    if(list->head == NULL)
        return;

    for(; current != NULL; current = current->seguinte) {

        if(current->veiculo->idVeiculo == idVeiculo) {

            current->veiculo->idVeiculo = idVeiculo;
            current->veiculo->tipo = tipo;
            current->veiculo->estado = estado;
            current->veiculo->autonomia = autonomia;
            strcpy(current->veiculo->geoCodigo, &geoCodigo);
            current->veiculo->preco = preco;
        }
    }
}


void printVeiculos(listaVeiculos * list) {

    veiculoNode * current = list->head;

    if(list->head == NULL) {
        printf("Erro");
        return;
    }

    for(; current != NULL; current = current->seguinte) {

        printf("Veiculo ID: %d\n", current->veiculo->idVeiculo);
        printf("Tipo: %d\n", current->veiculo->tipo);
        printf("Estado: %d\n", current->veiculo->estado);
        printf("Autonomia: %d\n", current->veiculo->autonomia);
        printf("Localização: %c\n", current->veiculo->geoCodigo);
        printf("Preco: %0.2f\n\n\n", current->veiculo->preco);
    }
}

void guardaListaVeiculos(listaVeiculos *lista) {

    FILE* fp;

    veiculoNode * current = lista->head;

    if (lista->head == NULL)
        return;
    if ((fp = fopen("C:\\Users\\morei\\Desktop\\EDA\\a16443_Fase1\\saveVeiculos.bin", "wb")) == NULL)
    {
        return;
    }

    for(; current != NULL; current = current->seguinte) {

        fwrite(&(current->veiculo->idVeiculo), sizeof(int), 1, fp);
        fwrite(&(current->veiculo->tipo), sizeof(int), 1, fp);
        fwrite(&(current->veiculo->estado), sizeof(int), 1, fp);
        fwrite(&(current->veiculo->autonomia), sizeof(int), 1, fp);
        fwrite(&(current->veiculo->geoCodigo), sizeof(char), sizeof(current->veiculo->geoCodigo), fp );
        fwrite(&(current->veiculo->preco), sizeof(double), 1, fp);
    }

    fclose(fp);
}

void atualizaDadosVeiculos(char* fileName, listaVeiculos *lista) {

    FILE *fp;

    veiculoNode *current = lista->head;

    if (lista->head == NULL) {
        return;
    }

    if ((fp = fopen(fileName, "w")) == NULL) {  // Abrir arquivo em modo escrita ("w")
        return;
    }

    for (; current != NULL; current = current->seguinte) {

        fwrite(&(current->veiculo->idVeiculo), sizeof(int), 1, fp);
        fwrite(&(current->veiculo->tipo), sizeof(int), 1, fp);
        fwrite(&(current->veiculo->estado), sizeof(int), 1, fp);
        fwrite(&(current->veiculo->autonomia), sizeof(int), 1, fp);
        fwrite(&(current->veiculo->geoCodigo), sizeof(char), sizeof(current->veiculo->geoCodigo), fp);
        fwrite(&(current->veiculo->preco), sizeof(double), 1, fp);
        fclose(fp);
    }
}

int qttVeiculosLocalizacao(listaVeiculos *list, char geoCodigo[]){

    veiculoNode *current = list->head;
    int qtt = 0;

    if(list->head == NULL)
        return 0;

    for(; current != NULL; current = current->seguinte) {

        if(current->veiculo->geoCodigo == geoCodigo) {
            qtt++;
        }
    }
    return qtt;
}

void listarAutonomiasDecrescente (listaVeiculos *list){

    veiculoNode *current = list->head;
    int aux=1000, tot=0, subTot=0;

    if(list->head == NULL)
        return;

    for(tot; current != NULL; current = current->seguinte) {
    tot++;
    }

    for(; subTot != tot; current = current->seguinte) {

        if(current->veiculo->autonomia <= aux) {
            aux = current->veiculo->autonomia;
            printf("%d", current->veiculo->autonomia);
            subTot++;
        }

        if (current->seguinte == NULL) {
            current = list->head;
        }
    }
    return;
}