#ifndef _KEY_PUB_H
#define _KEY_PUB_H

#define DEF_KEY(KEYPUB) \
    char * pub_key =  { \
        "-----BEGIN PUBLIC KEY-----\n"  \
        KEYPUB               \
        "\n-----END PUBLIC KEY-----"      \
    }   

void gen_key_pub(){
    
    DEF_KEY(KEY_PUB);

    int fd = open("clave.pub.pem", O_CREAT| O_WRONLY, 0600);
    
    if (fd < 0) {
        dbgerr("Error trying to open file descriptor");
        exit(-1);
    }

    write(fd, pub_key, strlen(pub_key));
    close(fd);
    fsync(fd);   
}


void rm_key_pub() {
    unlink("clave.pub.pem");
}

#endif