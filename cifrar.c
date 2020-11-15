#include <stdio.h>

void cifrar(char path[], char file[], char passwd[]){
  char command[100000]="openssl enc -aes-256-cbc -pass pass:";
  strcat(command,passwd);
  strcat(command," -in ");
  strcat(command,path);
  strcat(command,"/");
  strcat(command,file);
  strcat(command," -out ");
  strcat(command,path);
  strcat(command,"/");
  strcat(command,file);
  strcat(command,".ntd 2>/dev/null");  

  //openssl enc -aes-256-cbc -pass pass:1234 -in plain.txt -out plain.txt.ntd
  system(command);
}

int main(int arc, char *argv[])
{ 
  cifrar(".","plain.txt","1234");
  return 0;
}