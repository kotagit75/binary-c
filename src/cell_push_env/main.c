#include "../lib/main.h"

int main(int argc, char *argv[]){
    if(argc<2)return 1;
    struct cell *cell=cell_fileload(argv[1]);
    cell->fname=NULL;
    bio_env_cell_push(cell,NULL);
}