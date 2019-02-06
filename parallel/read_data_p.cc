//********************************************************
//File:read_data.cc
//Author: Wesley Ryder
//
//
//Purpose: This example program reads the data for homework #1
// It reads m q
// and then, a n x n matrix of integers
// This code is not elegant, but it is sufficient for the
// purpose of this class.
// the begining of this code was given to us by professor Juedes
//***********************************************************
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cassert>
#include <omp.h>
using namespace std;

  int new_MAX   = 1;

//***************************************************************
// Function read_q(istream &in)
// Parameters: istream in ---
// Return value: q
// Purpose: Read the value q (the modulus for the assigment)
//****************************************************************
int read_q(istream &in) {
  int q;
  in >> q;
  return q;
}
//***************************************************************
// Function read_m(istream &in)
// Parameters: istream in ---
// Return value: m
// Purpose: Read the value m (the number of values in the product)
//****************************************************************
int read_m(istream &in) {
  int m;
  in >> m;
  return m;
}
//***************************************************************
// Function read_M(istream &in)
// Parameters: istream in ---
// Return value: vector<vector<int> > M
// Purpose: Read the value M, the matrix, which exists to the end of the
// input.
// Assumption: each row of the matrix is stored on a line by itself.
//****************************************************************
vector<vector<int> > read_M(istream &in) {
  string line;
  vector<vector<int> > M;
  while (!in.eof()) {
    getline(in,line);
    if (!in.fail()) {
      istringstream in1(line);
      vector<int> row;
      while (!in1.eof()) {
	int x;
	in1 >> x;
	if (!in1.fail()) {
	  row.push_back(x);
	}
      }
      M.push_back(row);
    }
  }
  return M;
}


//******************************************************************************
// Function:      get_vector()
// Parameters:    vector<vector<int> >
//                int row
//                int column
//                int q, modulus value
// Return value:  that value%q at given index
// Purpose:       to retrieve that value at specified grid location
//******************************************************************************
int get_vector(vector<vector<int> > &M, int row, int column, int q)
{
  return(M[row][column]%q);
}

//******************************************************************************
// Function:      get_vector_Diag()
// Parameters:    vector<vector<int> >
//                int row
//                int column
//                int q, modulus value
// Return value:  that value%q at given index
// Purpose:       to retrieve that value at specified grid location
//******************************************************************************
int get_vector_Diag(vector<vector<int> > &M, int row, int column, int q)
{
  return(M[row][column]%q);
}

//******************************************************************************
// Function:      calculate_mod_product
// Parameters:    vector<vector<int> >
//                int max
//                int num
//                int q, modulus value
// Return value:  max%q
// Purpose:       to return the the product of two ints modded by q
//******************************************************************************
int calculate_mod_product(int max, int num, int q)
{
    num = num%q;
    max = (max*num)%q;
    return max%q;
}

//******************************************************************************
// Function:      determine_max
// Parameters:    int max
//                int val
// Return value:  max
// Purpose:       to update max value if nessary
//******************************************************************************
int determine_max(int max, int val)
{
  if(max <= val)
  {
    max = val;
  }
  else
  {
    max = max;
  }
  return max;
}

//******************************************************************************
// Function:      check_LR(vector<vector<int> > M, int m, int q, int index)
// Parameters:    vector<vector<int>>
//                int m, which is number of numbers to be multiplied
//                int q, which is the modulus
// Return value:  max product
// Purpose:       to check values right of grid
//******************************************************************************
int check_LR(vector<vector<int> > &M, int m, int q)
{
  int tmp_LR = 0;
  int max    = 1;
  int max1   = 1;

  for(int z = 0; z < M.size(); z++)
  {
    for (int i = 0; i < M.size() - m + 1 ;  i++)
    {
      for(int k = 0; k < m ; k++)
      {
        tmp_LR = get_vector(M, z, i + k, q);
        max1 = calculate_mod_product(max1, tmp_LR, q);
      }
      max = determine_max(max, max1);
      max1 = 1;
    }
  }
  return max;
}

//******************************************************************************
// Function:      check_LR(vector<vector<int> > M, int m, int q, int index)
// Parameters:    vector<vector<int>>
//                int m, which is number of numbers to be multiplied
//                int q, which is the modulus
// Return value:  max product
// Purpose:       to check values downward of grid
//******************************************************************************
int check_UD(vector<vector<int> > &M, int m, int q)
{
  int tmp_UD = 0;
  int max    = 1;
  int max1   = 1;

  for(int z = 0; z < M.size(); z++)
  {
    for (int i = 0; i < M.size() - m + 1;  i++)
    {
      for(int k = 0; k < m; k++)
      {
        tmp_UD = get_vector(M, i + k, z, q);
        max1 = calculate_mod_product(max1, tmp_UD, q);
      }
      max = determine_max(max, max1);
      max1 = 1;
    }
  }
  return max;
}


//******************************************************************************
// Function:      check_diag_LR_Bottom(vector<vector<int> > M, int m, int q,
//                int num of times, int starting_loc)
// Parameters:    vector<vector<int>>
//                int m, which is number of numbers to be multiplied
//                int q, which is the modulus
// Return value:  max product
// Purpose:       to check values of the grid left to right (bottom half of grid)
//******************************************************************************
int check_diag_LR_Bottom(vector<vector<int> > &M, int m, int q)
{
  int tmp_Diag_LR = 0;
  int LR_Bottom_Max = 1;
  int max         = 0;
  int bounds      = 0;

  for(int z = 0; z < M.size()/2; z++)
  {
    for(int i = 0; i < M.size() - m + 1 - z; i++)
    {
      for(int k = 0; k < m; k++)
      {
        tmp_Diag_LR = get_vector_Diag(M, k + i  + z , k + i, q);
        LR_Bottom_Max = calculate_mod_product(LR_Bottom_Max, tmp_Diag_LR, q);
      }
      max = determine_max(max, LR_Bottom_Max);
      LR_Bottom_Max = 1;
    }
  }
  return max;
}


//******************************************************************************
// Function:      check_diag_LR_Top(vector<vector<int> > M, int m, int q,
//                int num of times, int starting_loc)
// Parameters:    vector<vector<int>>
//                int m, which is number of numbers to be multiplied
//                int q, which is the modulus
// Return value:  max product
// Purpose:       to check values of the grid left to right (top half of grid)
//******************************************************************************
int check_diag_LR_Top(vector<vector<int> > &M, int m, int q)
{
  int tmp_Diag_LR = 0;
  int LR_Top_Max = 1;
  int max         = 0;
  int total       = 0;

  for(int z = 0; z < M.size()/2; z++)
  {
    for(int i = 0; i < M.size() - m + 1 - z; i++)
    {
      for(int k = 0; k < m; k++)
      {
        tmp_Diag_LR = get_vector_Diag(M, k + i , k + i + z , q);
        LR_Top_Max = calculate_mod_product(LR_Top_Max, tmp_Diag_LR, q);
      }
      max = determine_max(max, LR_Top_Max);
      LR_Top_Max = 1;
    }
  }
  return max;
}

//******************************************************************************
// Function:      check_diag_RL_Bottom(vector<vector<int> > M, int m, int q,
//                int num of times, int starting_loc, int starting_y)
// Parameters:    vector<vector<int>>
//                int m, which is number of numbers to be multiplied
//                int q, which is the modulus
// Return value:  max product
// Purpose:       to check values of the grid right to left (bottom half of grid)
//******************************************************************************
int check_diag_RL_Bot(vector<vector<int> > &M, int m, int q)
{
  int tmp_Diag_RL_Bot = 0;
  int RL_Bottom_Max = 1;
  int max         = 1;
  int starting_y = 0;

  for(int z = 0; z < M.size()/2; z++)
  {
    for(int i = 0; i < M.size() - m + 1 - z; i++)
    {
      for(int k = 0; k < m; k++)
      {
        tmp_Diag_RL_Bot = get_vector_Diag(M, k + i + z, M.size() - 1 -k + -i,  q);
        RL_Bottom_Max = calculate_mod_product(RL_Bottom_Max, tmp_Diag_RL_Bot, q);
      }
      max = determine_max(max, RL_Bottom_Max);
      RL_Bottom_Max = 1;
    }
  }
  return max;
}


//******************************************************************************
// Function:      check_diag_RL_Bottom()
// Parameters:    vector<vector<int>>
//                int m, which is number of numbers to be multiplied
//                int q, which is the modulus
// Return value:  max product
// Purpose:       to check values of the grid right to left (top  half of grid)
//******************************************************************************
int check_diag_RL_Top(vector<vector<int> > &M, int m, int q)
{
  int tmp_Diag_RL_Top = 0;
  int RL_Top_Max = 1;
  int max         = 1;
  int starting_y = 0;

  for(int z = 0; z < M.size()/2; z++)
  {
    for(int i = 0; i < M.size() - m + 1 - z; i++)
    {
      for(int k = 0; k < m; k++)
      {
        tmp_Diag_RL_Top = get_vector_Diag(M, k + i, M.size() - 1 -k + -i - z,  q);
        RL_Top_Max = calculate_mod_product(RL_Top_Max, tmp_Diag_RL_Top, q);
      }
      max = determine_max(max, RL_Top_Max);
      RL_Top_Max = 1;
    }
  }
  return max;
}


// Main just calls the loops and functions nessary to run the program
int main() {
  // max value
  int max   = 1;

  // Tmp store values for each function call
  int tmp_LR = 1;
  int tmp_UD = 1;
  int tmp_Diag_LR_Top = 0;
  int tmp_Diag_LR_Bot = 0;
  int tmp_Diag_RL_Bot = 0;
  int tmp_Diag_RL_Top = 0;

  int q;
  int m;
  vector<vector<int> > M;
  m = read_m(cin);
  q = read_q(cin);
  string line;
  getline(cin,line);
  assert(line == "");

  M = read_M(cin);

  // This for Loop prints the grid
  /*
  for (int i = 0; i < M.size(); i++)
  {
    for(int j = 0; j < M.size(); j++)
    {
      cout << M[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
  */

  // checking all directions of the matrix
  # pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      tmp_LR = check_LR(M, m, q);
      #pragma omp section
      tmp_UD = check_UD(M, m, q);
      #pragma omp section
      tmp_Diag_LR_Bot = check_diag_LR_Bottom(M, m, q);
      #pragma omp section
      tmp_Diag_LR_Top = check_diag_LR_Top(M, m, q);
      #pragma omp section
      tmp_Diag_RL_Bot = check_diag_RL_Bot(M, m, q);
      #pragma omp section
      tmp_Diag_RL_Top = check_diag_RL_Top(M, m, q);
    }
  }


  max = determine_max(max, tmp_LR);
  max = determine_max(max, tmp_UD);
  max = determine_max(max, tmp_Diag_LR_Bot);
  max = determine_max(max, tmp_Diag_LR_Top);
  max = determine_max(max, tmp_Diag_RL_Bot);
  max = determine_max(max, tmp_Diag_RL_Top);


  cout << max << endl << "done" << endl;

}
