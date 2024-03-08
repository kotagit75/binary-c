#include "main.h"

#if defined(_use_conf)
    extern struct _config config;
#endif

void bio_env_init1(struct bio_env *be)
{
    be->cycle = 0;
    be->put = 0;
}

void bio_env_init2(struct bio_env *be)
{
    if(be->put){
        printf("put.sh --> env(cell/comp)");
        char put_cmd[20];
        sprintf(put_cmd,"%s%s",tool_dir,"put.sh");
        if(WEXITSTATUS(system(put_cmd))==0)printf("\rput.sh --> env(cell/comp):complated\n");
        else printf("\rput.sh --> env(cell/comp):faild!\n");
    }
    printf("Opening to running file(Path:%s):",RUNNING);
    FILE *file;
    file = fopen(RUNNING, "w");
    if (file == NULL) {
        printf("faild!\n");
    }else{
        printf("complated\n");
        fprintf(file, "HELLOWORLD:こんにちは世界\n");
        fclose(file);
    }
}

void bio_env_setting_load(struct bio_env *be,char *setting){
    json_t* setting_json;
    json_error_t json_error;
    setting_json = json_load_file(setting, 0, &json_error);
    if (!setting_json) {
        fprintf(stderr, "%s", json_error.text);
        exit(-1);
    }
    json_t* cyclelen=json_object_get(setting_json,"cyclelen");
    if(cyclelen!=NULL){
        be->Cycle_len=json_integer_value(cyclelen);
        json_decref(cyclelen);
    }
    json_t* put=json_object_get(setting_json,"put");
    if(put!=NULL){
        be->put=json_is_true(put);
        json_decref(cyclelen);
    }
    json_decref(setting_json);
}

void bio_env_cell_push(struct cell *cell,struct cell *parent){
    if(strcmp(cell->fname,"unname")==0){
        char dir[1024]={'\0'} ;
        char* dirEnd;
        dirEnd = strrchr(parent->fname,'/');//winだったら'\\'にする
        if(dirEnd == NULL){
            printf("fileName only\n");
            return 0;
        }
        for(int i=0; i<(int)strlen(parent->fname); i++){
            dir[i] = parent->fname[i];
            if(parent->fname+i == dirEnd){
                break;
            }
        }
        char name[30];// ./cell/1/sad/1.json
        sprintf(name,"%d.json",rand()%10000);
        char path[30];
        sprintf(path,"%s%s",dir,name);
        while(exist_file(path)){
            sprintf(name,"%d.json",rand()%10000);
            sprintf(path,"%s%s",dir,name);
        }
        strcpy(cell->fname,path);
    }
    cell_file_push(cell,cell->fname);
}

void bio_env_cell_remove(struct cell *cell){
  if(cell->fname==NULL){printf("細胞はファイルに保存されていません。\n");exit(1);}
  printf("removepath:%s\n",cell->fname);
  if(remove(cell->fname)){
    printf("細胞ファイルの削除に失敗しました。\n");
    exit(1);
  }
}

void bio_env_compound_push(struct compound *comp){
    if(comp!=NULL)return;
    json_t *json_root = json_object();
    json_t *json_elem= json_array();
    if (json_elem){
        if(comp->len>0)for(size_t i = 0;i<comp->len; i++){
            char res[10];
            sprintf(res, "0x%x", comp->elem.elements[i]);
            json_array_append(json_elem,json_string(res));
        }
        json_object_set(json_root, "elem", json_elem);
        json_decref(json_elem);
    }
    char* json=json_dumps(json_root, json_object_size(json_root));
    json_decref(json_root);
    char name[30];
    if(comp->fname==NULL){
        sprintf(name,"%d.json",rand()%10000);
        char path[30];
        sprintf(path,"%s%s",COMPOUND_dir,name);
        while(exist_file(path)){
            sprintf(name,"%d.json",rand()%10000);
            sprintf(path,"%s%s",COMPOUND_dir,name);
        }
    }else{
        strcpy(name,comp->fname);
    }
    char path[strlen(COMPOUND_dir)+strlen(name)];
    sprintf(path,"%s%s",COMPOUND_dir,name);
    FILE* fp;
    fp = fopen(path, "w");
    fprintf(fp, "%s", json);
    fclose(fp);
}

void bio_env_compound_pop(struct compound *comp){
    if(comp->fname==NULL){printf("化合物はファイルに保存されていません。\n");return;}
    char path[strlen(COMPOUND_dir)+strlen(comp->fname)];
    sprintf(path,"%s%s",COMPOUND_dir,comp->fname);
    if(!remove(path) == 0 ){
        printf("化合物ファイルの削除に失敗しました。\n");
    }
}

struct compound *bio_env_compound_pop_path(char* name){
    struct compound *comp=compound_fileload(name);
    if(comp->fname==NULL){printf("化合物はファイルに保存されていません。\n");return;}
    char path[strlen(COMPOUND_dir)+strlen(comp->fname)];
    sprintf(path,"%s%s",COMPOUND_dir,comp->fname);
    if(!remove(path) == 0 ){
        printf("化合物ファイルの削除に失敗しました。\n");
    }
    return comp;
}

struct compound *bio_env_compound_pop_random(){
    DIR *dir;
    int n=0;
    struct dirent *dp;
    dir = opendir(COMPOUND_dir);
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
        if(dp->d_name[0]!=46)n++;
    }
    closedir(dir);
    if(n==0)return NULL;
    int index=rand() % n + 1;
    n=1;
    struct compound* comp=NULL;
    DIR *_dir;
    struct dirent *_dp;
    _dir = opendir(COMPOUND_dir);
    for (_dp = readdir(_dir); _dp != NULL; _dp = readdir(_dir)) {
        if (_dp->d_name[0] != '.' && index==n) {
            comp = bio_env_compound_pop_path(_dp->d_name);
            break;
        }
        if(_dp->d_name[0]!='.')n++;
    }
    closedir(_dir);
    return comp;
}

int bio_env_cell_cycle(struct bio_env *be)
{
    be->cycle++;
    DIR *dir;
    struct dirent *dp;
    dir = opendir(CELL_dir);
    int i=0;
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
        if (dp->d_name[0] != '.') {
            i++;
            MO_run(dp->d_name);
        }
    }

    closedir(dir);
    if(be->Cycle_len==be->cycle){
        return 0;
    }
    #if defined(_use_conf)
        return (config.cell_zero_finish==1?i:1);
    #endif
}

void bio_env_print(struct bio_env *be)
{
    printf("### %dcycle ###\n", be->cycle);
    printf("Cells:\n");
    DIR *dir;
    struct dirent *dp;
    dir = opendir(CELL_dir);
    for (dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
        if (dp->d_name[0] != '.') {
            struct cell *cell=cell_fileload(dp->d_name);
            printf(":   ");
            cell_print(cell);
            cell_free(cell);
        }
    }
    closedir(dir);
    printf("\ncompound:\n");
    DIR *_dir;
    struct dirent *_dp;
    _dir = opendir(COMPOUND_dir);
    for (_dp = readdir(_dir); _dp != NULL; _dp = readdir(_dir)) {
        if (_dp->d_name[0] != '.') {
            struct compound *comp=compound_fileload(_dp->d_name);
            compound_print(comp);
            compound_free(comp);
        }
    }
    closedir(_dir);
    printf("\n\n");
}

void bio_env_free(struct bio_env *be)
{
    free(be);
}

void bio_env_finish(struct MO *mo){
    char outpath[]="./out";
    char cmd[100];
    sprintf(cmd,"cp -r %s %s",mo->path,outpath);

    #if defined(_use_conf)
        printf("%c",(config.successed_bell?'\a':'\0'));
        printf("%c",(config.nomal_bell?'\a':'\0'));
    #endif
    printf("success!多細胞生物(%s)が目的のバイナリを生成しました。「%s」へ移動させます。\n",mo->path,outpath);

    int exit_code=0;
    if(WEXITSTATUS(system(cmd))==0){
        //cell_free(cell);
        printf("多細胞生物ディレクトリの移動に成功しました。%s--->%s\n",mo->path,outpath);
    }else{
        printf("多細胞生物ディレクトリの移動に失敗しました。%s-x->%s\n",mo->path,outpath);
        //cell_free(cell);
        exit_code=1;
    }
    exit(exit_code);
}

int bio_env_is_running(){
    return exist_file(RUNNING);
}