#include "main.h"

void cell_init(struct cell *cell,int lifespan){
    cell->DNA=malloc(sizeof(struct DNA));
    DNA_init(cell->DNA);
    cell->protein=malloc(sizeof(list));
    list_init(cell->protein);
    cell->lifespan=lifespan;
    cell->remaining_life=cell->lifespan;
    cell->fitness=50;
    strcpy(cell->fname,"unname");
}

void cell_file_push(struct cell *cell,const char *path){
    json_t *json_root = json_object();
    json_object_set(json_root,"lifespan",json_integer(cell->lifespan));
    json_object_set(json_root,"remaining_life",json_integer(cell->remaining_life));
    json_object_set(json_root,"fitness",json_integer(cell->fitness));
    int codn_num=0;
    node *ptr=cell->DNA->codn->head;
    while (ptr!=NULL){
        codn_num++;
        ptr=ptr->next;
    }
    json_object_set(json_root,"codn_n",json_integer(codn_num));
    json_t *json_proteinA = json_array();
    if (json_proteinA){
        node *ptr=cell->protein->head;
        while(ptr!=NULL){
            json_t *json_protein = json_array();
            node *_ptr=((struct protein*)(ptr->data))->compound->head;
            while(_ptr!=NULL){
                json_t *json_comp = json_array();
                for(int i=0;i<((struct compound*)(_ptr->data))->len;i++){
                    char res[10];
                    sprintf(res, "0x%x", ((struct compound*)(_ptr->data))->elem.elements[i]);
                    json_array_append(json_comp,json_string(res));
                }
                json_array_append(json_protein, json_comp);
                json_decref(json_comp);
                _ptr=_ptr->next;
            }
            json_array_append(json_proteinA, json_protein);
            json_decref(json_protein);
            ptr=ptr->next;
        }
        json_object_set(json_root, "protein", json_proteinA);
        json_decref(json_proteinA);
    }
    json_t *json_DNA = json_array();
    if (json_DNA){
        node *ptr=cell->DNA->codn->head;
        while(ptr!=NULL){
            json_t *json_codn = json_object();
            if(((struct codn*)(ptr->data))->compound)json_object_set(json_codn, "comp", json_integer(1));
            else json_object_set(json_codn, "comp", json_integer(0));
            json_array_append(json_DNA,json_codn);
            json_decref(json_codn);
            ptr=ptr->next;
        }
        json_object_set(json_root, "DNA", json_DNA);
        json_decref(json_DNA);
    }
    char* json=json_dumps(json_root, json_object_size(json_root));
    json_decref(json_root);
    FILE* fp;
    fp = fopen(path, "w");
    fprintf(fp, "%s", json);
    fclose(fp);
}

void cell_protein_push(struct cell *cell,struct protein *protein){
    node *ptr=protein->compound->head;
    int i;
    while(ptr!=NULL){
        struct codn *codn=malloc(sizeof(struct codn));
        codn_init(codn);
        struct compound *comp=((struct compound*)(ptr->data));
        for(i=0;i<comp->len;i++){
            codn->data[i]=comp->elem.elements[i];
        }
        DNA_codn_push(cell->DNA,codn);
        ptr=ptr->next;
    }
    list_insertion(cell->protein,protein);
}

void cell_print(struct cell *cell){
    node *ptr=cell->protein->head;
    printf("{");
    while(ptr!=NULL){
        protein_print(((struct protein*)(ptr->data)));
        if(ptr->next!=NULL)printf(",");
        ptr=ptr->next;
    }
    printf("}");
    DNA_print(cell->DNA);
    printf("(%d)",cell->remaining_life);
    printf("\n");
}

void cell_free(struct cell *cell){
    node *ptr=cell->protein->head;
    while(ptr!=NULL){
        protein_free(((struct protein*)(ptr->data)));
        ptr=ptr->next;
    }
    //free(cell->protein);
    DNA_free(cell->DNA);
    free(cell);
}

void metabolism_and_motion(struct cell *cell){
}

int grow(struct cell *cell,struct MO *mo){
    node *ptr=mo->comp->head;
    int isgrow=0;
    struct compound *comp;
    while (ptr!=NULL){
        comp = ((struct codn*)(ptr->data));
        node *ptr2=cell->DNA->codn->head;
        isgrow=0;
        while(ptr2!=NULL){
            if(codn_setcomp(((struct codn*)(ptr2->data)),comp)==1){comp->fname=NULL;isgrow=1;break;}
            next:
            ptr2=ptr2->next;
        }
        if(isgrow==1){
            ptr=ptr->next;
            list_deletion(mo->comp,comp);
        }
        else ptr=ptr->next;
    }
    proliferationisOK:
    node *_ptr=cell->DNA->codn->head;
    while(_ptr!=NULL){
        if(((struct codn*)(_ptr->data))->compound==NULL)goto no;
        _ptr=_ptr->next;
    }
    return 1;
    no:
    return 0;
}

void proliferation(struct cell *cell){
    struct cell *newcell=malloc(sizeof(struct cell));
    cell_init(newcell,cell->lifespan);
    node *ptr=cell->protein->head;
    node *codn_ptr=cell->DNA->codn->head;
    node *_ptr;
    while(ptr!=NULL){
        struct protein *protein=malloc(sizeof(struct protein));
        protein_init(protein);
        if((rand()%100)>=cell->fitness){
            //if((rand()%100)>=-1){
                struct compound *comp = bio_env_compound_pop_random();
                if(comp!=NULL){
                    protein_compound_push(protein,comp);
                    cell_protein_push(newcell,protein);
                    continue;
                }else{
                    _ptr=((struct protein*)(ptr->data))->compound->head;
                    while(_ptr!=NULL){
                        protein_compound_push(protein,((struct codn *)(codn_ptr->data))->compound);
                        codn_ptr=codn_ptr->next;
                        _ptr=_ptr->next;
                    }
                }
            //}else{
                /*_ptr=((struct protein*)(ptr->data))->compound->head;
                while(_ptr!=NULL){
                    if((rand()%100)>=50)protein_compound_push(protein,((struct codn *)(codn_ptr->data))->compound);
                    else bio_env_compound_push(((struct codn *)(codn_ptr->data))->compound);
                    codn_ptr=codn_ptr->next;
                    _ptr=_ptr->next;
                }*/
            //}
        }else{
            _ptr=((struct protein*)(ptr->data))->compound->head;
            while(_ptr!=NULL){
                protein_compound_push(protein,((struct codn *)(codn_ptr->data))->compound);
                codn_ptr=codn_ptr->next;
                _ptr=_ptr->next;
            }
        }
        cell_protein_push(newcell,protein);
        ptr=ptr->next;
    }
    DNA_setbone(newcell->DNA,newcell->protein);
    int codn_n=0;
    ptr=cell->DNA->codn->head;
    while(ptr!=NULL){
        codn_n++;
        ((struct codn *)(ptr->data))->compound=NULL;
        ptr=ptr->next;
    }
    newcell->lifespan=codn_n*40;
   // newcell->fitness=99;
    //cell_print(newcell);
    bio_env_cell_push(newcell,cell);
    cell_free(newcell);
    //free(newcell->fname);
}

void death(struct cell *cell){
    node *ptr=cell->protein->head;
    node *_ptr;
    while(ptr!=NULL){
        _ptr=((struct protein *)(ptr->data))->compound->head;
        while(_ptr!=NULL){
            bio_env_compound_push(((struct compound *)(_ptr->data)));
            //compound_free(((struct compound *)(_ptr->data)));
            _ptr=_ptr->next;
        }
        ptr=ptr->next;
    }
    ptr=cell->DNA->codn->head;
    while(ptr!=NULL){
        if(((struct codn *)(ptr->data))->compound!=NULL)bio_env_compound_push(((struct codn *)(ptr->data))->compound);
        //free(ptr->data);
        ptr=ptr->next;
    }
    bio_env_cell_remove(cell);
    //free(cell->protein);
    //free(cell);
}

void cell_run(char* path,struct MO *mo){
    struct cell *cell=cell_fileload_path(path);
    metabolism_and_motion(cell);
    if(cell->fitness == 200){
        death(cell);
        return;
    }
    if(grow(cell,mo)==1)proliferation(cell);
    cell->remaining_life--;
    if(cell->remaining_life==0){
        death(cell);
    }
    else{
        bio_env_cell_push(cell,NULL);
    }
}

struct cell *cell_fileload_path(char* path){
    json_t* cell_json;
    json_error_t json_error;
    cell_json = json_load_file(path, 0, &json_error);
    if (!cell_json) {
        fprintf(stderr, "%s:%s",path, json_error.text);
        exit(-1);
    }
    struct cell *cell=malloc(sizeof(struct cell));
    json_t* lifespan=json_object_get(cell_json,"lifespan");
    cell_init(cell,json_integer_value(lifespan));
    json_t* remaining_life=json_object_get(cell_json,"remaining_life");
    cell->remaining_life=json_integer_value(remaining_life)>=0?json_integer_value(remaining_life):json_integer_value(lifespan);
    json_decref(remaining_life);
    json_t* fitness=json_object_get(cell_json,"fitness");
    cell->fitness=json_integer_value(fitness);
    json_decref(fitness);
    json_t* protein=json_object_get(cell_json,"protein");
    char* json_value;
    json_t* item;
    json_t* item2;
    json_t* item3;
    char* tmp;
        for (size_t i = 0; ; i++) {
            item = json_array_get(protein, i);
            if (!item) {
                break;
            }
            struct protein *prot=malloc(sizeof(struct protein));
            protein_init(prot);
            for (size_t j = 0; ; j++) {
                item2 = json_array_get(item, j);
                if (!item2) {
                    break;
                }
                struct compound *comp=malloc(sizeof(struct compound));
                compound_init(comp);
                for (size_t k = 0; ; k++) {
                    item3 = json_array_get(item2, k);
                    if (!item3) {
                        break;
                    }
                    json_value = json_string_value(item3);
                    if (json_value) {
                        compound_code_push(comp,strtol(json_value, &tmp, 16));
                    }
                }
                protein_compound_push(prot,comp);
        }
        cell_protein_push(cell,prot);
    }
    json_t* DNA=json_object_get(cell_json,"DNA");
    if(DNA){
        node *ptr = cell->DNA->codn->head;
        for (size_t i = 0;ptr!=NULL; i++) {
            item = json_array_get(DNA, i);
            if (!item) {
                break;
            }
            if(json_integer_value(json_object_get(item,"comp"))==1){
                struct compound *comp=malloc(sizeof(struct compound));
                compound_init(comp);
                for (size_t j = 0;((struct codn*)(ptr->data))->data[j]!=-1 ; j++) {
                    compound_code_push(comp,((struct codn*)(ptr->data))->data[j]);
                }
                codn_setcomp(((struct codn*)(ptr->data)),comp);
                //puts("bba");
            }
            //DNA_codn_push(cell->DNA,codn);
            ptr=ptr->next;
        }
    }
    DNA_setbone(cell->DNA,cell->protein);
    json_decref(protein);
    json_decref(item);
    json_decref(item2);
    json_decref(item3);
    json_decref(DNA);
    json_decref(cell_json);
    strcpy(cell->fname,path);
    return cell;
}

struct cell *cell_fileload(char* name){
    char path[strlen(name)+strlen(CELL_dir)];
    sprintf(path,"%s%s",CELL_dir,name);
    struct cell *cell=cell_fileload_path(path);
    strcpy(cell->fname,name);
    return cell;
}