#include "../../src/lib/main.h"


int main(int argc, char *argv[]){
    if(argc<2)return 1;
    struct cell *cell=malloc(sizeof(struct cell));
    int lifespan=0;
    sscanf(argv[1], "%d", &lifespan);
    cell_init(cell,lifespan);
    {
        int putAcode1[]={
            0x48,0xc7,0xc7,0x01,0x00,0x00,0x00
        };
        int putAcode1_len=7;
        int putAcode2[]={
            0x48,0xc7,0xc6,0x78,0x00,0x40,0x00
        };
        int putAcode2_len=7;
        int putAcode3[]={
            0x48,0x83,0xc6,0x22
        };
        int putAcode3_len=4;
        int putAcode4[]={
            0x48,0xc7,0xc2,0x01,0x00,0x00,0x00
        };
        int putAcode4_len=7;
        int putAcode5[]={
            0x48,0x31,0xc0,0xb0,0x01,0x0f,0x05
        };
        int putAcode5_len=7;
        struct protein *prot2=malloc(sizeof(struct protein));
        protein_init(prot2);
        struct compound *comp=malloc(sizeof(struct compound));
        compound_init(comp);
        for(int i=0;i<putAcode1_len;i++)
            compound_code_push(comp,putAcode1[i]);
        protein_compound_push(prot2,comp);
        struct compound *comp1=malloc(sizeof(struct compound));
        compound_init(comp1);
        for(int i=0;i<putAcode2_len;i++)
            compound_code_push(comp1,putAcode2[i]);
        protein_compound_push(prot2,comp1);
        struct compound *comp2=malloc(sizeof(struct compound));
        compound_init(comp2);
        for(int i=0;i<putAcode3_len;i++)
            compound_code_push(comp2,putAcode3[i]);
        protein_compound_push(prot2,comp2);
        struct compound *comp3=malloc(sizeof(struct compound));
        compound_init(comp3);
        for(int i=0;i<putAcode4_len;i++)
            compound_code_push(comp3,putAcode4[i]);
        protein_compound_push(prot2,comp3);
        struct compound *comp4=malloc(sizeof(struct compound));
        compound_init(comp4);
        for(int i=0;i<putAcode5_len;i++)
            compound_code_push(comp4,putAcode5[i]);
        protein_compound_push(prot2,comp4);
        cell_protein_push(cell,prot2);
    }
    {
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
    }
    DNA_setbone(cell->DNA,cell->protein);
    bio_env_cell_push(cell,NULL);
    cell_free(cell);
    return 0;
}