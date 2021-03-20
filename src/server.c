#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include "lib/debug.h"

#define print(x) printf(x "\n")
#define err(x) fprintf(stderr, x "\n")

#define BUFFSZ 1024

#define STR(X) #X
#define STRINGIFY(X) STR(X)

#ifndef _LOGFILE
#define _LOGFILE /opt/server/server_logs

#endif


void handle_connection(int port) {
    int conn_server, conn_client;
    struct sockaddr_in server, client;
    conn_server = socket(AF_INET, SOCK_STREAM, 0); // ip generic protocol
    time_t t;
    struct tm * mydate;
    const char * f_str = "%d-%b-%Y-%H-%M-%S-";
    /**
     * 
     * todo: use microseconds This is IMPERATIVE!
     * 
     */
    const char * logf_str = "%d %m %y - %T";
    const char * logfile = STRINGIFY(_LOGFILE);
    char * encfile = malloc(sizeof(char) * BUFFSZ * 2);
    int buffsize = 512;
    char buffer[BUFFSZ];
    char * date_str;
    char * logd_str;
    date_str = (char *) malloc(buffsize * sizeof(char));
    logd_str = (char *) malloc(buffsize * sizeof(char));
    
    if (date_str == NULL) {
        dbgerr("No space here!");
        exit(EXIT_FAILURE);
    }


    if (conn_server < 0) {
        dbgerr("socket() failed");
        exit(conn_server);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY; // acepto todo.
    int r = bind(conn_server, (struct sockaddr *)&server, sizeof(server));

    if (r < 0) {
        dbgerr("bind failed");
        exit(r);
    }

    listen(conn_server, 3);
    debug("Listening...");

    int sz = sizeof(struct sockaddr_in);
    debug("Press Ctrl+C to exit.");

    int fd = open(logfile, O_CREAT|O_WRONLY|O_APPEND, 0400);
    int enc_fd;

    while ( 1 ) {
        debug("[*] Waiting connection...");
        conn_client = accept(
            conn_server, 
            (struct sockaddr *)&client, 
            (socklen_t *)&sz
        );
        t = time(NULL);
        mydate = localtime(&t);
        
        if (mydate == NULL) {
            dbgerr("localtime failed!");
            exit(EXIT_FAILURE);
        }

        if (
            strftime(date_str, buffsize, f_str, mydate) == 0 ||
            strftime(logd_str, buffsize, logf_str, mydate) == 0
        ) {
            dbgerr("Error");
            exit(EXIT_FAILURE);
        }

        debug(
            "[%s]: Connection received from %s, saving in %s\n",
            logd_str , inet_ntoa(client.sin_addr), encfile
        );

        encfile[0] = '\0';
        strcat(encfile, "/tmp/enctest/");
        strcat(encfile, date_str);
        strcat(encfile, inet_ntoa(client.sin_addr));
        strcat(encfile, ".enc");

        enc_fd = open(encfile, O_CREAT | O_WRONLY, 0400);

        if (enc_fd < 0) {
            dbgerr("Error on opening enc_fd");
            dbgerr("File was not saved.");
            exit(fd);
        }

        dprintf(
            fd, "[%s]: Connection received from %s, saving in %s\n",
            logd_str , inet_ntoa(client.sin_addr), encfile
        );

        if (conn_client < 0) {
            dbgerr("accept failed. Exiting...");
            exit(conn_client);
        }

        write(conn_client,  date_str, strlen(date_str));
        read(conn_client,  &buffer, BUFFSZ);
        //printf("%s",buffer);
        dprintf(
            enc_fd, "%s", buffer
        );

        close(enc_fd);
        shutdown(conn_client, SHUT_RDWR);
        close(conn_client);
        debug("Connection with %s closed.", inet_ntoa(client.sin_addr));
    }

}

int main(int argc, char * argv[]) {
    print("Server Proyeto1 e.e");
    printf("Waiting connection...\n");
    handle_connection(4444);

    return 0;
}
