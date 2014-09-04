//  Benchmark.cpp
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

#include <Benchmark.h>

//TODO: Cambiar ruta
//const string Benchmark::DEFAULT_FILE_BIAS = Configuration.cec2005SupportDataDirectory + "/fbias_data.txt";
const string Benchmark::DEFAULT_FILE_BIAS = "/Users/esteban/Documents/git/jmetalcpp/data/cec2005CompetitionResources/supportData/fbias_data.txt";


const int Benchmark::NUM_TEST_FUNC = 25;

// For certain functions, some essential data can be calculated beforehand.
// Hence, a maximum supported number of dimensions should be specified.
// Specifiy the number of dimensions here if you need more.
const int Benchmark::MAX_SUPPORT_DIM = 100;
const double Benchmark::PIx2 = 3.141592653589793 * 2.0;

/*
 * Default constructor
 */
Benchmark::Benchmark() : Benchmark(DEFAULT_FILE_BIAS) {}

/*
 * Constructor
 */
Benchmark::Benchmark(string file_bias) {
  m_biases = new double[NUM_TEST_FUNC];
  loadRowVectorFromFile(file_bias, NUM_TEST_FUNC, m_biases);
}

/*
 * Destructor
 */
Benchmark::~Benchmark() {
  delete [] m_biases;
}

/**
 * Random number generator
 * If you want to plan a specific seed, do it here
 */
mt19937 Benchmark::e2 {std::random_device{}()};
normal_distribution<double> Benchmark::dist (0,1);

/**
 * Use this function to manufacture new test function objects.
 */
TestFunc * Benchmark::testFunctionFactory(int func_num, int dimension) {
  switch (func_num) {
    case 1:
      return new F01ShiftedSphere(dimension, m_biases[func_num-1]);
      break;
    case 2:
      return new F02ShiftedSchwefel(dimension, m_biases[func_num-1]);
      break;
    case 3:
      return new F03ShiftedRotatedHighCondElliptic(dimension, m_biases[func_num-1]);
      break;
    case 4:
      return new F04ShiftedSchwefelNoise(dimension, m_biases[func_num-1]);
      break;
    case 5:
      return new F05SchwefelGlobalOptBound(dimension, m_biases[func_num-1]);
      break;
    case 6:
      return new F06ShiftedRosenbrock(dimension, m_biases[func_num-1]);
      break;
    case 7:
      return new F07ShiftedRotatedGriewank(dimension, m_biases[func_num-1]);
      break;
    case 8:
      return new F08ShiftedRotatedAckleyGlobalOptBound(dimension, m_biases[func_num-1]);
      break;
    case 9:
      return new F09ShiftedRastrigin(dimension, m_biases[func_num-1]);
      break;

    default:
      cerr << "Incorrect number of function. Expected an integer between " <<
          "1 and 25." << endl;
      exit(-1);
  }
}

/**
 * Sphere function
 */
double Benchmark::sphere(double * x, int length) {
   double sum = 0.0;
   for (int i = 0; i <length; i++) {
     sum += x[i] * x[i];
   }
   return sum;
}

/**
 * Schwefel's problem 1.2
 */
double Benchmark::schwefel_102(double * x, int length) {
  double prev_sum, curr_sum, outer_sum;
  curr_sum = x[0];
  outer_sum = (curr_sum * curr_sum);
  for (int i = 1 ; i < length ; i++) {
    prev_sum = curr_sum;
    curr_sum = prev_sum + x[i];
    outer_sum += (curr_sum * curr_sum);
  }
  return (outer_sum);
}

/**
 * Rosenbrock's function
 */
double Benchmark::rosenbrock(double * x, int length) {
  double sum = 0.0;
  for (int i = 0 ; i < (length-1) ; i ++) {
    double temp1 = (x[i] * x[i]) - x[i+1];
    double temp2 = x[i] - 1.0;
    sum += (100.0 * temp1 * temp1) + (temp2 * temp2);
  }
  return (sum);
}

/**
 * Griewank's function
 */
double Benchmark::griewank(double * x, int length) {
  double sum = 0.0;
  double product = 1.0;
  for (int i = 0 ; i < length ; i ++) {
    sum += ((x[i] * x[i]) / 4000.0);
    double m_iSqrt = sqrt(((double )i) + 1.0);
    product *= cos(x[i] / m_iSqrt);
  }
  return (sum - product + 1.0);
}

/**
 * Ackley's function
 */
double Benchmark::ackley(double * x, int length) {
  double sum1 = 0.0;
  double sum2 = 0.0;
  for (int i = 0 ; i < length ; i ++) {
    sum1 += (x[i] * x[i]);
    sum2 += (cos(PIx2 * x[i]));
  }
  return (-20.0 * exp(-0.2 * sqrt(sum1 / ((double )length))) - exp(sum2 / ((double )length)) + 20.0 + exp(1));
}

/**
 * Rastrigin's function
 */
double Benchmark::rastrigin(double * x, int length) {
  double sum = 0.0;
  for (int i = 0 ; i < length ; i ++) {
    sum += (x[i] * x[i]) - (10.0 * cos(PIx2 * x[i])) + 10.0;
  }
  return sum;
}

/**
 * Shift
 */
void Benchmark::shift(double * results, double * x, double * o, int length) {
  for (int i = 0; i < length; i++) {
    results[i] = x[i] - o[i];
  }
}

/**
 * Rotate
 */
void Benchmark::rotate(double * results, double * x, double ** matrix, int length) {
  xA(results, x, matrix, length);
}

/**
 * (1xD) row vector * (DxD) matrix = (1xD) row vector
 */
void Benchmark::xA(double * result, double * x, double ** A, int length) {
  for (int i = 0 ; i < length ; i ++) {
    result[i] = 0.0;
    for (int j = 0 ; j < length ; j ++) {
      result[i] += (x[j] * A[j][i]);
    }
  }
}

/**
 * (DxD) matrix * (Dx1) column vector = (Dx1) column vector
 */
void Benchmark::Ax(double * result, double ** A, double * x, int length) {
  for (int i = 0 ; i < length ; i++) {
    result[i] = 0.0;
    for (int j = 0 ; j < length ; j++) {
      result[i] += (A[i][j] * x[j]);
    }
  }
}



void Benchmark::loadRowVectorFromFile(string file, int columns, double * row) {

  // Open the file
  ifstream in(file.c_str());
  if( !in ) {
    cerr << "Benchmark::loadRowVectorFromFile: failed when reading from file : " <<
        file << endl;
    exit(-1);
  } else {

    loadRowVector(in, columns, row);
    in.close();

  }

}

void Benchmark::loadRowVector(ifstream& in, int columns, double * row) {
  string aux;

  if (getline(in, aux)) {

    istringstream iss(aux);
    for (int i = 0; i < columns; i++) {
      string stToken;
      iss >> stToken;
      if (stToken.compare("")!=0) {
        row[i] = atof(stToken.c_str());
      } else {
        cerr << "Benchmark::loadRowVector: unexpected format encountered when " <<
            "reading from file (incorrect number of tokens)." << endl;
        exit(-1);
      }
    }
  } else {
    cerr << "Benchmark::loadRowVector: unexpected format encountered when " <<
        "reading from file (zero lines found)." << endl;
    exit(-1);
  }
}

void Benchmark::loadMatrixFromFile(string file, int rows, int columns, double ** matrix) {
  // Open the file
  ifstream in(file.c_str());
  if( !in ) {
    cerr << "Benchmark::loadRowVectorFromFile: failed when reading from file : " <<
    file << endl;
    exit(-1);
  } else {

    loadMatrix(in, rows, columns, matrix);
    in.close();

  }
}

void Benchmark::loadMatrix(ifstream& in, int rows, int columns, double ** matrix) {
  for (int i=0; i<rows; i++) {
    loadRowVector(in, columns, matrix[i]);
  }
}
