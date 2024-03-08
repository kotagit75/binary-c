#include "main.h"

void tissue_init(struct tissue *tis){
    tis->cells=malloc(sizeof(list));
    list_init(tis->cells);
}

#define tissue_cell_push(tis,cell) list_insertion(tis->cells,cell);

void tissue_fileload(struct tissue *tis,char *path){
    strcpy(tis->path,path);
    DIR *dir;
    char cellpath[200];
    struct dirent *dp;
    dir = opendir(path);
    node *ptr = tis->cells->head;
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
        if (dp->d_name[0] != '.') {
            sprintf(cellpath,"%s/%s",path,dp->d_name);
            tissue_cell_push(tis,cell_fileload_path(cellpath));
        }
    }
    closedir(dir);
    /*ptr = tis->cells->head;
    while (ptr!=NULL){
        cell_print(((struct cell*)(ptr->data)));
        ptr=ptr->next;
    }*/
    
}

struct cell *tissue_meleget(struct tissue *tis){
    node *ptr = tis->cells->head;
    struct cell *mele=NULL;
    struct cell *tmp=NULL;
    int melefit=0;
    while(ptr!=NULL){
        tmp = ((struct cell*)(ptr->data));
        if(melefit < tmp->fitness){
            mele=tmp;
            melefit=mele->fitness;
        }
        ptr=ptr->next;
    }
    //printf(":%s",mele->fname);
    return mele;
}

void tissue_grow(struct tissue *tis,struct MO *mo){
    struct cell *cell=tissue_meleget(tis);
    if(cell!=NULL){
        cell_run(cell->fname,mo);
    }
}

int tissue_cellnum(struct tissue *tis){
    node *ptr = tis->cells->head;
    int n=0;
    while(ptr!=NULL){
        n++;
        ptr=ptr->next;
    }
    return n;
}

void tissue_meiosis(struct tissue *tis,char *bemv2path,char *mv2path){
    char path[100];
    int tmp;
    char tmpstr[100];
    node *ptr=tis->cells->head;
    while (ptr!=NULL){
        if(((struct cell*)(ptr->data))==tissue_meleget(tis)){
            ptr=ptr->next;continue;
        }
        sscanf(((struct cell*)(ptr->data))->fname,"" CELL_dir ""   "%d%s",&tmp,&tmpstr);
        sprintf(path,"%s%s\n",mv2path,tmpstr);
        rename(((struct cell*)(ptr->data))->fname,path);
        break;
    }
   
}