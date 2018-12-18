#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <bits/stdc++.h>
#include <string.h>
#include  <omp.h>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

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
  int nbthreads = atoi(argv[2]);
  omp_set_num_threads(nbthreads);
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  //insert sorting code here.
		
for(  int i = 0;  i < n;  i++ )
{       
int pos = i % 2;      
#pragma omp parallel for default(none),shared(arr,pos,n)
for(int j = pos;j < n-1;j += 2)
{       
	if(arr[j] > arr[j+1] )
	{       
		swap(arr[j],arr[j+1]);
	}       
}       

}
  checkMergeSortResult (arr, n);
  
  delete[] arr;
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  float elapsedTime = (end.tv_nsec - start.tv_nsec) / 1000000000.0 + (end.tv_sec - start.tv_sec);
  cerr << elapsedTime << endl;
  return 0;
}
