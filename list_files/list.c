/*
 * C Program to List Files in Directory
 * https://www.sanfoundry.com/c-program-list-files-directory/
 */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char path[100000]="test";
char extension[][10] = {".jpg",".jpeg",".raw",".tif",".gif",".png",".bmp",".3dm",".max",".accdb",".db",".dbf",".mdb",".pdb",".sql",".dwg",".dxf",".c",".cpp",".cs",".h",".php",".asp",".rb",".java",".jar",".class",".py",".js",".aaf",".aep",".aepx",".plb",".prel",".prproj",".aet",".ppj",".psd",".indd",".indl",".indt",".indb",".inx",".idml",".pmd",".xqx",".xqx",".ai",".eps",".ps",".svg",".swf",".fla",".as3",".as",".txt",".doc",".dot",".docx",".docm",".dotx",".dotm",".docb",".rtf",".wpd",".wps",".msg",".pdf",".xls",".xlt",".xlm",".xlsx",".xlsm",".xltx",".xltm",".xlsb",".xla",".xlam",".xll",".xlw",".ppt",".pot",".pps",".pptx",".pptm",".potx",".potm",".ppam",".ppsx",".ppsm",".sldx",".sldm",".wav",".mp3",".aif",".iff",".m3u",".m4u",".mid",".mpa",".wma",".ra",".avi",".mov",".mp4",".3gp",".mpeg",".3g2",".asf",".asx",".flv",".mpg",".wmv",".vob",".m3u8",".mkv",".dat",".csv",".efx",".sdf",".vcf",".xml",".ses",".rar",".zip",".7zip"}; //agregar como lista de extensiones
int ext_len = sizeof(extension)/(sizeof(char)*10);
/*
.jpg .jpeg .raw .tif .gif .png .bmp
.3dm .max
.accdb .db .dbf .mdb .pdb .sql
.dwg .dxf
.c .cpp .cs .h .php .asp .rb .java .jar .class .py .js
.aaf .aep .aepx .plb .prel .prproj .aet .ppj .psd .indd .indl .indt .indb .inx .idml .pmd .xqx .xqx .ai .eps .ps .svg .swf .fla .as3 .as
.txt .doc .dot .docx .docm .dotx .dotm .docb .rtf .wpd .wps .msg .pdf .xls .xlt .xlm .xlsx .xlsm .xltx .xltm .xlsb .xla .xlam .xll .xlw .ppt .pot .pps .pptx .pptm .potx .potm .ppam .ppsx .ppsm .sldx .sldm
.wav .mp3 .aif .iff .m3u .m4u .mid .mpa .wma .ra .avi .mov .mp4 .3gp .mpeg .3g2 .asf .asx .flv .mpg .wmv .vob .m3u8 .mkv
.dat .csv .efx .sdf .vcf .xml .ses
.rar .zip .7zip
*/
int find_ext(char file[]){
    for(int i=0; i<ext_len; i++){
        if(strstr(file,extension[i]) != NULL){
            return 1;
        }
    }
    return 0;
}

int list_files(int n, int h)
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
                    list_files(strlen(path),h+1);
                    path[n] = 0;
                }else if(dir->d_type == DT_REG){
                    if(find_ext(dir->d_name)){
                        printf("%s/%s -> Filesito\n", path,dir->d_name);
                    }
                }
            }
        }
        closedir(d);
    }
    return(0);
}

void main(){
    list_files(strlen(path),0);
}