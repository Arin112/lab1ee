#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>

#include <stdlib.h>
#include <time.h>

#include <termios.h>

#include <semaphore.h>

using namespace std;

sem_t semaphore;

int wfd, rfd;


char getch(){

	char buf=0;
	struct termios old={0};
	fflush(stdout);
	if(tcgetattr(0, &old)<0)
		perror("tcsetattr()");
	old.c_lflag&=~ICANON;
	old.c_lflag&=~ECHO;
	old.c_cc[VMIN]=1;
	old.c_cc[VTIME]=0;
	if(tcsetattr(0, TCSANOW, &old)<0)
		perror("tcsetattr ICANON");
	if(read(0,&buf,1)<0)
		perror("read()");
	old.c_lflag|=ICANON;
	old.c_lflag|=ECHO;
	if(tcsetattr(0, TCSADRAIN, &old)<0)
		perror ("tcsetattr ~ICANON");
	//printf("%c\n",buf);
	return buf;
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


void clearStr(char* str, int len){
	for(int i=0;i<len;i++){
		str[i]=0;
	}
	return;
}

bool readLine(int fd, char *str){
	char ch=0;
	for(;read(fd, &ch, 1)!=0;str++){
		if(ch=='\n')return true;
		*str=ch;
	}
	return false;
}

void myInit(){
	sem_init(&semaphore, 0, 1);
}

int myLock(const char *str){
	sem_wait(&semaphore);
	printf("%s locks the semaphore\n", str);
	getch();
}

void myUnlock(const char* str){
	sem_post(&semaphore);
	printf("%s unlocks the semaphore\n", str);
	getch();
}

void reader(char *name){
	char str[100], buf[1000];

	for(;;){
		clearStr(str, 100);
		clearStr(buf, 1000);

		myLock(name);

		if(readLine(rfd, str))
			printf("My name is \"%s\", and i have read flowing string : \"%s\"\n", name, str);
		else
			printf("My name is \"%s\", and i can't read string, becouse of EOF\n", name);

		myUnlock(name);
		usleep(rand()%100000);
	}

	return;
}

void writeInfoLine(int wfd, const char *str){
	char buf[100];
	clearStr(buf, 100);
	sprintf(buf, "I am the main thread and i have write this :%s, %s\n", getTime(), str);
	write(wfd, buf, strlen(buf));
}

void* thfunc(void* arg){
	reader((char*)(arg));
}

int main(){
	int N=10;
	myInit();
	pthread_t threads[N];
	char names[10][10]={};
	wfd = open("file", O_RDWR|O_CREAT|O_TRUNC, 0666); 
	if(wfd==-1){
		printf("can't open file for write\n");
		return -1;
	}

	rfd = open("file", O_RDWR|O_CREAT|O_TRUNC, 0666); 
	if(rfd==-1){
		printf("can't open file for read\n");
		return -1;
	}


	for(int i=0;i<N;i++){
		sprintf(names[i], "%d", i);
		pthread_create(&threads[i], 0, thfunc, (void*)names[i]);
	}
	int lineNumber=0;
	for(int i=0;i<50;i++){
		myLock("main");
		for(int j=0, r=rand()%10 + 4;j<r;j++){
			char buf[30]={};
			sprintf(buf, "line is %d", lineNumber++);
			writeInfoLine(wfd, buf);
		}
		myUnlock("main");
		usleep(21000);
	}
	usleep(1000000);
	return 0;
}

