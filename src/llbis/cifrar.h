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