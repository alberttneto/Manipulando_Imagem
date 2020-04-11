#include "TAD_IMG.h"
#include "TAD_List.h"
#include <stdio.h>
#include <stdlib.h>

struct img{
    int nrows;
    int ncolumns;
    int *data;
};

// Aloca matriz de acordo com as dimensões da imagem
IMG* IMG_create(int nrows, int ncolumns){

    IMG *img;
    img = malloc(sizeof(IMG));
    img->nrows = nrows;
    img->ncolumns = ncolumns;

    // Já aloca com tudo incializado com 0
    img->data = calloc((nrows*ncolumns), sizeof(int));

    return img;
}


// Inseri valor em determinada celula da matriz
int IMG_write(IMG* img, int i, int j, int valor){

    // Verifica se a matriz e as posições são existentes
    if(img == NULL || i < 0 || j < 0 || i >= img->nrows || j >= img->ncolumns){
        return -1;
    }
    // Converte linha e coluna em posição de vetor
    int pos;
    pos = (img->ncolumns*i)+j;
    img->data[pos] = valor;

    return 0;
}

// Consulta elemento em determinada celula na matriz
int IMG_query(IMG* img, int i, int j, int *elem){

    if(img == NULL || i < 0 || j < 0 || i >= img->nrows || j >= img->ncolumns){
        return -1;
    }
    int pos;
    pos = (i*img->ncolumns)+j;
    *elem = img->data[pos];

    return 0;
}

// Consulta linha, coluna e tamanho da matriz
void IMG_specifications(IMG* img, int *rows, int *columns, int *sizeI){
    *rows = img->nrows;
    *columns = img->ncolumns;
    *sizeI = img->ncolumns*img->nrows;
}

// Libera memoria alocada
void IMG_free(IMG* img){
    free(img->data);
    free(img);
}

// Mostra imagem em formato de matriz na tela
void IMG_print(IMG* img){
    int i;

    for(i = 0; i < (img->nrows * img->ncolumns); i++){
        printf("[%d]", img->data[i]);
        if((i+1)%img->ncolumns == 0){
            printf("\n");
        }
    }
}

// Escreve Matriz alocada em determinado arquivo binario
void IMG_write_fileim(char *fileout, IMG *img){

    FILE *fp;
    fp = fopen(fileout, "wb");
    if(fp == NULL){
        printf("Erro ao abrir arquivo");
        system("pause");
    }

    int row, col, i = 0, j = 0, max = img->data[0];
    row = img->nrows;
    col = img->ncolumns;

    // Maior elemento da imagem
    while(i < row*col){
        if(img->data[i] > max){
            max = img->data[i];
        }
        i++;
    }

    // Cabeçalho da imagem
    fprintf(fp, "P2\n");
    fprintf(fp, "%d %d\n", col, row);
    fprintf(fp, "%d\n", max);
    i = 0;
    int sizeI = row*col;

    // Escrevendo imagem no arquivo
    while(sizeI > 0){

        fprintf(fp,"%d", img->data[col*i+j]);
        if(j == (col-1)){
            fprintf(fp,"\n");
            i++;
            j = -1;
        }else{
            fprintf(fp," ");
        }
        j++;
        sizeI--;
    }

    fclose(fp);
}

// Escreve Matriz alocada em determinado arquivo binario
void IMG_write_filetxt(char *fileout, IMG *img){

    FILE *fp;
    fp = fopen(fileout, "w");
    if(fp == NULL){
        printf("Erro ao abrir arquivo");
        system("pause");
    }

    //Diferente do arquivo binario o texto não possui cabeçario
    int row, col, i = 0, j = 0;
    row = img->nrows;
    col = img->ncolumns;

    i = 0;
    int sizeI = row*col;

    // Escrevendo imagem no arquivo
    while(sizeI > 0){

        fprintf(fp,"%d", img->data[col*i+j]);
        if(j == (col-1)){
            fprintf(fp,"\n");
            i++;
            j = -1;
        }else{
            fprintf(fp," ");
        }
        j++;
        sizeI--;
    }

    fclose(fp);
}

// Abrir arquivo binario e salva em IMG
IMG* IMG_openim(char *file){

    FILE *fp;
    // Abre um arquivo BINÁRIO para LEITURA
    fp = fopen(file, "rb");
    // Se houve erro na abertura
    if (fp == NULL){
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }


    unsigned char num[4];
    int count = 0, row = 0, col = 0;

    // Armazenando numeto de linhas e colunas da imagem
    while(count < 4){
        fscanf(fp, "%s", num);
        if(count == 1){
            col = atoi(num);
        }else if(count == 2){
            row = atoi(num);
        }
        count++;
    }

    printf("lin: %d, col: %d\n", row, col);

    // Cria matriz imagem para receber valores do arquivo
    IMG *img;
    img = IMG_create(row, col);

    int i = 0, j = 0, numInt;

    // Lê até final de arquivo
    while(!feof(fp)){
        //Recebe cada valor do arquivo como string
        fscanf(fp, "%s", num);
        // Converte valor para inteiro
        numInt = atoi(num);
        // Volta a coluna quando chega no final da linha
        if(j%col == 0 && j != 0){
            i++;
            j = 0;
        }
        // Escreve valor no arquivo
        IMG_write(img, i, j, numInt);
        j++;
    }


    fclose(fp);

    // Mostra imagem na matriz
    IMG_print(img);

    return img;
}


// Abrir arquivo texto e salva valores em IMG
IMG* IMG_opentxt(char *file){

    // Leitura do arquivo
    FILE *fp;
    fp = fopen(file, "r");
    if(fp == NULL){
        printf("Erro na abertura do arquivo\n");
    }

    unsigned char num[4];
    char c;
    int col = 0, lin = 1;

    // Contar numero de linhas e colunas
    while((c = fgetc(fp)) != EOF){
        if(c == '\t'){
            col++;
        } else if(c == '\n'){
            lin++;
        }
    }

    col = (lin+col)/lin;
    printf("lin: %d, col: %d\n", lin, col);

    // Volta para o ponto inicial do arquivo
    rewind(fp);

    // Criando matriz para armazenar valores do arquivo
    IMG *img;
    img = IMG_create(lin, col);

    int i = 0, j = 0, numInt;

    while(!feof(fp)){

        //Recebe cada valor do arquivo como string
        fscanf(fp, "%s", num);
        // Converte valor para inteiro
        numInt = atoi(num);

        if(j%col == 0 && j != 0){
            i++;
            j = 0;
        }
        // Escreve valor no arquivo
        IMG_write(img, i, j, numInt);
        j++;
    }

    fclose(fp);

    //Mostra matriz
    IMG_print(img);

    return img;
}

// Converter arquivo texto para binario
void IMG_convert(char *filein, char *fileout){

    IMG *img;
    // Abre arquivo texto e salva valores na matriz IMG
    img = IMG_opentxt(filein);
    // Escreve matriz IMG em arquivo binario
    IMG_write_fileim(fileout, img);
    // Libera memoria
    IMG_free(img);
}


