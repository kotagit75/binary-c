#include "main.h"

void codn_init(struct codn *codn){
    codn->compound=NULL;
    for (int i=0;i<MAX_compound_element;i++){
        codn->data[i]=-1;
    }
}

int codn_setcomp(struct codn *codn,struct compound *comp){
    if(codn->compound!=NULL)goto zeroret;
    int i;
    for (i=0;i<comp->len;i++){
        //printf("codn:%d,comp:%d",codn->data[i],comp->elem.elements[i]);
        if(codn->data[i]!=comp->elem.elements[i])goto zeroret;
    }
    if(i==0)goto zeroret;
    codn->compound=comp;
    return 1;
    zeroret:
    return 0;
}

void codn_print(struct codn *codn){
    printf("<");
    for(int i=0;codn->data[i]!=-1;i++){
        printf("%02x",codn->data[i]);
        if(i+1!=MAX_compound_element && codn->data[i+1]!=-1) printf(" ");
    }
    printf(">");
}

void codn_print_compound(struct codn *codn){
    if(codn->compound!=NULL)compound_print(codn->compound);
}

void codn_free(struct codn *codn){
    compound_free(codn->compound);
    free(codn);
}