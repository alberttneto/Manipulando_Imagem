#include <stdio.h>
#include <stdlib.h>
#include "TAD_List.h"

struct list_node{
    List_node *next;
    point p;
};

struct list{
    List_node *head;
    int qtd;
};

// Cria lista
List* list_create(){

    List *li;

    li = malloc(1*sizeof(List));

    if(li == NULL){
        return li;
    }

    li->head = NULL;
    li->qtd = 0;

    return li;
}

// Inserir elemento na cabeça da lista
int list_push_front(List *li, point pt){

    if(li == NULL){
        return 0;
    }
    List_node *node;
    node = malloc(sizeof(List_node));
    node->next = li->head;
    li->head = node;
    li->qtd++;
    node->p = pt;
    return 1;
}

// Imprimindo lista
int list_print(List* li){

    if(li == NULL){
        return 0;
    }

    List_node *node;
    node = li->head;

    while(node != NULL){
        printf("i: %d, j: %d\n", node->p.x, node->p.y);
        node = node->next;
    }
    return 1;
}

// Consulta primeiro elemento da lista
int list_front(List *li, point *pt){

    if(li == NULL){
        return 0;
    }else if(li->head == NULL){
        return 0;
    }

    *pt = li->head->p;
    return 1;
}

// Remove primeiro elemento da lista
int list_pop_front(List *li){

    if(li == NULL){
        return 0;
    }else if(li->head == NULL){
        return 0;
    }

    List_node *aux;
    aux = li->head->next;
    free(li->head);
    li->head = aux;
    li->qtd--;

    return 1;
}

// Consulta segundo elemento da lista
int top_pos(List *li, int *prev, int col){

    if(li == NULL || li->qtd <= 1){
        return -1;
    }

    List_node *node;
    node = li->head->next;
    *prev = (node->p.x*col)+node->p.y;

    return 1;
}

// Liberando lista
int list_free(List *li){

    if(li == NULL){
        return 0;
    }
    if(li->head == NULL){
        free(li);
        return 1;
    }

    List_node *node, *aux;
    node = li->head;


    while(node != NULL){
        aux = node->next;
        free(node);
        node = aux;
    }
    free(li);
    return 1;
}

// Verifica lista vazia
int empty(List *st){

    if(st->qtd != 0){
        return 0;
    }

    return 1;
}

//Retorna tamanho da lista
int list_size(List *li){

    return li->qtd;
}
