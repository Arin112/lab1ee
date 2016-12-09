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
char *strerror(int errnum);
//---         

//---
#include <sys/wait.h>
//---

using namespace std;

int main(){
  
  pid_t pid;
  
  int r=0;
  
  char buf[100]={}, rbuf[200]={};
  
  int fd = open("file", O_RDWR|O_CREAT|O_TRUNC, 0666);  
  
  if(fd==-1){
    cout<<"Can't open/create file\n";
    cout<<strerror(errno)<<"\n";
    return -1;
  }
  
  srand(time(0));
  
  if(pid = fork()){
    srand(1);
    r = rand();
    
    sprintf(buf, "I'm father and my number is %d\n", r);
    
    write(fd, buf, strlen(buf));
    
    //cout<<pid<<" Ya roditel\n";
      
    waitpid(-1, NULL, 0);
    
    lseek(fd, 0, SEEK_SET);
    read(fd, rbuf, 199);
    cout<<"Я родитель и вот что я и мой сын записали в файл:\n"<<rbuf;
  }else{
    
    r = rand();
    
    sprintf(buf, "I'm son and my number is %d\n", r);
    sleep(1);
    write(fd, buf, strlen(buf));
    
    //cout<<pid<<" Ya rebenok\n";
  }
  
  close(fd);
  return 0;
}