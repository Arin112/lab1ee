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
#include <pthread.h>
//--

using namespace std;

void outFib(){
  if(sizeof(long double)>=12){
    long double  a=1, b=1, t;
    int i=0;
    do{
      printf("%.0Lf\n", a);
      t=b;
      b=a+b;
      a=t;
    }while(++i<100);
  }else{
    printf("Can't out 100 fibs using long double witch sizeof == %d.\n", int(sizeof(long double)));
    printf("Не хочу писать длинную арифметику\n");
  }
}

void * thfunc(void * arg){
  outFib();
}
  
int main(){
  
  //cout<<int(sizeof(double))<<"\n";
  
  const int N=10;
  
  pthread_t threads[N];
  
  for(int i=0;i<N;i++)
    pthread_create(&threads[i], 0, thfunc, 0);
  
  for(int i=0;i<N; i++){
    pthread_join(threads[i], 0);
  }
  
  return 0;
}
