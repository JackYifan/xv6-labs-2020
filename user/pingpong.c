#include "kernel/types.h"
#include "user.h"

#define MAX_LEN 10

int main(){
    //init pipe
    int p1[2],p2[2];
    pipe(p1); // child 0 read 
    pipe(p2); // child 1 write 
    if(fork() == 0){
        //child
        close(p1[1]);
        close(p2[0]);
        char buffer[MAX_LEN];
        read(p1[0],buffer,MAX_LEN);
        printf("%d: received %s\n", getpid(), buffer);
        write(p2[1],"pong",4);
    }else{
        close(p1[0]);
        close(p2[1]);
        write(p1[1],"ping",4);
        char buffer[MAX_LEN];
        read(p2[0],buffer,MAX_LEN);
        printf("%d: received %s\n", getpid(), buffer);
    }
    exit(0);
}