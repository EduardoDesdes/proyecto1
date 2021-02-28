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