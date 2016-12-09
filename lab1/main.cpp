#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <cmath>

#define MT_PER_SEC (1000000)

typedef unsigned long ulong;

ulong GetTickCount(){
 struct timeval tv;
 gettimeofday(&tv,NULL);
 return ((tv.tv_sec)*1000000 + (tv.tv_usec));
}

int main(){
  
 printf("\nЭто лабораторная работа №1.\n\n"
        "Исследование времени работы функции fork в зависимости от используемой программой динамической памяти.\n"
        );
 
  printf("Время считается с начала работы функции fork.\n"
         "\nВремя для родителя1 и ребёнка1 засекается без выделения динамической памяти.\n\n"
         );
  
  ulong t1 = GetTickCount();
  int pid = fork();
  if(pid){
    printf("Время до возобновления работы родителя1 %f сек\n", (float(GetTickCount() - t1))/MT_PER_SEC);
    
  }else{
    printf("Время до начала работы ребёнка1 \t%f сек\n" , (float(GetTickCount() - t1))/MT_PER_SEC);
  }
  if(!pid)return 0;
  
  //sleep(1);
  fflush(stdout);
  
  printf("\nВремя для родителя2 и ребёнка2 засекается с выделением памяти(1GB).\n\n");
  
  int *p = new int[1024*1024*1024/sizeof(int)];
  
  for(int i=0;i<1024*1024*1024/sizeof(int);i++){
    p[i]=i;
  }
  
  t1 = GetTickCount();
  pid = fork();
  if(pid){
    printf("Время до возобновления работы родителя2 %f сек\n", (float(GetTickCount() - t1))/MT_PER_SEC);
  }else{
    printf("Время до начала работы ребёнка2 \t%f сек\n" , (float(GetTickCount() - t1))/MT_PER_SEC);
  }
  //fflush(stdout);
  delete[] p;
  if(!pid)return 0;
  fflush(stdout);
  //sleep(5);
  printf("123\n");
  printf("\nКак видно из чисел выше, время работы fork зависит от количества выделенной динамической памяти.\n"
         "Также отличается время, требуемое для возобновления работы родителя и для начала работы дочерней копии\n"
         " - для начала работы копии требуется больше времени\n"
         );
  
  return 0;
}
