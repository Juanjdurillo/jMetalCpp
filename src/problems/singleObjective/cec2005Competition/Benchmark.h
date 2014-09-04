//  Benchmark.h
//
//  Authors:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//       Antonio J. Nebro <antonio@lcc.uma.es>
//
//  Copyright (c) 2014 Antonio J. Nebro
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __BENCHMARK__
#define __BENCHMARK__

#include <iostream>
#include <Math.h>
#include <fstream>
#include <random>
#include <sstream>
#include <string>

#include <F01ShiftedSphere.h>
#include <F02ShiftedSchwefel.h>
#include <F03ShiftedRotatedHighCondElliptic.h>
#include <F04ShiftedSchwefelNoise.h>
#include <F05SchwefelGlobalOptBound.h>
#include <F06ShiftedRosenbrock.h>
#include <F07ShiftedRotatedGriewank.h>
#include <F08ShiftedRotatedAckleyGlobalOptBound.h>
#include <F09ShiftedRastrigin.h>
#include <TestFunc.h>

using namespace std;

class Benchmark {

private:
  static const string DEFAULT_FILE_BIAS;

  static const int NUM_TEST_FUNC;

  // For certain functions, some essential data can be calculated beforehand.
  // Hence, a maximum supported number of dimensions should be specified.
  // Specifiy the number of dimensions here if you need more.
  static const int MAX_SUPPORT_DIM;
  static const double PIx2;

  // Instance variables
  double * m_biases;

public:
  Benchmark();
  Benchmark(string file_bias);
  ~Benchmark();

  // Random number generator
  static mt19937 e2;
  static normal_distribution<double> dist;

  TestFunc * testFunctionFactory(int func_num, int dimension);

  // Basic functions
  static double sphere(double * x, int length);
  static double schwefel_102(double * x, int length);
  static double rosenbrock(double * x, int length);
  static double griewank(double * x, int length);
  static double ackley(double * x, int length);
  static double rastrigin(double * x, int length);

  // Elementary operations
  static void shift(double * results, double * x, double * o, int length);
  static void rotate(double * results, double * x, double ** matrix, int length);

  // Matrix & vector operations
  static void xA(double * result, double * x, double ** A, int length);
  static void Ax(double * result, double ** A, double * x, int length);

  // Utility functions for loading data from given text files
  static void loadRowVectorFromFile(string file, int columns, double * row);
  static void loadRowVector(ifstream& brSrc, int columns, double * row);
  static void loadMatrixFromFile(string file, int rows, int columns, double ** matrix);
  static void loadMatrix(ifstream& brSrc, int rows, int columns, double ** matrix);

}; // Benchmark

#endif /* __BENCHMARK__ */
