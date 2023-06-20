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
#include "../Veiculos/veiculos.h"

#define _CRT_SECURE_NO_WARNINGS


/**
 * @brief L� os veiculos do ficheiro txt
 * @param fileName
 * @param listaVeiculos
 * @param grafo
 */
void LerVeiculos(char* fileName, ListaVeiculos *listaVeiculos, Grafo* grafo) {
    FILE* file;
    int idVeiculo, tipo, estado, autonomia;
    double preco;
    char geoCodigo[20];
    int peso;

    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening the file %s.\n", fileName);
        exit(1);
    }

    // Lê as informações de cada veículo no arquivo e cria um novo nó na lista para cada um.
    while (fscanf(file, "%d, %d, %d, %d, %19[^,], %lf, %d", &idVeiculo, &tipo, &estado, &autonomia, geoCodigo, &preco, &peso) == 7) {

        // Cria um novo veículo e insere na lista.
        Veiculo* veiculo = malloc(sizeof(Veiculo));

        veiculo->idVeiculo = idVeiculo;
        veiculo->tipo = tipo;
        veiculo->estado = estado;
        veiculo->autonomia = autonomia;
        strcpy(veiculo->geoCodigo, geoCodigo);
        veiculo->preco = preco;
        veiculo->peso = peso;

        adicionarVertice(grafo, veiculo->geoCodigo, veiculo);

        VeiculoNode *veiculoNode = CreateNodeVeiculo(veiculo);
        AddVeiculo(veiculoNode,  listaVeiculos);
    }
    fclose(file);
}

/**
 * @brief Fun��o para adicionar um veiculo a uma lista de veiculos
 * @param list
 * @param veiculo
 */
void AddVeiculo(VeiculoNode *veiculoNode, ListaVeiculos *list) {
    VeiculoNode *current = NULL;

    if (list->head == NULL) {
        list->head = veiculoNode;
    } else {
        current = list->head;
        while (current->seguinte != NULL) {
            current = current->seguinte;
        }
        current->seguinte = veiculoNode;
    }
}

/**
 * @brief Fun��o para criar um n� que aponta para o pr�ximo veiculo
 * @param data
 */
VeiculoNode* CreateNodeVeiculo(Veiculo *data) {
    VeiculoNode *newNode = malloc(sizeof(VeiculoNode));

    if (!newNode) {
        return NULL;
    }

    newNode->veiculo = data;
    newNode->seguinte = NULL;
    return newNode;
}

/**
 * @brief Fun��o para criar uma lista de veiculos
 */
ListaVeiculos * CriarListaVeiculos(){
    ListaVeiculos * list = malloc(sizeof(ListaVeiculos));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

/**
 * @brief Fun��o que apaga um veiculo de uma lista
 * @param idVeiculo
 * @param list
 */
void ApagarVeiculo(int idVeiculo, ListaVeiculos *list){

    VeiculoNode *current = list->head;
    VeiculoNode *previous = current;

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
void AlterarVeiculo(ListaVeiculos *list, int idVeiculo, int tipo, int estado, int autonomia, char geoCodigo[20], double preco) {

    VeiculoNode *current = list->head;

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

/**
 * @brief Fun��o para imprimir a lista de veiculos
 * @param list
 */
void PrintVeiculos(ListaVeiculos * list) {

    VeiculoNode * current = list->head;

    if(list->head == NULL) {
        printf("Erro");
        return;
    }

    for(; current != NULL; current = current->seguinte) {

        printf("Veiculo ID: %d\n", current->veiculo->idVeiculo);
        printf("Tipo: %d\n", current->veiculo->tipo);
        printf("Estado: %d\n", current->veiculo->estado);
        printf("Autonomia: %d\n", current->veiculo->autonomia);
        printf("Localiza��o: %c\n", current->veiculo->geoCodigo);
        printf("Preco: %0.2f\n\n\n", current->veiculo->preco);
    }
}

/**
 * @brief Fun��o para guardar a lista de utilizadores num ficheiro bin�rio
 * @param listaVeiculos
 */
void GuardaListaVeiculos(ListaVeiculos *lista) {

    FILE* fp;

    VeiculoNode * current = lista->head;

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

/**
 * @brief Fun��o para atualizar os veiculos no ficheiro txt
 * @param fileName
 * @param listaVeiculos
 */
void AtualizaDadosVeiculos(char* fileName, ListaVeiculos *lista) {

    FILE *fp;

    VeiculoNode *current = lista->head;

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

/**
 * @brief Conta o n�mero de ve�culos numa determinada localiza��o
 * @param list
 * @param geoCodigo
 * @return
 */
int QttVeiculosLocalizacao(ListaVeiculos *list, char geoCodigo[20]){

    VeiculoNode *current = list->head;
    int qtt;

    if(list->head == NULL)
        return 0;

    for(; current != NULL; current = current->seguinte) {

        if(strcmp(current->veiculo->geoCodigo, geoCodigo) == 0) {
            qtt++;
        }
    }
    return qtt;
}

/**
 * @brief Lista a autonomia dos veiculos do menorr para o maior
 * @param list
 */
void ListarAutonomiasDecrescente(ListaVeiculos *list) {
    VeiculoNode *current = list->head;
    int maxAutonomia = 0;
    int totalVeiculos = 0;

    //Conta o total de veículos na lista
    while (current != NULL) {
        totalVeiculos++;
        current = current->seguinte;
    }

    for (int i = 0; i < totalVeiculos; i++) {
        current = list->head;
        int currentAutonomia = 0;

        while (current != NULL) {
            if (current->veiculo->autonomia > currentAutonomia && current->veiculo->autonomia <= maxAutonomia) {
                currentAutonomia = current->veiculo->autonomia;
            }
            current = current->seguinte;
        }

        current = list->head;
        while (current != NULL) {
            if (current->veiculo->autonomia == currentAutonomia) {
                printf("Veiculo ID: %d\n", current->veiculo->idVeiculo);
                printf("Tipo: %d\n", current->veiculo->tipo);
                printf("Estado: %d\n", current->veiculo->estado);
                printf("Autonomia: %d\n", current->veiculo->autonomia);
                printf("Localizacao: %s\n", current->veiculo->geoCodigo);
                printf("Preco: %.2f\n\n", current->veiculo->preco);
                break;
            }
            current = current->seguinte;
        }

        maxAutonomia = currentAutonomia;
    }
}

