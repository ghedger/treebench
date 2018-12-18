// treebench.cc
//
// This file is part of treebench.
//
// treebench is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// treebench is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with treebench.  If not, see <https://www.gnu.org/licenses/>.
//

// C headers
#include <stdio.h>
#include <math.h>

// C++ headers
#include <iostream>
#include <map>
#include <vector>
#include <chrono>

// Project-specific
#include "common.h"
#include "algo.h"
#include "bstree.h"
#include "scapegoat_tree.h"

// PrintUsage
//
// Present the user with the usage instructions
void PrintUsage()
{
  printf("treebench\n" );
  printf("Usage:\n" );
  printf("\ttreebench <array_size>\n");
}

// PrintArray
//
// Prints n values of an array
//
// Entry: pointer
//        number of items to print
// Exit:  -
void PrintArray(const hedger::S_T *array, size_t n)
{
  for(size_t i = 0; i < n; i++) {
    printf("%04x\t", array[i]);
  }
  printf("\n");
}

// AllocArray
// Entry: size of array in elements
// Exit:  pointer to array, or nullptr == error
hedger::S_T * AllocArray(size_t size)
{
  // Allocate the array
  hedger::S_T *array = new hedger::S_T[ size ];
  if (!array) {
    // TODO: SEND TO LOGGER
    printf(" %s: Allocation error", __FUNCTION__);
    return nullptr;
  }
  return array;
}

// FreeArray
// Entry: pointer to array
void FreeArray(hedger::S_T *array)
{
  if (array) {
    delete array;
  }
}

// CreateDataSet
//
// Allocates and fills an array with unique pseudo-random values.
//
// Entry: size
// Exit:  pointer to array
hedger::S_T * CreateUniqueDataSet(hedger::S_T *array, size_t size)
{
  // In-situ STL map<> for quick lookup of already-used random values
  std::map<size_t, hedger::S_T> occupancy;

  size_t index = 0;
  hedger::S_T value = 0;
  // Generate array of scrambled unsorted but unique data in the array
  while(index < size) {
    // Get random number; if used, go fish again...
    do {
      value = rand() % size;
    } while (occupancy.find(value) != occupancy.end());
    occupancy[value] = 1;
    array[index] = value;
    ++index;
  }

  return array;
}

// Test
//
// Run the test on the Algo-derived search algorithm object
//
// Entry: pointer to algorithm object
//        pointer to array
//        size of array
int Test(hedger::Algo *o, hedger::S_T *arr, size_t size)
{
  int result = o->Test(arr, size);
  return result;
}

// TestBSTree()
//
// Test a bstree
//
// Entry: -
// Exit:  -
void TestBtree(size_t array_size)
{
  hedger::ScapegoatTree bstree;
  hedger::S_T *array = AllocArray(array_size);
  if (array) {
    CreateUniqueDataSet(array, array_size);

    if (!array) {
      // TODO: LOG ERROR
      return;
    }

    for (size_t i = 0; i < array_size; i++) {
      bstree.Add(array[i]);
    }

    bstree.Print();
    int maxDepth = bstree.MaxDepth();
    printf( "\nMAX DEPTH: %d\n", maxDepth);

    for (hedger::S_T i = 0; i < (hedger::S_T) array_size; i++)
    {
      hedger::Node *node = bstree.Find(i);
      printf("FIND: %08x\t", (int) (*(int *)node) );
    }

    printf("\n");
    bstree.DeleteKey(17);

    for (hedger::S_T i = 0; i < (hedger::S_T) array_size; i++)
    {
      hedger::Node *node = bstree.Find(i);
      printf("FIND: %p\t", node);
    }

    printf("\n");
    FreeArray(array);
  } else {
    printf("%s:%d Error allocating array.\n", __FUNCTION__, __LINE__);
  }
  return;
}

// ReportTiming
// Calculate and report mean and standard deviation of timing.
// Entry: vector of times
//        name of algorithm
// TODO: Move this to its own utility class
void ReportTiming(std::vector<double>& v, int iteration_tot, const char *name)
{
    // Calculate average (mu)
    double mu, sigma;
    double accum = 0.0;
    for (auto i : v) {
      accum = accum + i;
    }
    mu = (double) accum / (double) iteration_tot;

    // Calculate std deviation (sigma)
    accum = 0.0;
    for (auto i : v) {
      accum += pow(( (double) i - mu), 2);
    }
    sigma = sqrt(accum / iteration_tot);

    // Print report
        std::cout << COUT_YELLOW << name << ":" << COUT_NORMAL << std::endl;
    std::cout << "TRIES TOT: " << iteration_tot << std::endl;
    std::cout << "TIME MU: " << mu << std::endl;
    std::cout << "TIME SIGMA: " << sigma << std::endl;
}

// main
int main(int argc, const char **argv)
{
  // Seed random number generator (use seconds since epoch) to facilitate Monte-Carlo testing
  srand((unsigned int) time(NULL));

  int result = 0;
  // TODO: Move this to a separate testbench
  size_t array_size = 0;

  if (argc < 2) {
    PrintUsage();
    return -1;
  }
  sscanf(argv[1], "%d", (int *) &array_size);
  TestBtree(array_size);

  return result;
}
