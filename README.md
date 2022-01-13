# File System
## Big File
```cpp
//定义新的索引数量
#define NDIRECT 11
#define NINDIRECT (BSIZE / sizeof(uint))
#define NINDIRECT2 (NINDIRECT*NINDIRECT)
#define MAXFILE (NDIRECT + NINDIRECT + NINDIRECT2)
```
## Symbolic links
```cpp
uint64
sys_symlink(void)
{
  char target[MAXPATH], path[MAXPATH]; //参数
  if(argstr(0,target,MAXPATH) < 0 || argstr(1,path,MAXPATH) < 0){
    return -1;
  }

  begin_op(); //lock
  struct inode* ip;
  if((ip = create(path,T_SYMLINK,0,0))==0){
    end_op();
    return -1;
  }
  // 将目标的路径写入软连接的inode,target类型是char* 存储的是拷贝开始的地址，转为uint64类型地址
  if(writei(ip,0,(uint64)target,0,MAXPATH) != MAXPATH){
    end_op();
    return -1;
  }

  iupdate(ip); //将更新完的inode拷贝到disk
  iunlockput(ip); //解锁并回收inode空间
  end_op();
  return 0;
}
```
**Copy a modified in-memory inode to disk.
Must be called after every change to an ip->xxx field
that lives on disk, since i-node cache is write-through.
Caller must hold ip->lock.**
在内存中的inode是in-memory copy
