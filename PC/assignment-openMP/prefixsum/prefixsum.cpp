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
  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
auto start = chrono::steady_clock::now();
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }
 
  int n = atoi(argv[1]);
  int * arr = new int [n];
  generatePrefixSumData (arr, n);
  int nbthreads = atoi(argv[2]);
  omp_set_num_threads(nbthreads);
  int * pr = new int [n+1];
  int * index = new int [nbthreads];
  int * temp = new int [nbthreads];
  int pointer = 0;
  int granularity = n/nbthreads;
  pr[0] = 0;
  temp[0] = 0;
  index[0] = 0;

#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }

#pragma omp for schedule(static)
  for(int j = 0; j < nbthreads; j++){
    	for(int i = (j * granularity);i < ((j + 1) * granularity); i++){
      		if(i == (j * granularity)){
        		pr[i+1] = 0 + arr[i];
      		}
		else{
        		pr[i+1] = pr[i] + arr[i];
      		}
       pointer = i;
       temp[j + 1] = pr[i+1];  
       }
  index[j +1] = ((j +1) * granularity)+1;
  }
}

if(nbthreads * granularity <n){
        for(int i = nbthreads * granularity; i < n; i++){
            pr[i+1] = pr[i] + arr[i];
        }
      }

int * prefix = new int [nbthreads];
prefix[0] = temp[0];
for(int i = 1; i<nbthreads; i++){
    	prefix[i] = prefix[i-1] + temp[i];
  }

#pragma omp parallel for schedule(dynamic)
  for(int i = 0;i < nbthreads; i++){
    	for(int j = index[i]; j < index[i+1]; j++){
    		pr[j] = pr[j] + prefix[i];
    		pointer = j;
    }
  }


  if(nbthreads * granularity < n){
        for(int i = nbthreads * granularity; i < n; i++){
          	pr[i+1] = pr[i+1] + prefix[nbthreads-1];
        }
  }
  
checkPrefixSumResult(pr, n);
delete[] arr;

// end time
auto end = chrono::steady_clock::now();
std::chrono::duration<double> elapsed = end - start;
cerr<<elapsed.count();
return 0;
}
