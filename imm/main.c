#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD_IMG.h"
#include "TAD_List.h"


// Fazer thresholding em uma imagem e salvar resultado em outra imagem
void segment(char *num, char *filein, char *fileout){

    // Lê arquivo salva em matriz IMG e mostra na tela
    IMG *img;
    printf("Imagem: \n==================================\n");
    img = IMG_openim(filein);

    // Salva configurações da matriz
    int row, col, sizeI, i = 0, j = 0, elem, thr;
    IMG_specifications(img, &row, &col, &sizeI);
    // Converte de string para inteiro
    thr = atoi(num);

    // Percorrer matriz e alterar elementos de acordo com thr
    while(i != row || j != col-1){

        IMG_query(img, i, j, &elem);

        // Elementos maiores ou iguais a thr salva como 1
        if(elem >= thr){
            IMG_write(img, i, j, 1);
        // Elementos menores a thr salva como 0
        }else if(elem < thr){
            IMG_write(img, i, j, 0);
        }

        //Contagem para saltar de linha na matriz
        if(j == (col-1)){
            i++;
            j = -1;
        }
        j++;
    }

    //Mostra imagem segmentada e libera memoria
    printf("\n Segment: \n==================================\n");
    IMG_print(img);

    IMG_write_fileim(fileout, img);
    IMG_free(img);
}


//Identificar componentes conexos
void cc(char *filein, char *fileout){

    // IMG para receber componentes e IMG para rotular componentes
    IMG *img, *imgout;
    printf(" Componentes conexos: \n ===================================\n");
    img = IMG_openim(filein);

    int row, col, sizeI;
    IMG_specifications(img, &row, &col, &sizeI);

    // Criando matriz img para salvar a rotulação dos componentes
    imgout = IMG_create(row, col);

    //Lista para salvar posição dos elemntos de um componente convexos
    List *search_next;
    search_next = list_create();
    point pt;

    //Variavel id para rotular elementos
    //Variavel elemin e elemout para consultar elemtnos na imagem de entrada e saida
    //Flag para controle de qual celula ja foi verificada
    int id = 1, i, j, elemin, elemout, flag;

    //Percorrer toda matriz para identificar os objetos na imagem
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){

            //Verifica se encontra o primeiro objeto da IMG
            IMG_query(img, i, j, &elemin);
            IMG_query(imgout, i, j, &elemout);
            if(elemin == 1 && elemout == 0){
                // Salva posição na lista
                pt.x = i;
                pt.y = j;
                list_push_front(search_next, pt);
                // Escreve na matriz de saida a rotulação do objeto
                IMG_write(imgout, i, j, id);

                //Busca por elementos vizinhos do mesmo objeto para rotulação
                while(!empty(search_next)){
                    // Flag 0 não tem mais elementos para rotular
                    // Flag 1 identifica novo elemento do mesmo objeto
                    flag = 0;

                    // Verifica vizinhos da posição do elemento salva
                    IMG_query(img, pt.x, pt.y+1, &elemin);
                    IMG_query(imgout, pt.x, pt.y+1, &elemout);
                    if(elemin == 1 && elemout == 0 && flag != 1){
                        pt.x = pt.x;
                        pt.y = pt.y+1;
                        IMG_write(imgout, pt.x, pt.y, id);
                        list_push_front(search_next, pt);
                        flag = 1;
                    }

                    IMG_query(img, pt.x, pt.y-1, &elemin);
                    IMG_query(imgout, pt.x, pt.y-1, &elemout);
                    if(elemin == 1 && elemout == 0 && flag != 1){
                        pt.x = pt.x;
                        pt.y = pt.y-1;
                        IMG_write(imgout, pt.x, pt.y, id);
                        list_push_front(search_next, pt);
                        flag = 1;
                    }

                    IMG_query(img, pt.x+1, pt.y, &elemin);
                    IMG_query(imgout, pt.x+1, pt.y, &elemout);
                    if(elemin == 1 && elemout == 0 && flag != 1){
                        pt.x = pt.x+1;
                        pt.y = pt.y;
                        IMG_write(imgout, pt.x, pt.y, id);
                        list_push_front(search_next, pt);
                        flag = 1;
                    }

                    IMG_query(img, pt.x-1, pt.y, &elemin);
                    IMG_query(imgout, pt.x-1, pt.y, &elemout);
                    if(elemin == 1 && elemout == 0 && flag != 1){
                        pt.x = pt.x-1;
                        pt.y = pt.y;
                        IMG_write(imgout, pt.x, pt.y, id);
                        list_push_front(search_next, pt);
                        flag = 1;
                    }

                    //Se não tiver vizinhos para rotular..
                    //Pega elemento da lista e faz nova verificação..
                    //Até que a lista esteja vazia.
                    if(flag == 0){
                        list_pop_front(search_next);
                        list_front(search_next, &pt);
                    }
                }
                //A cada objeto rotulado muda o identificador
                id++;
            }
        }
    }

    //Mostra resultado da rotulação dos componentes conexos
    printf("\n Rotulacao componentes conexo: \n ========================================\n");
    IMG_print(imgout);

    //Escreve no arquivo a IMG resultante e libera memoria
    IMG_write_fileim(fileout, imgout);
    list_free(search_next);
    IMG_free(img);
    IMG_free(imgout);
}

//Resolve um labirinto em arquivo txt e salva resultado em novo arquivo
void labirinto(char *filein, char *fileout){

    // Lê arquivo e salva labirinto
    IMG *img;
    printf("Labirinto: \n==============================\n");
    img = IMG_opentxt(filein);

    // Variavel para salvar posições do lab
    point lab;

    int i = 0, row, col, sizeI, elem;
    IMG_specifications(img, &row, &col, &sizeI);

    // Identifica inicio do lab na primeira coluna
    for(i = 0; i < row; i++){
        IMG_query(img, i, 0, &elem);
        if(elem == 1){
            lab.x = i;
            lab.y = 0;
        }
    }

    // Lista route salva caminho percorrido no labirinto
    // Lista checkpt salva posição onde existe mais de um caminho possivel
    List *route, *checkpt;
    route = list_create();
    checkpt = list_create();

    // Inseri primeira posição do lab
    list_push_front(route, lab);

    // pos salva posição da matriz em valor de vetor
    // cont recebe quantidade de caminhos possiveis
    // z e w salva nova posição
    int pos, cont, prev, z = 0, w = 0;

    // LOOP até que chegue na ultima coluna da matriz (ou seja final do lab);
    while(lab.y != (col-1)){

        cont = 0;
        pos = (col*lab.x)+lab.y;
        top_pos(route, &prev, col);

        // Pos e Prev são comparados para não voltar em posição anterior
        // Procura por novos caminhos
        IMG_query(img, lab.x, lab.y-1, &elem);
        if(lab.y >= 0 && elem == 1 && pos-1 != prev){
            cont++;
            z = lab.x;
            w = lab.y-1;
        }
        IMG_query(img, lab.x, lab.y+1, &elem);
        if(lab.y < col && elem == 1 && pos+1 != prev){
            cont++;
            z = lab.x;
            w = lab.y+1;
        }
        IMG_query(img, lab.x-1, lab.y, &elem);
        if(lab.x >= 0 && elem == 1 && pos-col != prev){
            cont++;
            z = lab.x-1;
            w = lab.y;
        }
        IMG_query(img, lab.x+1, lab.y, &elem);
        if(lab.x < row && elem == 1 && pos+col != prev){
            cont++;
            z = lab.x+1;
            w = lab.y;
        }

        // Se houver mais de um caminho, salva ponto
        // Inicia pela ultimo caminho salvo nas variaveis z e w
        if(cont > 1){
            list_push_front(checkpt, lab);
            lab.x = z;
            lab.y = w;

            // Salva novo caminho na rota
            list_push_front(route, lab);

        // Se tiver só um caminho possivel, continuar percorrendo o mesmo
        }else if(cont == 1){

            lab.x = z;
            lab.y = w;
            list_push_front(route, lab);

        // Se não tiver mais caminho possivel, voltar para ultimo ponto salvo
        }else{

            point ck;
            list_front(checkpt, &ck);
            list_pop_front(checkpt);

            while(lab.x != ck.x || lab.y != ck.y){

                IMG_write(img, lab.x, lab.y, 0);
                list_pop_front(route);
                list_front(route, &lab);
            }
        }
    }

    IMG *imgout;
    imgout = IMG_create(row, col);

    //Salva unico caminho possivel na matriz IMG de saida
    i = 0;
    int si = list_size(route);
    while(i < si){

        list_front(route, &lab);
        list_pop_front(route);
        IMG_write(imgout, lab.x, lab.y, 2);
        i++;
    }

    // Escreve resultado no arquivo de saida e imprimi lab pronto na tela
    i = 0;
    printf("\nResultado labirinto: \n==============================\n");
    IMG_print(imgout);

    IMG_write_filetxt(fileout, imgout);
    // Libera memoria
    list_free(route);
    list_free(checkpt);
    IMG_free(img);
    IMG_free(imgout);
}

int main(int argc, char *argv[])
{
    IMG *img;

    // Recebe argumentos de entrada e chama função desejada
    if(strcmp(argv[1], "-opentxt") == 0){
        img = IMG_opentxt(argv[2]);
        IMG_free(img);
    } else if(strcmp(argv[1], "-openim") == 0){
        img = IMG_openim(argv[2]);
        IMG_free(img);
    } else if(strcmp(argv[1], "-convert") == 0){
        IMG_convert(argv[2], argv[3]);
    } else if(strcmp(argv[1], "-segment") == 0){
        segment(argv[2], argv[3], argv[4]);
    } else if(strcmp(argv[1], "-cc") == 0){
        cc(argv[2], argv[3]);
    } else if(strcmp(argv[1], "-lab") == 0){
        labirinto(argv[2], argv[3]);
    }else{
        printf("Comando não existe");
    }

    return 0;
}
