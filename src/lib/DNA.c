#include "main.h"

void DNA_init(struct DNA *DNA){
    DNA->codn=malloc(sizeof(list));
    list_init(DNA->codn);
    DNA->protein=malloc(sizeof(list));
    list_init(DNA->protein);
}

void DNA_codn_push(struct DNA *DNA,struct codn *codn){
    list_insertion(DNA->codn,codn);
}

void DNA_print(struct DNA *DNA){
    node *ptr=DNA->codn->head;
    while(ptr!=NULL){
        codn_print(((struct codn*)(ptr->data)));
        if(((struct codn*)(ptr->data))->compound!=NULL)printf("->");
        codn_print_compound(((struct codn*)(ptr->data)));
        if(ptr->next!=NULL)printf("=");
        ptr=ptr->next;
    }
}

void DNA_setbone(struct DNA *DNA,list *list){
    node *ptr=list->head;
    while(ptr!=NULL){
        //
        struct protein* prot = malloc(sizeof(struct protein));
        protein_init(prot);
        node *_ptr=((struct protein*)(ptr->data))->compound->head;
        while(_ptr!=NULL){
            struct compound* comp = malloc(sizeof(struct compound));
            compound_init(comp);
            for(int i=0;i<((struct compound*)(_ptr->data))->len;i++){
                comp->elem.elements[i]=((struct compound*)(_ptr->data))->elem.elements[i];
            }
            comp->len=((struct compound*)(_ptr->data))->len;
            comp->size=((struct compound*)(_ptr->data))->size;
            list_insertion(prot->compound,comp);
            _ptr=_ptr->next;
        }
        list_insertion(DNA->protein,prot);
        ptr=ptr->next;
    }
}

void DNA_free(struct DNA *DNA){
    if (DNA == NULL)return;
    
    if(DNA->protein!=NULL){
        node *ptr=DNA->protein->head;
        while(ptr!=NULL){
            protein_free((struct protein*)(ptr->data));
            ptr=ptr->next;
        }
    }
    node *ptr=DNA->codn->head;
    while(ptr!=NULL){
        codn_free((struct codn*)(ptr->data));
        ptr=ptr->next;
    }
    free(DNA->codn);
    free(DNA->protein);
    if (DNA == NULL)free(DNA);
}