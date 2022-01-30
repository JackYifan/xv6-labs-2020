#include "kernel/types.h"
#include "user.h"
#include "kernel/param.h"

#define MAX_LEN_WORD 10

int main(int argc, char **argv){
    char* args[MAXARG];
    int idx = argc - 1;
    for(int i=1;i<argc;i++){
        args[i-1] = argv[i]; 
    }

    char c;
    char arg[MAX_LEN_WORD];
    int arg_idx = 0;
    while(read(0,&c,1)==1){
        if(c == ' ' || c == '\n'){
            arg[arg_idx] = '\0';
            args[idx] = (char*)malloc(arg_idx);
            strcpy(args[idx++],arg);
            arg_idx = 0;
            if(c == '\n'){
                //执行命令
                arg[idx] = 0;
                if(fork() == 0){
                    exec(args[0],args);
                }
                wait(0);
                idx = argc - 1;
            }
        }else{
            arg[arg_idx++] = c;
        }
    }
    if(idx > argc -1){
        if(fork() == 0){
            exec(args[0],args);
        }
        wait(0);
    }
    exit(0);
}