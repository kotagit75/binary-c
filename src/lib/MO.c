#include "main.h"

void MO_init(struct MO *mo){
    mo->organ=malloc(sizeof(list));
    list_init(mo->organ);
    mo->comp=malloc(sizeof(list));
    list_init(mo->comp);
}

#define MO_organ_push(mo,org) list_insertion(mo->organ,org)

void MO_fileload(struct MO *mo,char *path){
    strcpy(mo->path,path);
    DIR *dir;
    char orgpath[200];
    struct dirent *dp;
    dir = opendir(path);
    node *ptr = mo->organ->head;
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
        if (dp->d_name[0] != '.') {
            struct organ *n_org = malloc(sizeof(struct organ));
            organ_init(n_org);
            MO_organ_push(mo,n_org);
        }
    }
    closedir(dir);
    dir = opendir(path);
    ptr = mo->organ->head;
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir)){
        if (dp->d_name[0] != '.') {
            sprintf(orgpath,"%s/%s",path,dp->d_name);
            if(ptr==NULL)break;
            organ_fileload(((struct organ*)(ptr->data)),orgpath);
            ptr=ptr->next;
        }
    }
    closedir(dir);
}

void MO_metabolism_and_motion(struct MO *mo){
    node *ptr=mo->organ->head;
    int evalscript_num = 1;
    int fitness_total=0;
    while (ptr!=NULL){
        fitness_total+=organ_metabolism_and_motion(((struct organ*)(ptr->data)),evalscript_num);
        evalscript_num++;
        ptr=ptr->next;
    }
    mo->fitness=fitness_total/(evalscript_num-1);
}
void MO_grow(struct MO *mo){
    node *ptr=mo->organ->head;
    while(ptr!=NULL){
        organ_grow(((struct organ*)(ptr->data)),mo);
        ptr=ptr->next;
    }
}

void MO_death(struct MO *mo){
    char cmd[100];
    sprintf(cmd,"rm -r %s",mo->path);
    if(WEXITSTATUS(system(cmd))){
        printf("多細胞ファイルの分解に失敗しました。\n");
        exit(1);
    }
    else printf("多細胞ファイルの分解に成功しました。\n");
}

void MO_meiosis(struct MO *mo){
    char name[100];
    sprintf(name,"%d",rand()%1000);
    char path[30];
    sprintf(path,"%s%s",CELL_dir,name);
    while(exist_dir(path)){
        sprintf(name,"%d",rand()%1000);
        sprintf(path,"%s%s",CELL_dir,name);
        printf("%s\n",path);
    }
    char cmd[100];
    sprintf(cmd,"binc_dironly_cp %s %s",mo->path,path);
    if(!WEXITSTATUS(system(cmd))){
        node *ptr=mo->organ->head;
        while (ptr!=NULL){
            organ_meiosis(((struct organ*)(ptr->data)),mo->path,path);
            ptr=ptr->next;
        }
    }
}

void MO_run(char *name){
    char path[100];
    sprintf(path,"%s%s",CELL_dir,name);
    struct MO *mo=malloc(sizeof(struct MO));
    MO_init(mo);
    MO_fileload(mo,path);
    MO_metabolism_and_motion(mo);
    if(mo->fitness==100)bio_env_finish(mo);
    node *ptr=mo->organ->head;
    while (ptr!=NULL){
        if(((struct organ*)(ptr->data))->fitness!=100){
            goto next;
        }
        ptr=ptr->next;
    }
    next:
    for (int i = 0; i < 20; i++){
        struct compound *comp=(rand() / (RAND_MAX / 101 + 1) <= mo->fitness)?bio_env_compound_pop_random():NULL;
        if(comp!=NULL)list_insertion(mo->comp,comp);
    }
    MO_grow(mo);
    ptr=mo->comp->head;
    while (ptr!=NULL){
        bio_env_compound_push(((struct compound*)(ptr->data)));
        ptr=ptr->next;
    }
    
    free(mo);
    struct MO *mo2=malloc(sizeof(struct MO));
    MO_init(mo2);
    MO_fileload(mo2,path);
    ptr=mo2->organ->head;
    int is_death=1;
    int can_meiosis=1;
    int cell_n;
    while (ptr!=NULL){
        if(organ_isdeath(((struct organ*)(ptr->data)))!=1)is_death=0;
        if(organ_canmeiosis(((struct organ*)(ptr->data)))!=1)can_meiosis=0;
        ptr=ptr->next;
    }
    if(is_death){
        MO_death(mo2);
    }
    if(can_meiosis){
        MO_meiosis(mo2);
    }
}