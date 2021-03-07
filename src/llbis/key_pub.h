void key_pub(int i){
    char * pub_key = {
        "-----BEGIN PUBLIC KEY-----\n"
	"MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAsU7b9tr+vYd9PfqOCUJ3HNKzDlMhCitOOv18TukGjtr5i/ryS2eNhZAIcAdpWyosGCzODsp1RQXzjlqFVqzPyg86hGxgFgwHvRzQciQ6xNWgd97bffYH6UH9iTmtN/RlNPRAx7l8nuVCJJSX8IJsU1I5HFYjsmYT42XNBE/wWdtkIU6HCpI/Ke4HuAPZC92U8yumg8wvDPr2E65w2bwZOaJ2vzXuvZiGq8Rkx/ueaJpeCuJ0Sy6WOMI7VVWbF6cHFQMvejZS2s+USdX2TWTK52yEaHf99B8jaKGk/8eeiQI78GAdWkIrgIr1+81ajyQHAs5ke2wXyZJMKI+ZnPYjVO/TL9bM3dDFiQU9qWVDwacn1paksw6bbQP9Kt2HNIDDU+TjWp7eqk5F5KAJSIvMWkv70ePMdChoYzV7Ed8f1TrnkgroDaUFtAh/KfY3U89Poa4Dto+ku3jZ60S2w6/cuGlEdIZm836LNZsKpuvwaPmgABWLtbc8IQjuqXgpcQ7jMlFSFt43gD7oIFWYhkP1vZe0vVknGnstapmKsYNUwuh66Dj573aeu/BZ36oyzZmcswJlNO80JUiXkiHnmmmZ/8MBUl1MsFFbK225wdsbYvoEDsekd8yNqnmwvTxDFmTpc8Xd7oKEDirwGQFR73L1/EXIQ+latY9FN7YgFxxrnEkCAwEAAQ==\n"
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
