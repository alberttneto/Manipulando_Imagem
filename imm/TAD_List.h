#ifndef TAD_LIST_H_INCLUDED
#define TAD_LIST_H_INCLUDED

struct point{
    int x, y;
};

//Estruturas
typedef struct point point;
typedef struct list_node List_node;
typedef struct list List;

//Funções do TAD
List* list_create();
int list_push_front(List *li, point pt);
int list_pop_front(List *li);
int list_free(List *li);
int top_pos(List *li, int *prev, int col);
int list_front(List *li, point *pt);
int list_size(List *li);
int list_print(List* li);
int empty(List *st);



#endif // TAD_LIST_H_INCLUDED
