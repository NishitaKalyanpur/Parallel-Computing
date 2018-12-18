#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
  void generateReduceData (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
 auto start = chrono::steady_clock::now();
  //forces openmp to create the threads beforehand
if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

float result=0.0;
int n = atoi(argv[1]);
int * arr = new int [n];
int nbthreads = atoi(argv[2]);
int granularity = atoi(argv[4]);
omp_set_num_threads(nbthreads);
if(argv[7] == "static")
{
    omp_set_schedule(omp_sched_static,granularity);
}
else if(argv[7] == "dynamic")
{
	omp_set_schedule(omp_sched_dynamic,granularity);
}
else if(argv[7] == "guided")
{
	omp_set_schedule(omp_sched_guided,granularity);
}
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  
  
  
  generateReduceData (arr, atoi(argv[1]));
  
  //insert reduction code here

 
  #pragma omp for reduction(+ :result) schedule(runtime)
  for (int i=0;i<n;i++)
  {
  	//printf("\n result: %f",result);
  	result+= arr[i];
  }

  
  }

  cout<<result;
  delete[] arr;
  auto end = chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cerr<<elapsed.count();
  return 0;
}