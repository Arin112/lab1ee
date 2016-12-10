#include <iostream>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <errno.h>

//---
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//---

//---
//char *strerror(int errnum);
//---

//---
#include <pthread.h>
//---

//---
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//---

int syncRead(int fd, char* buf, int count){
  int s=0, t;
  do{
    t=read(fd, buf+s, count-s);
    if(t!=-1)s+=t;
    else{
      return -1;
    }
  }while(s!=count);
  return 0;
}

int syncWrite(int fd, char* buf, int count){
  int s=0, t;
  do{
    t=write(fd, buf+s, count-s);
    if(t!=-1)s+=t;
    else{
      return -1;
    }
  }while(s!=count);
  return 0;
}

char * getTime(){
  time_t current_time;
  struct tm * time_info;
  static char timeString[9];  // space for "HH:MM:SS\0"
  
  time(&current_time);
  time_info = localtime(&current_time);
  
  strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
  return timeString;
}

using namespace std;
  
int main(int argc, char *argv[]){
  const int size=20000;

  const int N=10;

  char *array;
  int shmid;
  int n = 1;
  bool rw;
  const char *pathname = "shMem";
  
  key_t key;
  
  if((key = ftok(pathname,0)) < 0){
    cout<<strerror(errno)<<"\n";
    printf("Can't generate key\n");
    exit(-1);
  }
  
  if((shmid = shmget(key, size, 0666|IPC_CREAT|IPC_EXCL)) < 0){
    if(errno != EEXIST){
      printf("Can't create shared memory\n");
      exit(-1);
    } else {
    
      if((shmid = shmget(key, size, 0666)) < 0){
        printf("Can't find shared memory\n");
        cout<<strerror(errno)<<"\n";
        exit(-1);
      }
      n = 0;
    }
  }

  if((array = (char *)shmat(shmid, NULL, 0)) ==(char*) (-1)){
    printf("Can't attach shared memory\n");
    exit(-1);
  }
  
  if(mkfifo("fifofile", 0777)<0){
    if(errno == EEXIST){
      rw=false;
      cout<<"Открываю на запись, "<<getpid()<<"\n";
    }else{
      cout<<"Can't make fifo:\n";
      cout<<strerror(errno)<<"\n";
      shmdt(array);
      exit(-1);
    }
  }else{
    rw=true;
    cout<<"Открываю на чтение, "<<getpid()<<"\n";
  }
  
  int fd;
  
  if(rw){
    if((fd = open("fifofile", O_RDONLY)) == -1){
      cout<<"Can't open fifofile\n";
      shmdt(array);
      return -1;
    }
  }else{
    if((fd = open("fifofile", O_WRONLY)) == -1){
      cout<<"Can't open fifofile\n";
      shmdt(array);
      return -1;
    }
  }

  if(rw){
    for(int i=0;i<size;i++){
      array[i]=0;
    }
  }
  
  if(rw){
    
    int offset=0;
    char sem=0;
    
    for(int i=0;i<N;i++){
      
      if((i+1)%2)
        syncRead(fd, &sem, 1);
      for(;array[offset];offset++);
      cout<<offset<<"\n";
      sprintf(array + offset, "pid - %d, time - %s\n", int(getpid()), getTime());
      if((i)%2)
        syncRead(fd, &sem, 1);
      //cout<<getTime()<<"\n";
    }
    
  }else{
  
    int offset=0;
    char sem=1;
    
    for(int i=0;i<N;i++){
      
      
      if((i)%2)
        syncWrite(fd, &sem, 1);
      //cout<<getTime()<<"\n";
      for(;array[offset];offset++);
      if(offset==280)offset=0;
      cout<<offset<<"\n";
      sprintf(array + offset, "pid - %d, time - %s\n", int(getpid()), getTime());
      if((i+1)%2)
        syncWrite(fd, &sem, 1);
      
    }

  }

  if(rw){
    cout<<"Результирующая память:\n";
    for(int i=0;array[i];i++){
      cout<<array[i];
    }
    cout<<"\n";
  }
  close(fd);
  
  shmdt(array);
  
  return 0;
}
