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
    int exitcode1[]={
        0x48,
        0x31,
        0xff
    };
    int exitcode1_len=3;
    int exitcode2[]={
            0x48,
            0x31,
            0xc0,
            0xb0,
            0x3c,
            0xf,
            0x5
    };
    int exitcode2_len=7;
    int exitcode3[]={
            0xc3
    };
    int exitcode3_len=1;
    for(int i=0;i<exitcode1_len;i++)
        compound_code_push(comp,exitcode1[i]);
    protein_compound_push(prot,comp);
    struct compound *comp1=malloc(sizeof(struct compound));
    compound_init(comp1);
    for(int i=0;i<exitcode2_len;i++)
        compound_code_push(comp1,exitcode2[i]);
    protein_compound_push(prot,comp1);
    struct compound *comp2=malloc(sizeof(struct compound));
    compound_init(comp2);
    for(int i=0;i<exitcode3_len;i++)
        compound_code_push(comp2,exitcode3[i]);
    protein_compound_push(prot,comp2);
    cell_protein_push(cell,prot);
    DNA_setbone(cell->DNA,cell->protein);
    bio_env_cell_push(cell,NULL);
    cell_free(cell);
    return 0;
}