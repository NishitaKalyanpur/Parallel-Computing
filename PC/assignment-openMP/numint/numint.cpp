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

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif



int main (int argc, char* argv[]) {

auto start = chrono::steady_clock::now();
  if (argc < 9) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  //forces openmp to create the threads beforehand
  int fid = atoi(argv[1]);
  float a = atof(argv[2]);
  float b = atof(argv[3]);
  int n = atoi(argv[4]);
  int intensity = atoi(argv[5]);
  int nbthreads = atoi(argv[6]);
  int granularity = atoi(argv[8]);
  float result = 0.0;

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

#pragma omp for reduction( + :result) schedule(runtime)
  	for(int i=0; i < n; i++)
  	{
	 	float x = a + (i + 0.5)*((b-a)/n);

     		if( fid == 1)
  		{
 			result += f1(x,intensity);
  			
  		}
  		else if( fid == 2)
  		{
      			result += f2(x,intensity);
  		}
   		else if( fid == 3)
    		{
      			result += f3(x,intensity);
  		}
  		else if( fid == 4)
    		{
      			result += f4(x,intensity);
  		}

}
}
result = result * ((b-a)/n);
  cout<<result;
  auto end = chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cerr<<elapsed.count();

  return 0;
}
