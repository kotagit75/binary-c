#include "../lib/main.h"

void copyDirectory(const char* srcDir, const char* destDir,int ismkdir) {
    DIR* dir;
    struct dirent* entry;
    struct stat statbuf;
    char srcPath[256];
    char destPath[256];
    // コピー元のディレクトリを開く
    dir = opendir(srcDir);
    if (dir == NULL) {
        printf("Failed to open source directory.(path:%s)\n",srcDir);
        return;
    }
    // コピー先のディレクトリを作成
    if(ismkdir)mkdir(destDir, 0755);
    // コピー元のディレクトリ内のファイルやサブディレクトリを取得
    while ((entry = readdir(dir)) != NULL) {
        // "." と ".." はスキップする
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // コピー元のパスを作成
        snprintf(srcPath, sizeof(srcPath), "%s/%s", srcDir, entry->d_name);
        // ファイルやディレクトリの情報を取得
        if (lstat(srcPath, &statbuf) == -1) {
            printf("Failed to get file information.\n");
            continue;
        }
        // コピー先のパスを作成
        snprintf(destPath, sizeof(destPath), "%s/%s", destDir, entry->d_name);
        // ファイルの場合はコピーする
        if (S_ISREG(statbuf.st_mode)) {
            FILE* srcFile = fopen(srcPath, "rb");
            FILE* destFile = fopen(destPath, "wb");
            if (srcFile == NULL || destFile == NULL) {
                printf("Failed to open file.\n");
                continue;
            }
            char buffer[1024];
            size_t bytesRead;
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), srcFile)) > 0) {
                fwrite(buffer, 1, bytesRead, destFile);
            }
            fclose(srcFile);
            fclose(destFile);
        }
        // ディレクトリの場合は再帰的にコピーする
        else if (S_ISDIR(statbuf.st_mode)) {
            copyDirectory(srcPath, destPath,1);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]){
    if(argc<2)return 1;
    if(strcmp(argv[1],"init")==0){
        char* path=getenv("BINC");
        if(path==NULL)return 1;
        printf("%s\n",path);
        char init_path_cpyfrom[100];
        if(argc==3 && strcmp(argv[2],"tool")==0){
            sprintf(init_path_cpyfrom,"%s%s%s",path,model_dir,"tool");
            char cwd[30];getcwd(cwd,sizeof(cwd));
            if(cwd==NULL)return 1;
            char tool[30];
            sprintf(tool,"%s/%s",cwd,"tool");
            printf("from:%s\n",init_path_cpyfrom);
            printf("to:%s\n",tool);
            copyDirectory(init_path_cpyfrom,tool,1);
            printf("complated.\n");
        }
        else{
            sprintf(init_path_cpyfrom,"%s%s%s",path,model_dir,"petri_dish");
            char cwd[30];getcwd(cwd,sizeof(cwd));
            if(cwd==NULL)return 1;
            printf("from:%s\n",init_path_cpyfrom);
            printf("to:%s\n",cwd);
            copyDirectory(init_path_cpyfrom,cwd,0);
            printf("complated.\n");
        }
    }else{
        char cmd[100];
        sprintf(cmd,"make %s",argv[1]);
        system(cmd);
    }
}