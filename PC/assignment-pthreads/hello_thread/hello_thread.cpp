#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;

void* f(void* p) 
{
    long val = (long) p;
    printf("I am thread %d in nbthreads \n \n", val);
    return  NULL;
}
int main (int argc, char* argv[]) {
pthread_t  th[50];
  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<"<nbthreads>"<<std::endl;
    return -1;
  }
  int n = stoi(argv[1]);
  for (int i=0; i < n; ++i)
  	pthread_create (&th[i], NULL , f,(void *) i);
  for (int i=0; i < n; i++)
	pthread_join(th[i], NULL);
  return 0;
}

