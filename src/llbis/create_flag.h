int create_flag(const char* filename){
    int fd = open(filename, O_CREAT | O_WRONLY, 0400);
    dprintf(fd, "1");
    close(fd);
    return 0;
}