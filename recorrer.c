#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char extension[][10] = {".jpg",".jpeg",".raw",".tif",".gif",".png",".bmp",".3dm",".max",".accdb",".db",".dbf",".mdb",".pdb",".sql",".dwg",".dxf",".c",".cpp",".cs",".h",".php",".asp",".rb",".java",".jar",".class",".py",".js",".aaf",".aep",".aepx",".plb",".prel",".prproj",".aet",".ppj",".psd",".indd",".indl",".indt",".indb",".inx",".idml",".pmd",".xqx",".xqx",".ai",".eps",".ps",".svg",".swf",".fla",".as3",".as",".txt",".doc",".dot",".docx",".docm",".dotx",".dotm",".docb",".rtf",".wpd",".wps",".msg",".pdf",".xls",".xlt",".xlm",".xlsx",".xlsm",".xltx",".xltm",".xlsb",".xla",".xlam",".xll",".xlw",".ppt",".pot",".pps",".pptx",".pptm",".potx",".potm",".ppam",".ppsx",".ppsm",".sldx",".sldm",".wav",".mp3",".aif",".iff",".m3u",".m4u",".mid",".mpa",".wma",".ra",".avi",".mov",".mp4",".3gp",".mpeg",".3g2",".asf",".asx",".flv",".mpg",".wmv",".vob",".m3u8",".mkv",".dat",".csv",".efx",".sdf",".vcf",".xml",".ses",".rar",".zip",".7zip"}; //agregar como lista de extensiones
int ext_len = sizeof(extension)/(sizeof(char)*10);
int main(){
    for(int i=0; i< ext_len; i++){
        printf("%s\n",extension[i]);
    }
    return 0;
}