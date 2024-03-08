#include "../../src/lib/main.h"


int main(int argc, char *argv[]){
    if(argc<2)return 1;
    struct cell *cell=malloc(sizeof(struct cell));
    int lifespan=0;
    sscanf(argv[1], "%d", &lifespan);
    cell_init(cell,lifespan);
    struct protein *prot=malloc(sizeof(struct protein));
    protein_init(prot);
    struct compound *comp=malloc(sizeof(struct compound));
    compound_init(comp);
    compound_code_push(comp,0xc3);
    protein_compound_push(prot,comp);
    cell_protein_push(cell,prot);
    DNA_setbone(cell->DNA,cell->protein);
    bio_env_cell_push(cell,NULL);
    cell_free(cell);
    return 0;
}