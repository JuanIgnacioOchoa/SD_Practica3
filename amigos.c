#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int suma(int x){
    int res = 1;
    int i = 0;
    for(i = 2; i < x; i++){
        if(x % i == 0)
            res += i;
    }
    return res;
}
int main(int argc, char *argv[]){
    printf("Ingresar el valor inicial\n");
    int a;
    scanf("%d", &a);
    printf("Ingresar el valor final\n");
    int b;
    scanf("%d", &b);
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned int start = tv.tv_sec;
    int x = 0;
    for(x = a; x < b-1; x++) {
        int y = x+1;
        for(; y < b; y++){
            int res1 = suma(x);
            int res2 = suma(y);
            if(res1 == y && res2 == x){
                printf("%d y %d son numeros amigos\n", res2, res1);
            }
        }
    }
    gettimeofday(&tv, NULL);
    unsigned int end = tv.tv_sec;
    printf("%d\n", end - start);
}