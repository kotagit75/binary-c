#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/stat.h>
#include <elf.h>
#include <assert.h>
#include <getopt.h>
#include <dirent.h>
#include <jansson.h>

#define CELL_dir "./cell/"
#define COMPOUND_dir "./compound/"
#define LOGtree_file "./log_tree.md"
#define RUNNING "./running"
#define tool_dir "./tool/"
#define tmp_dir "./tmp/"
#define system_dir "./sys/"
#define model_dir "./model/"
#define binc_dir "./.binc/"
#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x ## y

// list.c
typedef struct Node{
  void *data;
  struct Node *next;
}node;
typedef struct _list{
    node *head;
}list;
void list_init(list *list);
void list_insertion(list *list,void *num);
void list_deletion(list *list,void *num);

typedef unsigned long long bio_data_t;
struct bio_env;

// element.c
typedef unsigned char element_t;

// compound.c
#define MAX_compound_element 8
enum compound_TYPE{
    compound_TYPE_NONE,
    compound_TYPE_CODE,
    compound_TYPE_DATA,
};
struct compound{
    union{
        element_t elements[MAX_compound_element];
        bio_data_t data;
    }elem;
    int len;
    size_t size;
    char* fname;
    int free;
};
void compound_init(struct compound *comp);
void compound_print(struct compound *comp);
void compound_print_data(struct compound *comp);
void compound_code_push(struct compound *comp,element_t element);
void compound_data_push(struct compound *comp,bio_data_t data);
int compound_type(struct compound *comp);
void compound_bond(struct compound *comp,struct bio_env *be);
void compound_free(struct compound *comp);
struct compound *compound_fileload(char* name);

// protein.c
struct protein{
    list *compound;
};
void protein_init(struct protein *protein);
void protein_compound_push(struct protein *protein,struct compound *comp);
void protein_bond(struct protein *protein,struct bio_env *be);
void protein_print(struct protein *protein);
void protein_free(struct protein *protein);

// codn.c
struct codn{
    struct compound *compound;
    int data[MAX_compound_element];
};
void codn_init(struct codn *codn);
void codn_print(struct codn *codn);
void codn_print_compound(struct codn *codn);
void codn_free(struct codn *codn);
int codn_setcomp(struct codn *codn,struct compound *comp);

// DNA.c
struct DNA{
    list *codn;
    list *protein;
};
void DNA_init(struct DNA *DNA);
void DNA_codn_push(struct DNA *DNA,struct codn *codn);
void DNA_print(struct DNA *DNA);
void DNA_setbone(struct DNA *DNA,list *list);
void DNA_free(struct DNA *DNA);

// cell.c
struct cell{
    list *protein;
    struct DNA *DNA;
    int lifespan;
    int remaining_life;
    int fitness;
    char fname[50];
};
void cell_init(struct cell *cell,int lifespan);
void cell_protein_push(struct cell *cell,struct protein *protein);
void cell_print(struct cell *cell);
//void cell_run(char* path,struct MO *mo);
struct cell *cell_fileload_path(char* path);
struct cell *cell_fileload(char* name);
void cell_file_push(struct cell *cell,const char *path);
void cell_free(struct cell *cell);

//tissue.c
struct tissue{
    list *cells;
    char path[50];
};
void tissue_init(struct tissue *tis);
void tissue_fileload(struct tissue *tis,char *path);
struct cell *tissue_meleget(struct tissue *tis);
//void tissue_grow(struct tissue *tis,struct MO *mo);
int tissue_cellnum(struct tissue *tis);
void tissue_meiosis(struct tissue *tis,char *bemv2path,char *mv2path);

//oragan.c
struct organ{
    list *tissue;
    int fitness;
    char path[50];
};
void organ_init(struct organ *org);
void organ_fileload(struct organ *org,char *path);
int organ_metabolism_and_motion(struct organ *org,int evalscript_num);
//void organ_grow(struct organ *org,struct MO *mo);
int organ_isdeath(struct organ *org);
int organ_canmeiosis(struct organ *org);
void organ_meiosis(struct organ *org,char *bemv2path,char *mv2path);

//MO.c
struct MO{
    list *organ;
    list *comp;
    int fitness;
    char path[50];
};
void MO_init(struct MO *MO);
void MO_fileload(struct MO *mo,char *path);
void MO_run(char *path);


// env.c
struct bio_env{
    int cycle;
    int put;
    int codedata_bond[100];
    int codedata_bond_len;
    int Cycle_len;
    int Cycle_sleeptime;
};
void bio_env_init(struct bio_env *be);
void bio_env_setting_load(struct bio_env *be,char *setting);
void bio_env_cell_push(struct cell *cell,struct cell *parent);
void bio_env_cell_remove(struct cell *cell);
void bio_env_compound_push(struct compound *comp);
void bio_env_compound_pop(struct compound *comp);
struct compound *bio_env_compound_pop_path(char* name);
struct compound *bio_env_compound_pop_random();
int bio_env_cell_cycle(struct bio_env *be);
void bio_env_print(struct bio_env *be);
void bio_env_free(struct bio_env *be);
void bio_env_finish(struct MO *mo);
int bio_env_is_running();

// tool.c
int exist_file(const char* path);
int get_digit(int n);
char *GetFileName( const char *file_path);
void str_replace(const char *src, const char *target, const char *replace, char **result);