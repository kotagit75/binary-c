#include "../../src/lib/main.h"


int main(int argc, char *argv[]){
    if(argc<2)return 1;
    int num=0;
    sscanf(argv[1], "%d", &num);
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
    for(int i=0;i<num;i++){
        struct compound *comp=malloc(sizeof(struct compound));
        compound_init(comp);
        for(int i=0;i<putAcode1_len;i++)
            compound_code_push(comp,putAcode1[i]);
        bio_env_compound_push(comp);
        struct compound *comp1=malloc(sizeof(struct compound));
        compound_init(comp1);
        for(int i=0;i<putAcode2_len;i++)
            compound_code_push(comp1,putAcode2[i]);
        bio_env_compound_push(comp1);
        struct compound *comp2=malloc(sizeof(struct compound));
        compound_init(comp2);
        for(int i=0;i<putAcode3_len;i++)
            compound_code_push(comp2,putAcode3[i]);
        bio_env_compound_push(comp2);
        struct compound *comp3=malloc(sizeof(struct compound));
        compound_init(comp3);
        for(int i=0;i<putAcode4_len;i++)
            compound_code_push(comp3,putAcode4[i]);
        bio_env_compound_push(comp3);
        struct compound *comp4=malloc(sizeof(struct compound));
        compound_init(comp4);
        for(int i=0;i<putAcode5_len;i++)
            compound_code_push(comp4,putAcode5[i]);
        bio_env_compound_push(comp4);
    }
    return 0;
}