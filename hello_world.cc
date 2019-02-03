// Wesley Ryder
// OpenMP
#include <omp.h>
#include <iostream>

using namespace std;


void test()
{
  #pragma omp critical
  {
    cout << "Hello from thread " << omp_get_thread_num() << endl;
    cout << "This is Wes Ryder's first parallel program" << endl;
    cout << "There are currently "
    <<  omp_get_num_threads() <<  " threads running\n" << endl;
  }
}

int main()
{
#pragma omp parallel
  test();
}
