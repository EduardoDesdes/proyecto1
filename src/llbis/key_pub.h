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