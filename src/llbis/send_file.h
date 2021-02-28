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