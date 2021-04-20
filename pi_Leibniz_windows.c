
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
struct args{
double first;
double n;
};
double pi_Leibniz(double n)
{
	double l=0;
	double m=0;
	double value1=0;
	for(double i=0; i<=n; i++)
	{	if((long long)i%2==0)
		{
			l=1;
		}
		else
		{
			l=-1;
		}
		m=(2*i)+1;
		value1+=(l/m);
	}
	return value1*4;
}
double value=0;
DWORD WINAPI pi_Leibniz_threads(LPVOID arg){
struct args *data = (struct args *) arg;
DWORD self=GetCurrentThreadId();
double size=(data->n)-(data->first);
size++;
double first=data->first;
printf("Thread %xd started, size =%2.0f first = %2.0f\n",self,size,first);
double l=0;
double m=0;
double result=0;
HANDLE Mutex;
for (double i=first;i<=data->n;i++)
{
	if((long long)i%2==0)
	{
		l=1;
	}
	else
	{
		l=-1;
	}
	m=(2*i)+1;
	result+=(l/m);
}
Mutex=CreateMutex(NULL,FALSE,NULL);
value+=result;
ReleaseMutex(Mutex);
CloseHandle(Mutex);
printf("Thread %xd sum=%2.20f\n",GetCurrentThreadId(),result);
}
int main(int argc, char **argv){
if(argc !=3)
{
	fprintf(stderr,"invalid number of arguments\n");
	return 1;
}
char *end;
char *end1;
double n_Leibniz=strtol(argv[1],&end,10);
int number_of_threads=strtol(argv[2],&end1,10);
long long range=n_Leibniz/number_of_threads;
if(*end !='\0' || *end1 != '\0')
{
	fprintf(stderr,"argument is not a number\n");
	return 1;
}
struct args arguments[number_of_threads];
for (int i=0; i<number_of_threads; i++)
{
	if(i==number_of_threads-1)
	{
		arguments[i].first=1+(i*range);
		arguments[i].n=n_Leibniz;
	}
	else
	{
		arguments[i].first=(i*range);
		arguments[i].n=range+(i*range);
	}

}
clock_t start,start1,end2,end3;
HANDLE threads[number_of_threads];
DWORD thrdids[number_of_threads];
start=clock();
for (int i=0; i<number_of_threads;i++)
{
	threads[i]=CreateThread(NULL,0,pi_Leibniz_threads,arguments+i,0,thrdids+i);
}

for(int i = 0; i <number_of_threads; i++) 
{
 	WaitForSingleObject(threads[i], INFINITE);
	CloseHandle(threads[i]);
}
end2=clock();
double time,time1;
time=(double)(end2-start)/CLOCKS_PER_SEC;
printf("with threads PI=%2.20f time=%2.8f\n",value*4,time);
start1=clock();
double value1=pi_Leibniz(n_Leibniz);
end3=clock();
time1=(double)(end3-start1)/CLOCKS_PER_SEC;
printf("without threads PI=%2.20f time=%2.8f\n",value1,time1);
return 0;
}