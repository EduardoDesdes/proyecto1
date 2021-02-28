#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

char * gen_aes(){
    static char aes[48];
    char dic[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    for(int i=0; i<48; i++){
        int nt = uniform_int_distribution<int>(0,47)(rng);
        aes[i] = dic[nt];
    }
    //printf("%s\n",aes);
    return aes;
}

int main(){
	printf("%s\n",gen_aes());
}
