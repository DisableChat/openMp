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


int get_vector_LR(vector<vector<int> > &M, int row, int column, int q)
{
  return(M[column][row]%q);
}

int get_vector_UD(vector<vector<int> > &M, int row, int column, int q)
{
  return(M[row][column]%q);
}

int get_vector_Diag(vector<vector<int> > &M, int row, int column, int q)
{
  return(M[row][column]%q);
}

int calculate_mod_product(int max, int num, int q)
{
    num = num%q;
    max = (max*num)%q;
    return max;
}
int determine_max(int max, int val)
{
  if(max <= val)
  {
    new_MAX = val;
    max = val;
  }
  else
  {
    max = max;
  }
  if (new_MAX <= max){
    new_MAX = max;
  }
  return max;
}

int get_diag_end_loc(vector<vector<int> > &M, int m)
{
  int index;
  index = M.size() - m;
  return index;
}

//******************************************************************************
// Function:      check_LR(vector<vector<int> > M, int m, int q, int index)
// Parameters:    vector<vector<int>>
//                int m, int q, int index
// Return value:
// Purpose:       to check values right of grid
//******************************************************************************
int check_LR(vector<vector<int> > &M, int m, int q, int index)
{
  int tmp_LR = 0;
  int ver    = 0;
  int max    = 1;
  int max1   = 1;

  int size = M.size() - m;

  for (int i = 0; i < size + 1;  i++)
  {
    for(int k = 0; k < m; k++)
    {
      tmp_LR = get_vector_LR(M, ver, index, q);
      max1 = calculate_mod_product(max1, tmp_LR, q);
      max = determine_max(max, max1);
      ver++;
    }
    ver = ver - m + 1;
    max1 = 1;
  }
  return max;
}

//******************************************************************************
// Function:      check_LR(vector<vector<int> > M, int m, int q, int index)
// Parameters:    vector<vector<int>>
//                int m, int q, int index
// Return value:
// Purpose:       to check values downward of grid
//******************************************************************************
int check_UD(vector<vector<int> > &M, int m, int q, int index)
{
  int tmp_UD = 0;
  int ver    = 0;
  int max    = 1;
  int max1   = 1;

  int size = M.size() - m;

  for (int i = 0; i < size + 1 ;  i++)
  {
    for(int k = 0; k < m; k++)
    {

      tmp_UD = get_vector_UD(M, ver, index, q);
      max1 = calculate_mod_product(max1, tmp_UD, q);
      max = determine_max(max, max1);
      ver++;
    }
    ver = ver - m + 1;
    max1 = 1;
  }
  return max;
}


//******************************************************************************
// Function:      check_diag_LR_Bottom(vector<vector<int> > M, int m, int q,
//                int num of times, int starting_loc)
// Parameters:    vector<vector<int>>
//                int m, int q, int num of times, in start loc
// Return value:
// Purpose:       to check values of the grid left to right (bottom half of grid)
//******************************************************************************
int check_diag_LR_Bottom(vector<vector<int> > &M, int m, int q, int num_of_times, int starting_loc)
{
  int tmp_size = M.size();
  int tmp_Diag_LR = 0;
  int LR_Bottom_Max = 1;
  int ver         = 0;
  int max         = 0;

  for(int i = 0; i < num_of_times; i++)
  {
    for(int k = 0; k < m; k++)
    {
      tmp_Diag_LR = get_vector_Diag(M, k + ver + starting_loc , k + ver, q);
      LR_Bottom_Max = calculate_mod_product(LR_Bottom_Max, tmp_Diag_LR, q);
      max = determine_max(max, LR_Bottom_Max);
    }
    ver++;
    LR_Bottom_Max = 1;
  }
  return max;
}


//******************************************************************************
// Function:      check_diag_LR_Top(vector<vector<int> > M, int m, int q,
//                int num of times, int starting_loc)
// Parameters:    vector<vector<int>>
//                int m, int q, int num of times, in start loc
// Return value:
// Purpose:       to check values of the grid left to right (top half of grid)
//******************************************************************************
int check_diag_LR_Top(vector<vector<int> > &M, int m, int q, int num_of_times, int starting_loc)
{
  int tmp_size = M.size();
  int tmp_Diag_LR_Top = 0;
  int LR_Top_Max = 1;
  int ver         = 0;
  int max         = 0;
  int total       = 0;

  for(int i = 0; i < num_of_times; i++)
  {
    for(int k = 0; k < m; k++)
    {
      tmp_Diag_LR_Top = get_vector_Diag(M, k + ver, k + ver + starting_loc, q);
      LR_Top_Max = calculate_mod_product(LR_Top_Max, tmp_Diag_LR_Top, q);
      max = determine_max(max, LR_Top_Max);
    }
    ver++;
    LR_Top_Max = 1;
  }
  return max;
}

//******************************************************************************
// Function:      check_diag_RL_Bottom(vector<vector<int> > M, int m, int q,
//                int num of times, int starting_loc, int starting_y)
// Parameters:    vector<vector<int>>
//                int m, int q, int num of times, in start loc, in starting_y
// Return value:
// Purpose:       to check values of the grid right to left (bottom half of grid)
//******************************************************************************
int check_diag_RL_Bot(vector<vector<int> > &M, int m, int q, int num_of_times, int starting_loc, int starting_y)
{
  int tmp_size = M.size();
  int tmp_Diag_RL_Bot = 0;
  int tmp_Diag_RL_Top = 0;
  int RL_Bottom_Max = 1;
  int ver         = 0;
  int max         = 1;
  int tmp        = 0;

  for(int i = 0; i < num_of_times; i++)
  {
    for(int k = 0; k < m; k++)
    {
      tmp_Diag_RL_Bot = get_vector_Diag(M, k + ver + starting_y , -k + -ver + starting_loc,  q);
      RL_Bottom_Max = calculate_mod_product(RL_Bottom_Max, tmp_Diag_RL_Bot, q);
      max = determine_max(tmp, RL_Bottom_Max);
    }
    ver++;
    RL_Bottom_Max = 1;
  }
  return max;
}


//******************************************************************************
// Function:      check_diag_RL_Bottom(vector<vector<int> > M, int m, int q,
//                int num of times, int starting_loc, int starting_y)
// Parameters:    vector<vector<int>>
//                int m, int q, int num of times, in start loc, in starting_y
// Return value:
// Purpose:       to check values of the grid right to left (top  half of grid)
//******************************************************************************
int check_diag_RL_Top(vector<vector<int> > &M, int m, int q, int num_of_times, int starting_loc, int starting_y)
{
  int tmp_size = M.size();
  int tmp_Diag_RL_Top = 0;
  int RL_Top_Max = 1;
  int ver         = 0;
  int max         = 0;

  for(int i = 0; i < num_of_times; i++)
  {

    for(int k = 0; k < m; k++)
    {
      tmp_Diag_RL_Top = get_vector_Diag(M, k + ver + (starting_y), -k + -ver + starting_loc, q);
      RL_Top_Max = calculate_mod_product(RL_Top_Max, tmp_Diag_RL_Top, q);
      max = determine_max(max, RL_Top_Max);

    }
    ver++;
    RL_Top_Max = 1;
  }
  return max;
}


// Main just calls the loops and functions nessary to run the program
int main() {
  int max   = 1;
  int times = 0;
  int tmp = 0;
  int num_of_times = 0;

  int tmp_LR = 1;
  int tmp_UD = 1;

  int tmp_Diag_LR_Top = 0;
  int tmp_Diag_LR_Bot = 0;
  int tmp_Diag_RL_Bot_ = 0;
  int tmp_Diag_RL_Top_ = 0;

  int q;
  int m;
  vector<vector<int> > M;
  m = read_m(cin);
  q = read_q(cin);
  string line;
  getline(cin,line);
  assert(line == "");

  M = read_M(cin);
  //cout << M.size() << endl;

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

  //#pragma omp parallel

  // this calulates the going in the rightward direction
  for(int index = 0; index < M.size(); index++)
  {
    tmp_LR = check_LR(M, m, q, index);
    max = determine_max(max, tmp_LR);
  }

  // this calulates the going in the downward direction
  for(int index = 0; index < M.size(); index++)
  {
    tmp_UD = check_UD(M, m, q, index);
    max = determine_max(max, tmp_UD);
  }


  times = (M.size() - 1)/2;
  num_of_times = M.size() - m + 1;

  // this calulates the going in the left to right downward direction
  // its also only the bottom half
  for(int i = 0; i < times; i++)
  {
    tmp = num_of_times - i;
    tmp_Diag_LR_Bot = check_diag_LR_Bottom(M, m, q, tmp, i);
    max = determine_max(max, tmp_Diag_LR_Bot);
  }

  // this calulates the going in the left to right downward direction
  // its also only the top half
  for(int i = 1; i < times; i++)
  {
    tmp = num_of_times - i;
    tmp_Diag_LR_Top = check_diag_LR_Top(M, m, q, tmp, i);
    max = determine_max(max, tmp_Diag_LR_Top);
  }

  int starting_y = 0;
  // this calulates the going in the right to left downward direction
  // its also only the top half
  for(int i = 0; i < times ; i++)
  {
    tmp = num_of_times - i;
    int tmp2 = (M.size() -1);
    tmp_Diag_RL_Top_ = check_diag_RL_Top(M, m, q, tmp, tmp2 - i, starting_y);
    max = determine_max(max, tmp_Diag_RL_Top_);

  }

  starting_y = 0;
  // this calulates the going in the right to left downward direction
  // its also only the
  for(int i = 0; i < times ; i++)
  {
    tmp = num_of_times - i;
    int tmp2 = (M.size() -1);
    tmp_Diag_RL_Bot_ = check_diag_RL_Bot(M, m, q, tmp, tmp2 - i, starting_y);
    max = determine_max(max, tmp_Diag_RL_Bot_);
  }

  cout << "new max " << new_MAX << endl;
  cout << max << endl << "done" << endl;

}
