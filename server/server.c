#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

#define print(x) printf(x "\n")
#define err(x) fprintf(stderr, x "\n")


void handle_connection(int port) {
    int conn_server, conn_client;
    struct sockaddr_in server, client;
    conn_server = socket(AF_INET, SOCK_STREAM, 0); // ip generic protocol
    time_t t;
    struct tm * mydate;
    const char * f_str = "%A, %d %b %Y %T\n";
    const char * logf_str = "%d %m %y - %T";
    const char * filename = "server_logs";
    const char * enviame = "/tmp/enviame.enc";
    int buffsize = 512;
    char buffer[800];
    char * date_str;
    char * logd_str;
    date_str = (char *) malloc(buffsize * sizeof(char));
    logd_str = (char *) malloc(buffsize * sizeof(char));
    
    if (date_str == NULL) {
        err("No space here!");
        exit(EXIT_FAILURE);
    }


    if (conn_server < 0) {
        err("socket() failed");
        exit(conn_server);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY; // acepto todo.
    int r = bind(conn_server, (struct sockaddr *)&server, sizeof(server));

    if (r < 0) {
        err("bind failed");
        exit(r);
    }

    listen(conn_server, 3);
    print("Listening...");

    int sz = sizeof(struct sockaddr_in);
    print("Press Ctrl+C to exit.");

    int fd = open(filename, O_CREAT|O_WRONLY|O_APPEND, 0666);
    int fd2 = open(enviame, O_CREAT|O_WRONLY|O_TRUNC, 0666);

    while (1) {
        conn_client = accept(
            conn_server, 
            (struct sockaddr *)&client, 
            (socklen_t *)&sz
        );
        t = time(NULL);
        mydate = localtime(&t);
        
        if (mydate == NULL) {
            err("localtime failed!");
            exit(EXIT_FAILURE);
        }

        if (
            strftime(date_str, buffsize, f_str, mydate) == 0 ||
            strftime(logd_str, buffsize, logf_str, mydate) == 0
        ) {
            err("Error");
            exit(EXIT_FAILURE);
        }

        dprintf(
            fd, "[%s]: Connection received from %s, saving in %s\n",
            logd_str , inet_ntoa(client.sin_addr), enviame
        );

        if (conn_client < 0) {
            err("accept failed. Exiting...");
            exit(conn_client);
        }

        write(conn_client,  date_str, strlen(date_str));
        read(conn_client,  &buffer, 800);
        //printf("%s",buffer);
        dprintf(
            fd2, "%s", buffer
        );
        shutdown(conn_client, SHUT_RDWR);
        close(conn_client);
    }

}

int main(int argc, char * argv[]) {
    print("Server Proyeto1 e.e");
    printf("Esperando el fichero enviame.enc\n");
    handle_connection(4444);

    return 0;
}
