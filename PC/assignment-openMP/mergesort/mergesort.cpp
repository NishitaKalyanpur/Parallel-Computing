#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

int main (int argc, char* argv[]) {
auto start = chrono::steady_clock::now();
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
 
  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
 
  // get arr data
  int * arr = new int [n];
  int * temp = new int [n];
  generateMergeSortData (arr, n);
  int rght, wid, rend;
  int i,j,m, k = 0;
//#pragma omp for schedule(static)
#pragma omp parallel
  {

    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
#pragma omp for reduction(*:k) schedule(static, 1)

     for(int t = 0; t <nbthreads; t++){
     while(k<n){     
        for (int left=0; left+k < n; left += k*2 ) {
            rght = left + k;       
            rend = rght + k;
            if (rend > n)
		 rend = n;
            m = left;
	    i = left;
	    j = rght;
            while (i < rght && j < rend) {
                if (arr[i] <= arr[j]) {        
                    temp[m] = arr[i]; 
		    i++;
                } 
		else {
                    temp[m] = arr[j];
		    j++;
                }
                m++;
            }
            while (i < rght) {
                temp[m]=arr[i];
                i++; 
       		m++;
            }
            while (j < rend) {
                temp[m]=arr[j];
                j++; 
	        m++;
            }
            for (m=left; m < rend; m++) {
                arr[m] = temp[m];
            }
        }

   k = k * 2;
}
   
}
} 
  checkMergeSortResult (arr, n);
 
  delete[] arr;
  // end time
  auto end = chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cerr<<elapsed.count();
  return 0;
}

