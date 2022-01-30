#include "kernel/types.h"
#include "user.h"

#define MAX 35

//筛选素数
void sieve(int readfd){
    //read returns zero when the write-side of a pipe is closed. 
    int prime;
    if(read(readfd,&prime,sizeof(int))==0){
        close(readfd);
        return;
    }

    printf("prime %d\n",prime);

    int p[2];
    pipe(p);
    if(fork() == 0){
        //child
        close(p[1]); // close write
        sieve(p[0]);
        exit(0);


    }else{
        close(p[0]); //close read
        int num;
        while(read(readfd,&num,sizeof(int))){
            // 一直读到pipe关闭
            if(num % prime != 0){
                write(p[1],&num,sizeof(int));
            }
        }
        close(readfd);
        close(p[1]);
        wait(0); //Wait for a child process to exit and return its pid.
        exit(0);
    }
}

int main(){
    int p[2];
    pipe(p);
    for(int i=2;i<=MAX;i++){
        write(p[1],&i,sizeof(int));
    }
    close(p[1]);
    sieve(p[0]);
    exit(0);
    
}