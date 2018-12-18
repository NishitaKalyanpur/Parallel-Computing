#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <vector>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

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

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
      return -1;
    }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthreads=atoi(argv[3]);
  omp_set_num_threads(nbthreads);
  char *X = new char[m];
  char *Y = new char[n];
  int** C = new int*[m+1];
  int result;
  generateLCS(X, m, Y, n);
  for (int i=0; i<=m; i++) 
    {
      C[i] = new int[n+1];
    }

  #pragma omp parallel
  {
  #pragma omp parallel for
  	for (int i=0; i<=m; i++) 
	{
      		C[i][0] = 0;
    	}
	#pragma omp parallel for
    	for (int j=1; j<=n; j++) 
	{
      		C[0][j] = 0;
    	}

    	for (int a=1; a<=m; a++) 
	{
	#pragma omp parallel for
      		for (int b=1; b<=n; b++) 
		{
			if (X[a-1] == Y[b-1]) 
			{
          			C[a][b] = C[a-1][b-1] + 1;
        		}
        		else 
			{
				C[a][b] = max(C[a][b-1], C[a-1][b]);
        		}
      		}

    	}
  }
  result=C[m][n];
  checkLCS(X, m, Y, n, result);
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  float elapsedTime = (end.tv_nsec - start.tv_nsec) / 1000000000.0 + (end.tv_sec - start.tv_sec);
  cerr << elapsedTime << endl;
  delete[] X;
  delete[] Y;
  return 0;
}