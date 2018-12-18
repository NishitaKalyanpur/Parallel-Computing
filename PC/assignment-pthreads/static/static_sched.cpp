#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <pthread.h>
using namespace std;
pthread_mutex_t  mut; 

#ifdef __cplusplus
extern "C" {
#endif
float result = 0;
float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

struct ReturnValue
{
    float a,b,n,x;
    int functionid, nbthreads, intensity;
    string sync;
    int lowerbound,upperbound;
    float v;
};

void* f(void* p)
{
  struct ReturnValue* s =(ReturnValue*)p;
  float a=s->a;
  float b=s->b;
  float n=s->n;
  float x;
  int nbthreads=s->nbthreads;
  int functionid=s->functionid;
  int intensity=s->intensity;
  string sync=s->sync;
  int i=0;
  int lowerbound=s->lowerbound;
  int upperbound=s->upperbound;
  int v = s->v;
 
  if ( sync == "iteration")
  {
    if( functionid == 1)
    {
        for(i=lowerbound;i<=upperbound;i++)
        {
             pthread_mutex_lock (&mut);
             x = a + (i + 0.5)*((b-a)/n);
             result += f1(x,intensity);
             pthread_mutex_unlock (&mut);
        }
    }
    else if( functionid == 2)
    {
        for(i=lowerbound;i<=upperbound;i++)
        {
            pthread_mutex_lock (&mut);
            x = a + (i + 0.5)*((b-a)/n);
            result += f2(x,intensity);
            pthread_mutex_unlock (&mut);
        }	
    }
    else if( functionid == 3)
     {
        for(i=lowerbound;i<=upperbound;i++)
        {
            pthread_mutex_lock (&mut);
            x = a + (i + 0.5)*((b-a)/n);
            result += f3(x,intensity);
            pthread_mutex_unlock (&mut);
        }
    }
    else if( functionid == 4)
    {
        for(i=lowerbound;i<=upperbound;i++)
        {
            pthread_mutex_lock (&mut);
            x = a + (i + 0.5)*((b-a)/n);
            result += f4(x,intensity);
            pthread_mutex_unlock (&mut);
        }
    }
  }
  else if(sync=="thread")
  {
      float temp;
      if( functionid == 1)
      {
        for(i=lowerbound;i<=upperbound;i++)
        {
            x = a + (i + 0.5)*((b-a)/n);
            temp += f1(x,intensity);  
        }
      }    
      if( functionid == 2)
      {
        for(i=lowerbound;i<=upperbound;i++)
        {
            x = a + (i + 0.5)*((b-a)/n);
            temp += f2(x,intensity);  
         }
      } 
      if(functionid == 3)
      {
        for(i=lowerbound;i<=upperbound;i++)
        {
            x = a + (i + 0.5)*((b-a)/n);
            temp += f3(x,intensity);  
        }
      }    
      if( functionid == 4)
      {
        for(i=lowerbound;i<=upperbound;i++)
        {
            x = a + (i + 0.5)*((b-a)/n);
            temp += f4(x,intensity);  
        }
      }
  pthread_mutex_lock (&mut);  
  result += temp;
  pthread_mutex_unlock (&mut);
  }
}

int main (int argc, char* argv[]) {
  auto start = chrono::steady_clock::now();

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<std::endl;
    return -1;
  }
  pthread_mutex_init(&mut,NULL);
  int nbthreads = stof(argv[6]);
  float a = stof(argv[2]);
  float b = stof(argv[3]);
  struct ReturnValue s[nbthreads];
  pthread_t th[nbthreads];
  float n = stof(argv[4]);
  float v = n/nbthreads;
  float upperbound = v-1;
  float lowerbound = 0;
  for (int i=0; i < nbthreads; ++i)
  {
      s[i].functionid = stoi(argv[1]);
      s[i].a = stof(argv[2]);
      s[i].b = stof(argv[3]);
      s[i].n = stof(argv[4]);
      s[i].intensity = stof(argv[5]);
      s[i].nbthreads = stof(argv[6]);
      s[i].sync = argv[7];
      s[i].lowerbound = lowerbound;
      s[i].upperbound = upperbound;
      s[i].v = v;
      pthread_create( &th[i], NULL, f, &s[i]);
      lowerbound = upperbound + 1;      
      upperbound = lowerbound + (v-1);
  }
  for (int i=0; i < nbthreads; ++i)
  {
      pthread_join(th[i], NULL);
  }
  pthread_mutex_destroy(&mut);

  result = result*((b-a)/n);
  cout<<result;
  auto end = chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cerr<<elapsed.count();
  return 0;
}

