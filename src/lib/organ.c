#include "main.h"

void organ_init(struct organ *org){
    org->tissue=malloc(sizeof(list));
    list_init(org->tissue);
}

#define organ_tissue_push(org,tis) list_insertion(org->tissue,tis);

void organ_fileload(struct organ *org,char *path){
    sprintf(org->path,path);
    DIR *dir;
    char tispath[200];
    struct dirent *dp;
    dir = opendir(path);
    node *ptr = org->tissue->head;
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
        if (dp->d_name[0] != '.') {
            struct tissue *n_tis = malloc(sizeof(struct tissue));
            tissue_init(n_tis);
            organ_tissue_push(org,n_tis);
        }
    }
    closedir(dir);
    dir = opendir(path);
    ptr = org->tissue->head;
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
        if (dp->d_name[0] != '.') {
            sprintf(tispath,"%s/%s",path,dp->d_name);
            if(ptr==NULL)break;
            tissue_fileload(((struct tissue*)(ptr->data)),tispath);
            ptr=ptr->next;
        }
    }
    closedir(dir);
}

int organ_metabolism_and_motion(struct organ *org,int evalscript_num){
    char cmd[50];
    sprintf(cmd,"./eval%d.sh %s",evalscript_num,org->path);
    int ret = system(cmd);
    org->fitness=WEXITSTATUS(ret);
    node *ptr=org->tissue->head;
    while(ptr!=NULL){
        struct cell *cell=tissue_meleget(((struct tissue*)(ptr->data)));
        if(cell!=NULL)cell->fitness=org->fitness;
        ptr=ptr->next;
    }
    return org->fitness;
}

void organ_grow(struct organ *org,struct MO *mo){
    node *ptr=org->tissue->head;
    while(ptr!=NULL){
        tissue_grow(((struct tissue*)(ptr->data)),mo);
        ptr=ptr->next;
    }
}

int organ_isdeath(struct organ *org){
    node *ptr=org->tissue->head;
    while(ptr!=NULL){
        if(tissue_cellnum(((struct tissue*)(ptr->data)))!=0){
            return 0;
        }
        ptr=ptr->next;
    }
    return 1;
}

int organ_canmeiosis(struct organ *org){
    node *ptr=org->tissue->head;
    while(ptr!=NULL){
        if(tissue_cellnum(((struct tissue*)(ptr->data)))<2){
            return 0;
        }
        ptr=ptr->next;
    }
    return 1;
}

void organ_meiosis(struct organ *org,char *bemv2path,char *mv2path){
    node *ptr=org->tissue->head;
    while(ptr!=NULL){
        tissue_meiosis(((struct tissue*)(ptr->data)),bemv2path,mv2path);
        ptr=ptr->next;
    }
}