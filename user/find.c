#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char* path,char* file_name){
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type){
        case T_FILE:
            if(strcmp(fmtname(path),file_name)==0){
                printf("%s\n",path);
            }
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("find: path too long\n");
                close(fd);
                return;
		    }
            // restore current path
            strcpy(buf,path);
            p = buf + strlen(path);
            *p++ = '/';

            while(read(fd,&de,sizeof(de))==sizeof(de)){
                //read directory entry by file description
                if(de.inum == 0){
                    //empty directory
                    continue;
                }
                // path plus the directory name 
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0; // now buf contains the current path
                if(stat(buf, &st) < 0){
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0){
                    continue; // avoid recurse into "." or ".."
                }
                //recursive
                find(buf,file_name);

            }
            break;
    }
    close(fd);
}

int main(int argc,char* argv[]){
    if(argc<3) {
        printf("require 3 arguments\n");
        exit(0);
    }
    char file_name[DIRSIZ+1];
    strcpy(file_name,argv[2]);
    memset(file_name+strlen(argv[2]), ' ', DIRSIZ-strlen(argv[2]));
    find(argv[1],file_name);
    exit(0);
}