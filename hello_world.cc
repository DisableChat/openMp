////////////////////////////////////////////////////////////////////////////////
// Wesley Ryder
// OpenMP HW Part #1
// 2-2-19
////////////////////////////////////////////////////////////////////////////////

#include <omp.h>
#include <iostream>
using namespace std;

//******************************************************************************
// Function:    test()
// Parameters:  None
// Description: The purpose of this function is to print off the thread along
//              with the totall number of threads running
//******************************************************************************
void test()
{
  // restricting the execution of the block to single thread at a time
  #pragma omp critical
  {
    cout << "Hello from thread " << omp_get_thread_num() << endl;
    cout << "This is Wes Ryder's first parallel program" << endl;
    cout << "There are currently "
    <<  omp_get_num_threads() <<  " threads running\n" << endl;
  }
}

//******************************************************************************
// Function:    main()
// Parameters:  None
// Description: The purpose of this function is to run the main of the program
//******************************************************************************
int main()
{
  //build parallel program that partition solution space
  #pragma omp parallel
    test();
}
