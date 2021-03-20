/*
 * C Program to List Files in Directory
 * https://www.sanfoundry.com/c-program-list-files-directory/
 */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int nf=0; //Numero de archivos
char path[100000]="/home";

int list_files(int n){
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d){
        while ((dir = readdir(d)) != NULL){
            if(strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")){
                if(dir->d_type == DT_DIR){
                    strcat(path,"/");
                    strcat(path,dir->d_name);
                    list_files(strlen(path));
                    path[n] = 0;
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
    list_files(strlen(path));
    printf("Cantidad Total de archivos: %i",nf);
}
