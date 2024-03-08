#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <fcntl.h>
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
    if(strcmp(argv[1],"-e")==0){
        if (argc != 4) {
            fprintf(stderr, "Usage: %s <executable>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        textsection text = get_text_section(argv[2]);

        FILE * fp = NULL;
        fp=fopen(argv[2], "rb");
        if (fp == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        unsigned char code[text.sh_size];
        fseek(fp, text.sh_offset, SEEK_SET);
        fread(code, sizeof(unsigned char), text.sh_size, fp);
        fclose(fp);

        struct cell *cell=malloc(sizeof(struct cell));
        int lifespan=0;
        sscanf(argv[3], "%d", &lifespan);
        cell_init(cell,lifespan);
        struct protein *prot=malloc(sizeof(struct protein));
        protein_init(prot);
        for(int i=0;i<text.sh_size;i++){
            struct compound *comp=malloc(sizeof(struct compound));
            compound_init(comp);
            compound_code_push(comp,code[i]);
            protein_compound_push(prot,comp);
        }
        cell_protein_push(cell,prot);

        DNA_setbone(cell->DNA,cell->protein);

        bio_env_cell_push(cell,NULL);
    }else{

        if (argc != 4) {
            fprintf(stderr, "Usage: %s <executable>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        textsection text = get_text_section(argv[1]);

        FILE * fp = NULL;
        fp=fopen(argv[1], "rb");
        if (fp == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        unsigned char code[text.sh_size];
        fseek(fp, text.sh_offset, SEEK_SET);
        fread(code, sizeof(unsigned char), text.sh_size, fp);
        fclose(fp);

        struct cell *cell=malloc(sizeof(struct cell));
        int lifespan=0;
        sscanf(argv[2], "%d", &lifespan);
        cell_init(cell,lifespan);
        struct protein *prot=malloc(sizeof(struct protein));
        protein_init(prot);
        for(int i=0;i<text.sh_size;i++){
            struct compound *comp=malloc(sizeof(struct compound));
            compound_init(comp);
            compound_code_push(comp,code[i]);
            protein_compound_push(prot,comp);
        }
        cell_protein_push(cell,prot);

        DNA_setbone(cell->DNA,cell->protein);

        cell_file_push(cell,argv[3]);
    }

    printf("\n");
    return 0;
}
