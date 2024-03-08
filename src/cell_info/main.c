#include "../lib/main.h"

int main(int argc, char *argv[]) {
    if(argc!=2){
        printf("invalid argument\n");
        return EXIT_FAILURE;
    }
    struct cell *cell=cell_fileload_path(argv[1]);
    if(!cell){
        printf("Could not load cell file\n");
        return EXIT_FAILURE;
    }
    cell_print(cell);
    return EXIT_SUCCESS;
}
