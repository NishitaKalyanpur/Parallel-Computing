#include <iostream>
#include <chrono>

__global__ void polynomial_expansion (float* poly, int degree,
               int n, float* array) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if( i < n )
  {
    float result = 0.0;
    float xtothepowerof = 1.0;
    for ( int x = 0; x < degree; ++x)
    {
      result += xtothepowerof * poly[x];
      xtothepowerof *= array[i];
    }
    array[i] = result;
  }
}

int main (int argc, char* argv[])
{
  if (argc < 3)
  {
     std::cerr<<"usage: "<<argv[0]<<" n degree"<<std::endl;
     return -1;
  }

  int n = atoi(argv[1]);
  int degree = atoi(argv[2]);
  int nbiter = 1;

  float* array = new float[n];
  float* poly = new float[degree+1];

    std::chrono::time_point<std::chrono::system_clock> begin_time, end_time;

  for (int i=0; i<n; ++i)
    array[i] = 1.;

  for (int i=0; i<degree+1; ++i)
    poly[i] = 1.;

  float *d_array, *d_poly;

  
  cudaMalloc((void **)&d_array, n*sizeof(float));
  cudaMalloc((void **)&d_poly, (degree+1)*sizeof(float));
 
  cudaMemcpy(d_array, array, n*sizeof(float), cudaMemcpyHostToDevice);
  cudaMemcpy(d_poly, poly, n*sizeof(float), cudaMemcpyHostToDevice);
  

  begin_time = std::chrono::system_clock::now();

  for (int iter = 0; iter<nbiter; ++iter)
  polynomial_expansion<<<((n+255)/256) +1 , 256>>>(d_poly, degree, n, d_array);
  cudaMemcpy(array, d_array, n*sizeof(float), cudaMemcpyDeviceToHost);


  cudaDeviceSynchronize();

  end_time = std::chrono::system_clock::now();
  std::chrono::duration<double> totaltime = (end_time-begin_time)/nbiter;

  std::cout<<n<<" "<<degree<<" "<<totaltime.count()<<std::endl;

  delete[] array;
  delete[] poly;
 
  cudaFree(d_array);
  cudaFree(d_poly);

  return 0;
}