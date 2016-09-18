#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <cmath>

using std::abs;

int main(){
  
 printf("\nЭто лабораторная работа №1.\n\n"
        "Исследование времени работы функции fork в зависимости от используемой программой динамической памяти.\n"
        );
 
  printf("Время считается с начала работы функции fork.\n"
         "\nВремя для родителя1 и ребёнка1 засекается без выделения динамической памяти.\n\n"
         );
  
  clock_t t1 = clock();
  
  int pid = fork();
  if(pid){
    printf("Время до возобновления работы родителя1 %f сек\n", abs(float(clock() - t1))/CLOCKS_PER_SEC);
  }else{
    printf("Время до начала работы ребёнка1 \t%f сек\n" , abs(float(clock() - t1))/CLOCKS_PER_SEC);
  }
  if(!pid)return 0;
  
  sleep(1);
  
  printf("\nВремя для родителя2 и ребёнка2 засекается с выделением памяти(1GB).\n\n");
  
  int *p = new int[1024*1024*1024/sizeof(int)];
  
  for(int i=0;i<1024*1024*1024/sizeof(int);i++){
    p[i]=i;
  }
  
  t1 = clock();
  pid = fork();
  if(pid){
    printf("Время до возобновления работы родителя2 %f сек\n", abs(float(clock() - t1))/CLOCKS_PER_SEC);
  }else{
    printf("Время до начала работы ребёнка2 \t%f сек\n" , abs(float(clock() - t1))/CLOCKS_PER_SEC);
  }
  delete[] p;
  if(!pid)return 0;
  sleep(5);
  printf("\nКак видно из чисел выше, время работы fork зависит от количества выделенной динамической памяти.\n"
         "Также существенно отличается время, требуемое для возобновления работы родителя и для начала работы дочерней копии,\n"
         "причём чем больше памяти используется, тем больше времени пройдёт до начала работы копии.\n\n");
  
  return 0;
}
