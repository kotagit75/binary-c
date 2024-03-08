#include "../lib/main.h"

#if defined(_use_conf)
    struct _config config={0,0,0};
#endif

int main(int argc, char*argv[]){
    system("clear");
    srand(10000);
    struct bio_env *be;
    be = malloc(sizeof(struct bio_env));
    printf("Initializing the environment\n");
    bio_env_init1(be);
    char setting_path[100];
    sprintf(setting_path,"%s%s",binc_dir,"setting.json");
    bio_env_setting_load(be,setting_path);
    bio_env_init2(be);
    printf("Environment initialization has finished.\n");
    #if defined(_use_conf)
        printf("option\n");
        extern char	*optarg;
        extern int	optind, opterr; 

        int ret = -1;
        while (1) {
            ret = getopt(argc, argv, "zb:");
            if (ret == -1)
                break;
            if(ret=='b'){
                if(strcmp(optarg,"s")==0){
                    config.successed_bell=1;
                }else if(strcmp(optarg,"n")==0){
                    config.nomal_bell=1;
                }
                break;
            }else if(ret=='z'){
                config.cell_zero_finish=1;
            }else{
                printf("?? getopt returned character code 0%d char:%c ??\n", ret,ret);
            }
        }
        printf("|-option successed_bell:%d\n",config.successed_bell);
        printf("|-option nomal_bell:%d\n",config.nomal_bell);
        printf("|-option cell_zero_finish:%d\n",config.cell_zero_finish);
        printf("\n");
    #endif
    //fs2cell(be);
    //bio_env_print(be);
    puts("start.");
    int n=0;
    while(bio_env_is_running()){
        if(bio_env_cell_cycle(be)==0)break;
        n++;
    }
    #if defined(_use_conf)
        printf("%c",(config.nomal_bell?'\a':'\0'));
    #endif
    //bio_env_print(be);
    bio_env_free(be);
    printf("\nfinish.\n");
    return 0;
}

