typedef struct img IMG;

// Funções:
IMG* IMG_create(int nrows, int ncolumns);
int IMG_write(IMG* img, int i, int j, int valor);
int IMG_query(IMG* img, int i, int j, int *elem);
void IMG_specifications(IMG* img, int *rows, int *columns, int *sizeI);
void IMG_print(IMG* img);
void IMG_free(IMG* mat);

// Comandos:
IMG* IMG_opentxt(char *file);
IMG* IMG_openim(char *file);
void IMG_convert(char *filein, char *fileout);
void IMG_write_filetxt(char *fileout, IMG *img);
void IMG_write_fileim(char *fileout, IMG *img);
