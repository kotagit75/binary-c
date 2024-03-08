#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "../lib/main.h"

typedef struct{
    int sh_offset;
    int sh_size;
}textsection;

textsection get_text_section(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        perror("Error getting file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    void *file_data = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Error mapping file to memory");
        close(fd);
        exit(EXIT_FAILURE);
    }

    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)file_data;
    Elf64_Shdr *section_headers = (Elf64_Shdr *)((char *)file_data + elf_header->e_shoff);

    textsection text={0,0};
    for (int i = 0; i < elf_header->e_shnum; ++i) {
        if (section_headers[i].sh_type == SHT_PROGBITS &&
            section_headers[i].sh_flags & SHF_EXECINSTR) {
            printf("Found .text section at offset 0x%lx, size 0x%lx\n",
                   section_headers[i].sh_offset, section_headers[i].sh_size);
            text.sh_offset=section_headers[i].sh_offset;
            text.sh_size=section_headers[i].sh_size;
            break;
        }
    }

    munmap(file_data, file_stat.st_size);
    close(fd);
    return text;
}

int main(int argc, char *argv[]) {
    char tmp_f[30];
    sprintf(tmp_f,"%s%s",tmp_dir,"tmp");
    char tmp_f2[30];
    sprintf(tmp_f2,"%s%s",tmp_dir,"as2cell_tmpo.o");
    char tmp_f3[30];
    sprintf(tmp_f3,"%s%s",tmp_dir,"as2cell_tmp");
    if(strcmp(argv[1],"-e")==0){
        printf("env push\n");
        if (argc != 4) {
            fprintf(stderr, "Usage: %s <executable>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        struct cell *cell=malloc(sizeof(struct cell));
        int lifespan=0;
        sscanf(argv[3], "%d", &lifespan);
        cell_init(cell,lifespan);
        FILE *file;
        char buffer[100];
        // ファイルを開く
        file = fopen(argv[2], "r");
        if (file == NULL) {
            printf("ファイルを開けませんでした。\n");
            return 1;
        }
        FILE *tmp;
        puts("a");
        // ファイルから1行ずつ読み込む
        struct protein *prot=malloc(sizeof(struct protein));
        protein_init(prot);
        int prot_refresh_flag=0;
        while (fgets(buffer, sizeof(buffer), file) != NULL){
            tmp = fopen(tmp_f, "w");
            if (file == NULL) {
                printf("ファイルを開けませんでした。\n");
                return 1;
            }
            fprintf(tmp,".text\n.global _start\n_start:\n%s", buffer);
            fclose(tmp);
            char cmd[50];
            sprintf(cmd,"as -o %s %s",tmp_f2,tmp_f);
            system(cmd);
            sprintf(cmd,"ld -o %s %s",tmp_f3,tmp_f2);
            system(cmd);
            textsection text = get_text_section(tmp_f3);

            FILE * fp = NULL;
            fp=fopen(tmp_f3, "rb");
            if (fp == NULL) {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }
            if(text.sh_size!=0){
                struct compound *comp=malloc(sizeof(struct compound));
                compound_init(comp);
                unsigned char code[text.sh_size];
                fseek(fp, text.sh_offset, SEEK_SET);
                fread(code, sizeof(unsigned char), text.sh_size, fp);
                fclose(fp);
                for(int i=0;i<text.sh_size;i++){
                    compound_code_push(comp,code[i]);
                }
                protein_compound_push(prot,comp);
                prot_refresh_flag=0;
            }else{
                prot_refresh_flag++;
                if(prot_refresh_flag==4){
                    cell_protein_push(cell,prot);
                    prot=malloc(sizeof(struct protein));
                    protein_init(prot);
                    prot_refresh_flag=0;
                }
            }
        }
        // ファイルを閉じる
        fclose(file);
        cell_protein_push(cell,prot);

        DNA_setbone(cell->DNA,cell->protein);
        puts("a");
        bio_env_cell_push(cell,NULL);
    }else{
        if (argc != 4) {
            fprintf(stderr, "Usage: %s <executable>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        struct cell *cell=malloc(sizeof(struct cell));
        int lifespan=0;
        sscanf(argv[2], "%d", &lifespan);
        cell_init(cell,lifespan);
        FILE *file;
        char buffer[100];
        // ファイルを開く
        file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("ファイルを開けませんでした。\n");
            return 1;
        }
        FILE *tmp;
        // ファイルから1行ずつ読み込む
        struct protein *prot=malloc(sizeof(struct protein));
        protein_init(prot);
        int prot_refresh_flag=0;
        while (fgets(buffer, sizeof(buffer), file) != NULL){
            tmp = fopen(tmp_f, "w");
            if (file == NULL) {
                printf("ファイルを開けませんでした。\n");
                return 1;
            }
            fprintf(tmp,".text\n.global _start\n_start:\n%s", buffer);
            fclose(tmp);
            char cmd[50];
            sprintf(cmd,"as -o %s %s",tmp_f2,tmp_f);
            system(cmd);
            sprintf(cmd,"ld -o %s %s",tmp_f3,tmp_f2);
            system(cmd);
            textsection text = get_text_section(tmp_f3);

            FILE * fp = NULL;
            fp=fopen(tmp_f3, "rb");
            if (fp == NULL) {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }
            if(text.sh_size!=0){
                struct compound *comp=malloc(sizeof(struct compound));
                compound_init(comp);
                unsigned char code[text.sh_size];
                fseek(fp, text.sh_offset, SEEK_SET);
                fread(code, sizeof(unsigned char), text.sh_size, fp);
                fclose(fp);
                for(int i=0;i<text.sh_size;i++){
                    compound_code_push(comp,code[i]);
                }
                protein_compound_push(prot,comp);
                prot_refresh_flag=0;
            }else{
                prot_refresh_flag++;
                if(prot_refresh_flag==4){
                    cell_protein_push(cell,prot);
                    prot=malloc(sizeof(struct protein));
                    protein_init(prot);
                    prot_refresh_flag=0;
                }
            }
        }
        // ファイルを閉じる
        fclose(file);
        cell_protein_push(cell,prot);

        DNA_setbone(cell->DNA,cell->protein);

        cell_file_push(cell,argv[3]);
    }

    printf("\n");
    return 0;
}
