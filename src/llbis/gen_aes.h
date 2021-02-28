char * gen_aes(){
    static char aes[48];
    srand(time(NULL)); //Tener cuidado que con la semilla pueden calcular la key
    char dic[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(int i=0; i<48; i++){
        aes[i] = dic[rand () % strlen(dic)];
    }
    //printf("%s\n",aes);
    return aes;
}