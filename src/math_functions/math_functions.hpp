#ifndef __MATH__
#define __MATH__

#include <vector>
#include <climits>

int coin_toss();
int hamming_distance(int a, int b);
int bin2dec(int size, int *bin_array);
int uniform_int(int a, int b=INT_MAX);

double uniform_real(double a, double b);
double dist_k(std::vector<int> vector_a, std::vector<int> vector_b, int size, int k);
double dist_k(std::vector<int> vector_a, std::vector<double> vector_b, int size, int k);
double dist_k(std::vector<double> vector_a, std::vector<double> vector_b, int size, int k);

std::vector<int> hamming_distance_k(int number, int lo, int hi, int k);

std::vector<double> positive_gaussian_vector(int size);


#endif