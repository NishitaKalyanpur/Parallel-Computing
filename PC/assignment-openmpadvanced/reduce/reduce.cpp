#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <string>
using namespace std;


#ifdef __cplusplus
extern "C" {
#endif

  void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
struct timespec start, end1;
clock_gettime(CLOCK_MONOTONIC_RAW, &start);

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
  
  if (argc < 3) {
    std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  int * arr = new int [n];
  int * p = new int [nbthreads];
  int result = 0;
  generateReduceData (arr, atoi(argv[1]));
  omp_set_dynamic(0);
  omp_set_num_threads(nbthreads);
  int chunk_size = n/nbthreads;

  if(nbthreads % 2 == 0) 
	chunk_size = n/nbthreads;
  else 
	chunk_size = (n/nbthreads)+1;

#pragma omp parallel 
{
	#pragma omp single 
  	for(int i = 0; i < nbthreads; i++)
	{
		int end = ((i+1) * chunk_size);
		if(i == nbthreads)
                {
			end = n;
		}
  		#pragma omp task
		{
 			for(int j = i*chunk_size ; j < end ; j++)
			{
				 p[i] += arr[j];
 				 if(j >= n-1) 
    			         break;
			}
		}
	}

for(int i = 0;i < nbthreads;i++)
	result += p[i];
}
 clock_gettime(CLOCK_MONOTONIC_RAW, &end1);
 float elapsedTime = (end1.tv_nsec - start.tv_nsec) / 1000000000.0 + (end1.tv_sec - start.tv_sec);
 cout <<endl<< result << endl;
 cerr << elapsedTime << endl;
 delete[] arr;
 return 0;
}
