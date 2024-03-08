#include "main.h"

void compound_init(struct compound *comp){
    comp->size=NULL;
    comp->len=0;
    comp->free=0;
    comp->fname=NULL;
}

void compound_code_push(struct compound *comp,element_t element){
    comp->elem.elements[comp->len]=element;
    comp->len++;
    comp->size=sizeof(element_t);
}

void compound_data_push(struct compound *comp,bio_data_t data){
    comp->elem.data=data;
    comp->size=sizeof(bio_data_t);
}

void compound_print(struct compound *comp){
    if(comp==NULL)return;
    printf("<");
    for(int i=0;i<comp->len;i++){
        printf("%02x",comp->elem.elements[i]);
        if(i!=comp->len-1) printf(" ");
    }
    printf(">");
}

void compound_print_data(struct compound *comp){
    printf("[%010x]",comp->elem.data);
}

int compound_type(struct compound *comp){
    if(comp->size==sizeof(bio_data_t)) return compound_TYPE_DATA;
    return compound_TYPE_CODE;
}

void compound_bond(struct compound *comp,struct bio_env *be){
    for(int i=0;i<comp->len;i++){
       be->codedata_bond[i+be->codedata_bond_len]=comp->elem.elements[i];
    }
    be->codedata_bond_len+=comp->len;
}

void compound_free(struct compound *comp){
    if(comp!=NULL && comp->free!=1 && comp->size!=NULL){
        free(comp);
        comp->free=1;
    }
}

struct compound *compound_fileload(char* name){
    json_t* comp_json;
    json_error_t json_error;
    char path[strlen(name)+strlen(COMPOUND_dir)];
    sprintf(path,"%s%s",COMPOUND_dir,name);
    comp_json = json_load_file(path, 0, &json_error);
    if (!comp_json) {
        fprintf(stderr, "%s", json_error.text);
        exit(-1);
    }
    struct compound *comp=malloc(sizeof(struct compound));
    compound_init(comp);
    json_t* elemjson=json_object_get(comp_json,"elem");
    if(elemjson){
        char* tmp;
        json_t* json_value;
		json_t* item;
		for (size_t i = 0; ; i++) {
			item = json_array_get(elemjson, i);
			if (!item) {
				break;
			}
			json_value = json_string_value(item);
			if (json_value) {
                compound_code_push(comp,strtol(json_value, &tmp, 16));
			}
			json_decref(json_value);
		}
    }
    comp->fname=name;
    json_decref(elemjson);
	json_decref(comp_json);
    return comp;
}