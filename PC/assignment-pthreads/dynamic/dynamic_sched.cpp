#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
using namespace std;
pthread_mutex_t  mut,get; 
float result = 0.0;

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
float a,b;
int n;
int functionid, nbthreads, intensity, granularity;
string sync;
int begin1=0;
int end1 = 0;

int pointer;

int done()
{
pthread_mutex_lock (&get);
if (pointer == 0)
{
	pthread_mutex_unlock (&get);
	return 1;	
}

else
{
	if(pointer >= granularity)
	{
		pointer -= granularity;
	}
	else
	{
		pointer = 0;
	}
pthread_mutex_unlock (&get);
return 0;
}
}

void getnext(int *lowerbound, int *upperbound)
{
pthread_mutex_lock (&get);
*lowerbound = begin1;
*upperbound = end1;
if((n-end1) >= granularity)
{
begin1 = *upperbound;
end1 = begin1 + granularity;
}
pthread_mutex_unlock (&get);
}


void *iteration(void *p)
{
  
  while(done() == 0){
   	float x=0.0;
	int lowerbound, upperbound;
	getnext(&lowerbound,&upperbound);
        if( functionid == 1)
        {
            for(int i = lowerbound ; i < upperbound ; i++ )
            {      
                     pthread_mutex_lock (&mut);
                     x = a + (i + 0.5)*((b-a)/n);
                     result += f1(x,intensity);
                     pthread_mutex_unlock (&mut);
            }
        }
        else if( functionid == 2)
        {
            for(int i = lowerbound ; i < upperbound ; i++ )
            {
                    pthread_mutex_lock (&mut);
                    x = a + (i + 0.5)*((b-a)/n);
                    result += f2(x,intensity);
                    pthread_mutex_unlock (&mut);
            }    
        }
        else if( functionid == 3)
         {
            for(int i = lowerbound ; i < upperbound ; i++ )
            {
                    pthread_mutex_lock (&mut);
                    x = a + (i + 0.5)*((b-a)/n);
                    result += f3(x,intensity);
                    pthread_mutex_unlock (&mut);
            }
        }
        else if( functionid == 4)
        {
            for(int i = lowerbound ; i < upperbound ; i++ )
            {
                    pthread_mutex_lock (&mut);
                    x = a + (i + 0.5)*((b-a)/n);
                    result += f4(x,intensity);
                    pthread_mutex_unlock (&mut);
                }
        }
}
}

void *chunk(void *p)
{
      	while(done() == 0)//!done())
	{
	float temp=0.0;
	float x=0.0;
	int lowerbound, upperbound;
	getnext(&lowerbound,&upperbound); 
        if( functionid == 1)
        {
            for( int i = lowerbound ; i < upperbound; i++ )
            {
		    x = a + (i + 0.5)*((b-a)/n);
                    temp += f1(x,intensity);  
            }    
	  }
          else if( functionid == 2)
          {
            for( int i = lowerbound ; i < upperbound; i++ )
            {
                    x = a + (i + 0.5)*((b-a)/n);
                    temp += f2(x,intensity);  
             }
          } 
          else if(functionid == 3)
          {
            for(int i = lowerbound ; i < upperbound; i++ )
            {
                    x = a + (i + 0.5)*((b-a)/n);
                    temp += f3(x,intensity);  
            }
          }    
          else if( functionid == 4)
          {
            for( int i = lowerbound ; i < upperbound; i++ )
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

void *thread(void *p)
{
float value=0.0;
  while(done() == 0){
	float x=0.0;
	
	int lowerbound, upperbound;
	getnext(&lowerbound, &upperbound);
        if( functionid == 1)
        {
            for(int i = lowerbound ; i < upperbound; i++ )
            {
                    x = a + (i + 0.5)*((b-a)/n);
                    value += f1(x,intensity);  
            }
          }    
          else if( functionid == 2)
          {
            for(int i = lowerbound ; i < upperbound; i++ )
            {
                    x = a + (i + 0.5)*((b-a)/n);
                    value += f2(x,intensity);  
               }
          } 
          else if(functionid == 3)
          {
            for(int i = lowerbound ; i < upperbound; i++ )
            {
                    x = a + (i + 0.5)*((b-a)/n);
                    value += f3(x,intensity);  
            }
          }    
          else if( functionid == 4)
          {
            for(int i = lowerbound ; i < upperbound; i++ )
            {
                    x = a + (i + 0.5)*((b-a)/n);
                     value += f4(x,intensity);  
            }
          }

}

   	pthread_mutex_lock (&mut);
   	result += value;
   	pthread_mutex_unlock (&mut);
}


void* execute_inner_loop(void *p)
{
  void* val = (void *)p;

  if ( sync == "iteration")
  {
    iteration(val);
  }
  else if( sync == "chunk")
  {
    chunk(val);
  }
  else if( sync == "thread")
  {
    thread(val);
  }
}

int main (int argc, char* argv[]) {
  auto start = chrono::steady_clock::now();
  if (argc < 9) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
    return -1;
  }
  
  pthread_mutex_init(&mut,NULL);
  pthread_mutex_init(&get,NULL);
 
  functionid = stoi(argv[1]);
  a = stof(argv[2]);
  b = stof(argv[3]);
  n = stof(argv[4]);
  intensity = stof(argv[5]);
  nbthreads = stoi(argv[6]);
  sync = argv[7];
  granularity = stoi(argv[8]);
  pthread_t th[nbthreads];
 
 pointer = n;
  
 end1= granularity;
  int k=0;
  for (int i=0; i < nbthreads; ++i)
  {
      pthread_create( &th[i], NULL, execute_inner_loop, (void *)&k);
  }
  for (int i=0; i < nbthreads; ++i)
  {
      pthread_join(th[i], NULL);
  }
  pthread_mutex_destroy(&mut);
  pthread_mutex_destroy(&get);
 
  result = result*((b-a)/n);
  cout<<result;
  
  auto endo = chrono::steady_clock::now();
  std::chrono::duration<double> elapsed = endo - start;
  cerr<<elapsed.count();
  return 0;
}
	
	
	

            	