#include "../../src/lib/main.h"

int main(int argc, char *argv[]){
    if(argc<2)return 1;
    int num=0;
    sscanf(argv[1], "%d", &num);
    int exitcode1[]={
        0xc3
    };
    int exitcode1_len=1;
    for(int i=0;i<num;i++){
        struct compound *comp=malloc(sizeof(struct compound));
        compound_init(comp);
        for(int i=0;i<exitcode1_len;i++)
            compound_code_push(comp,exitcode1[i]);
        bio_env_compound_push(comp);
        compound_free(comp);
    }
    return 0;
}