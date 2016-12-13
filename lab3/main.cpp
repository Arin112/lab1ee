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
/*
void printPt(FILE *f, pthread_t pt) {
  unsigned char *ptc = (unsigned char*)(void*)(&pt);
  printf("0x");
  for (size_t i=0; i<sizeof(pt); i++) {
    printf("%02x", (unsigned)(ptc[i]));
  }
}
*/
//using namespace std;

void outFib(){

  pthread_t pthrd = pthread_self();
  if(sizeof(long double)>=12){
    long double  a=1, b=1, t;
    int i=0;
    printf("Я, %0X, начал работу\n", int(pthrd));
    do{
      printf("Я, %0X, говорю, что %0X-ое число фибоначчи - %.0Lf\n", int(pthrd), int(pthrd), a);
      t=b;
      b=a+b;
      a=t;
    }while(++i<100);
    printf("Я, %0X, закончил работу\n", int(pthrd));
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
  
  int N=10;
  
  pthread_t threads[N];
  
  for(int i=0;i<N;i++)
    pthread_create(&threads[i], 0, thfunc, 0);
  
  for(int i=0;i<N; i++){
    pthread_join(threads[i], 0);
  }
  
  return 0;
}
