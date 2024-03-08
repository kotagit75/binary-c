#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <dirent.h>
#include <assert.h>
#include "../lib/main.h"
#include <unistd.h>
#include <jansson.h>

#define PHNUM	1
#define LOAD_VADDR	0x400000

static char load_data_char_list[] =
	"\n !\"#$%&'()*+,-./0123456789:;<=>?@"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"[\\]^_`"
	"abcdefghijklmnopqrstuvwxyz"
	"{|}~";
static unsigned char load_data_char_list_len;

static void usage(char *prog_name)
{
	fprintf(stderr, "Usage: %s CELL_FILE_NAME ELF_FILE_NAME\n", prog_name);
	exit(EXIT_FAILURE);
}

static size_t fwrite_safe(void *ptr, size_t size, FILE *stream)
{
	size_t write_bytes = 0;
	unsigned char *p = ptr;
	while (write_bytes < size) {
		size_t n = fwrite(p, 1, size - write_bytes, stream);
		assert(ferror(stream) == 0);
		p += n;
		write_bytes += n;
		if (feof(stream) != 0)
			break;
	}
	return write_bytes;
}

static void write_elf64_ehdr(FILE *fp)
{
	Elf64_Ehdr ehdr;
	memcpy(ehdr.e_ident, ELFMAG, SELFMAG);
	ehdr.e_ident[EI_CLASS] = ELFCLASS64;
	ehdr.e_ident[EI_DATA] = ELFDATA2LSB;
	ehdr.e_ident[EI_VERSION] = EV_CURRENT;
	ehdr.e_ident[EI_OSABI] = ELFOSABI_SYSV;
	ehdr.e_ident[EI_ABIVERSION] = 0;
	unsigned char i;
	for (i = EI_PAD; i < EI_NIDENT; i++)
		ehdr.e_ident[i] = 0;
	ehdr.e_type = ET_EXEC;
	ehdr.e_machine = EM_X86_64;
	ehdr.e_version = EV_CURRENT;
	ehdr.e_phoff = sizeof(Elf64_Ehdr);
	ehdr.e_shoff = 0;
	ehdr.e_flags = 0;
	ehdr.e_ehsize = sizeof(Elf64_Ehdr);
	ehdr.e_phentsize = sizeof(Elf64_Phdr);
	ehdr.e_phnum = PHNUM;
	ehdr.e_shentsize = 0;
	ehdr.e_shnum = 0;
	ehdr.e_shstrndx = SHN_UNDEF;

	ehdr.e_entry =
		LOAD_VADDR + sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * PHNUM)
		+ load_data_char_list_len;

	size_t n = fwrite_safe(&ehdr, sizeof(Elf64_Ehdr), fp);
	if (n != sizeof(Elf64_Ehdr)) {
		perror("fwrite(ehdr)");
		exit(EXIT_FAILURE);
	}
}

static void write_elf64_phdr(FILE *fp, unsigned int load_data_size)
{
	Elf64_Phdr phdr;
	phdr.p_type = PT_LOAD;
	phdr.p_flags = PF_X | PF_R;
	phdr.p_offset = 0;
	phdr.p_vaddr = LOAD_VADDR;
	phdr.p_paddr = phdr.p_vaddr;
	phdr.p_filesz = sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * PHNUM)
		+ load_data_char_list_len + load_data_size;
	phdr.p_memsz = phdr.p_filesz;
	phdr.p_align = 0x200000;

	size_t n = fwrite_safe(&phdr, sizeof(Elf64_Phdr), fp);
	if (n != sizeof(Elf64_Phdr)) {
		perror("fwrite(phdr)");
		exit(EXIT_FAILURE);
	}
}

static void write_load_data(
	FILE *fp, void *load_data, unsigned int load_data_size)
{
	size_t n;

	n = fwrite_safe(load_data_char_list, load_data_char_list_len, fp);
	if (n != load_data_char_list_len) {
		perror("fwrite(char list)");
		exit(EXIT_FAILURE);
	}

	n = fwrite_safe(load_data, load_data_size, fp);
	if (n != load_data_size) {
		perror("fwrite(load data)");
		exit(EXIT_FAILURE);
	}
}

char buf[500]={0};int n=0;

int f2code(char* path){
	json_t* cell_json;
	json_error_t json_error;
	cell_json = json_load_file(path, 0, &json_error);
	if (!cell_json) {
		fprintf(stderr, "%s", json_error.text);
		exit(-1);
	}
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
			for (size_t j = 0; ; j++) {
				item2 = json_array_get(item, j);
				if (!item2) {
					break;
				}
				for (size_t k = 0; ; k++) {
					item3 = json_array_get(item2, k);
					if (!item3) {
						break;
					}
					json_value = json_string_value(item3);
					if (json_value) {
						buf[n]=strtol(json_value, &tmp, 16);
						//compound_code_push(comp,strtol(json_value, &tmp, 16));
						//protein_compound_push(prot,comp);
						n++;
					}
				}
		}
	}
	json_decref(protein);
	json_decref(item);
	json_decref(item2);
	json_decref(item3);
	json_decref(cell_json);
	return n;
}

void help(){
	puts("Usage: cell2elf [-h] | [cell_file elf_output_file]");
	puts("cellfile-(cell2elf)->elffile");
}

int main(int argc, char *argv[])
{
	int ret = -1;
    ret = getopt(argc, argv, "h");
    if (ret != -1){
        switch (ret) {
        case 'h':
			help();
			return 0;
		}
	}

	load_data_char_list_len = strlen(load_data_char_list);
	char* mo_fn=argv[1];
	char* eval_fn=argv[2];
	FILE *fp = fopen(eval_fn, "w+b");
	if (fp == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	int size;
	struct MO *mo=malloc(sizeof(struct MO));
	puts("a");
	MO_init(mo);
	puts("a");
	MO_fileload(mo,mo_fn);
	puts("a");
	struct cell *mele = NULL;
	node *ptr = mo->organ->head;
	node *_ptr;
	while (ptr!=NULL){
		_ptr = ((struct organ*)(ptr->data))->tissue->head;
		while (_ptr!=NULL){	
			mele = tissue_meleget(((struct tissue*)(_ptr->data)));
			if(mele!=NULL){
				size = f2code(mele->fname);
			}
			_ptr=_ptr->next;
		}
		ptr=ptr->next;
	}
	
	
	//return 0;
	write_elf64_ehdr(fp);
	write_elf64_phdr(fp, size);
	write_load_data(fp, buf, size);
  	fclose(fp);
  	chmod(eval_fn,0755);
}