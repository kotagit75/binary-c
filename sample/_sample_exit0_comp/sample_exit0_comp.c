#include "../../src/lib/main.h"


int main(int argc, char *argv[]){
    if(argc<2)return 1;
    int num=0;
    sscanf(argv[1], "%d", &num);
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
    for(int i=0;i<num;i++){
        struct compound *comp=malloc(sizeof(struct compound));
        compound_init(comp);
        for(int i=0;i<exitcode1_len;i++)
            compound_code_push(comp,exitcode1[i]);
        bio_env_compound_push(comp);
        compound_free(comp);
        struct compound *comp1=malloc(sizeof(struct compound));
        compound_init(comp1);
        for(int i=0;i<exitcode2_len;i++)
            compound_code_push(comp1,exitcode2[i]);
        bio_env_compound_push(comp1);
        compound_free(comp1);
    }
    return 0;
}