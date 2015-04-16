#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

double y(double);

int main()
{
  int nthreads, tid;
  double leftRectanglesResult = 0;
  double step = 0.001;
  double start = 2.0;
  double end = 3.0;
  int iterationsAmount = (int)(end - start) / step;
  clock_t startTime = clock();
  
  int nProcessors=omp_get_max_threads();

  printf("max: %d\n", nProcessors);
  omp_set_num_threads(nProcessors);
  
  #pragma omp parallel for reduction(+:leftRectanglesResult)
  for(int i = 0; i < iterationsAmount; ++i) {
    // tid = omp_get_thread_num();
    // printf("thread %d\n", tid);
    // printf("setted max: %d\n", omp_get_num_threads());

    leftRectanglesResult += y(start + i*step) * step;
  }
  clock_t endTime = clock();
  float seconds = (float)(endTime - startTime) / CLOCKS_PER_SEC;

  printf("%f\n", leftRectanglesResult);
  printf("Time: %f\n", seconds);

  return 0;
}

double y(double x)
{
  return pow(x, 3.0) / sqrt(pow(pow(x, 2.0) + 9.0, 3.0));
}