#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <string>
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

  if (argc < 6) {
    std::cerr<<"usage: "<<argv[1]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  float a,b,n,intensity,result=0,x,r;
  int c;
  c = stoi(argv[1]);
  a = stof(argv[2]);
  b = stof(argv[3]);
  n = stof(argv[4]);
  intensity = stof(argv[5]);
  r = ((b-a)/n);
  int i=0;
  if( c == 1)
  {
  for( i=0; i <= n-1; i++)
  {
      x = a + (i + 0.5)*r;
      result += f1(x,intensity);
  }
  }
  else if( c == 2)
  {
  for( i=0; i <= n-1; i++)
  {
      x = a + (i + 0.5)*r;
      result += f2(x,intensity);
  }
  }
   else if( c == 3)
    {
  for( i=0; i <= n-1; i++)
  {
      x = a + (i + 0.5)*r;
      result += f3(x,intensity);
  }
  }
  else if( c == 4)
    {
  for( i=0; i <= n-1; i++)
  {
      x = a + (i + 0.5)*r;
      result += f4(x,intensity);
  }
  }
  result = result * r;
  cout<<result;
  auto end = chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cerr<<elapsed.count();

return 0;
}
