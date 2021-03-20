/*
 * C Program to List Files in Directory
 * https://www.sanfoundry.com/c-program-list-files-directory/
 */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nf=0; //Numero de archivos
char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int list_files(char path[]){
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d){
        while ((dir = readdir(d)) != NULL){
            if(strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")){
                if(dir->d_type == DT_DIR){
                    list_files(concat(concat(path,"/"),dir->d_name));
                }else if(dir->d_type == DT_REG){
                    nf++;
                }
            }
        }
        closedir(d);
    }
    return(0);
}

void main(){
    list_files("/home");
    printf("Cantidad Total de archivos: %i",nf);
}
