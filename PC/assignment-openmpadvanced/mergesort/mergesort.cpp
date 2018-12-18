#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <cstring>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void merge(int * arr, int n, int * temp) {
  int i = 0;
  int k = 0;
  int j = n/2;
 
  while (i<n/2 && j<n) 
  {
     if (arr[i] < arr[j] ) 
     {
       	temp[k++] = arr[i++];
     } 
     else 
     {
       temp[k++] = arr[j++];
     }
  }
  while (i < n/2) 
  {
     temp[k++] = arr[i++];
  }
  while (j < n) 
  {
     temp[k++] = arr[j++];
  }
  for (int l=0;l<n;l++)
  {
      arr[l]=temp[l];
  }
}


void mergesort(int * arr, int n, int * temp, int chunkSize) {
if(n < 2)
return;
int mid = n/2;
if(n >= chunkSize)
{
	#pragma omp task
	{
 	mergesort(arr,mid,temp,chunkSize);
	}
	#pragma omp taskwait
	#pragma omp task
	{
	 mergesort(arr+mid,n-mid,temp,chunkSize);
	}
	#pragma omp taskwait
	merge(arr,n,temp);
}
else
{
     mergesort(arr,mid,temp,chunkSize);
     mergesort(arr+mid,n-mid,temp,chunkSize);
     merge(arr,n,temp);
}
}


int main (int argc, char* argv[]) {
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }

  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * arr = new int [n];
  int * temp=new int [n];
  int nbthreads= atoi(argv[2]);
  generateMergeSortData (arr, n);
  int chunkSize = n/nbthreads;
  omp_set_num_threads(nbthreads);

  //insert sorting code here.
#pragma omp parallel
{
 	#pragma omp single
	{
  		mergesort(arr, n, temp, chunkSize);
	}
}
  checkMergeSortResult (arr, n);
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  float elapsedTime = (end.tv_nsec - start.tv_nsec) / 1000000000.0 + (end.tv_sec - start.tv_sec);
  cerr << elapsedTime << endl;
  delete[] arr;
  delete[] temp;
  return 0;
}