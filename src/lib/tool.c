#include "main.h"

int exist_file(const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        return 0;
    }

    fclose(fp);
    return 1;
}

int exist_dir(const char *path){
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

int get_digit(int n)
{
    int digit = 0;

    while (n != 0) {
        n /= 10;
        ++digit;
    };

    return digit;
}

char *GetFileName( const char *file_path)
{
    char* dir_end=NULL;

    char *file_name;

    if ((file_path == NULL) || (file_name==NULL))
    {
        return NULL;
    }

    dir_end = strrchr(file_path,'/');
    if (dir_end == NULL)
    {
        return NULL;
    }

    (void)sprintf(file_name,"%s",dir_end+1);

    return file_name;
}

char* str_concat(char *str1, const char *str2) {

    // str1の最初のアドレスを保持しておく
    char *top = str1;

    // 文字列終端までポインタを進める
    while (*(str1++) != '\0');

    // `\0`分を消すため、ポインタをひとつ戻す（連結のため）
    str1 -= 1;

    // 終端直前から、str2の文字を追加していく
    do {
        *(str1++) = *str2;
    } while (*(str2++) != '\0');

    return str1;
}

/**
 *  mallocを使ってヒープに領域を確保するパターン
 */
char* str_concat2(const char *str1, const char *str2, char **result) {

    // 連結後の文字列分の領域を確保する（+1は終端文字（\0）分）
    size_t size = sizeof(char) * (strlen(str1) + strlen(str2) + 1);
    char *work = (char*)malloc(size);
    if (work == NULL) {
        printf("Cannot allocate memory.\n");
        return NULL;
    }

    // 確保した領域の最初のアドレスを保持しておく
    char *top = work;

    // str1をコピー
    strcpy(work, str1);

    // 追加した文字数分、ポインタを進める
    work += strlen(str1);

    // str2をコピー
    strcpy(work, str2);

    // 確保した先頭アドレスを結果に格納
    *result = top;

    return top;
}

/**
 *  文字列を置換する
 *
 *  @param src 評価する文字列
 *  @param target  置き換え対象となる文字列
 *  @param replace 置き換える文字列
 *  @param result  置換後の文字列を返すポインタ
 */
void str_replace(const char *src, const char *target, const char *replace, char **result) {

    char *temp = (char*)malloc(sizeof(char) * 1000);
    if (temp == NULL) {
        printf("Cannot allocate memory.\n");
        return;
    }

    char *top = temp;

    // 操作できるようにコピーする
    char *work = (char*)malloc(sizeof(char) * strlen(src));
    strcpy(work, src);

    char *p;
    while ((p = strstr(work, target)) != NULL) {
        // 検知した位置に文字列終端文字を挿入
        *p = '\0';

        // 検知した位置＋対象文字数分の位置にポインタを移動
        p += strlen(target);

        // 該当文字列以降をいったんtempに退避
        strcpy(temp, p);

        // 前半文字列と置き換え文字列を連結する
        str_concat(work, replace);
        str_concat(work, temp);
    }

    free(temp);

    *result = work;
}