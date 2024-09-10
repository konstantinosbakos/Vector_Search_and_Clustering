
#include <bits/stdc++.h>

#include "math_functions.hpp"

int coin_toss(){
    return (std::rand() % 2);
}

int hamming_distance(int a, int b){      //Return the hamming distance by
                                         //calculating the XOR value of the 
                                         //two numbers and counting the set bits
                                         //of that value.
    std::bitset<32> weight(uint32_t(a ^ b));
    
    return weight.count();
}

int bin2dec(int size, int *bin_array){   //Turn a binary array to a decimal number.
    int dec = 0;
    
    for(int i=(size-1); i>=0; i--){
        dec += bin_array[i] * pow(2,(size-i-1));
    }

    return dec;
}

int uniform_int(int a, int b){           //Return an integer using uniform distribution.
    std::random_device               random_seed; 
    std::default_random_engine       generator(random_seed());
    std::uniform_int_distribution<>  U(a,b);

    return U(generator);
}

double uniform_real(double a, double b){ //Return a using uniform distribution.
    std::random_device               random_seed; 
    std::default_random_engine       generator(random_seed());
    std::uniform_real_distribution<> U(a,b);

    return U(generator);
}

double dist_k(std::vector<int> vector_a, std::vector<int> vector_b, int size, int k){
    double sum = 0.0;

    for(int i=0; i<size; i++){           //|x_i - y_i|^k.
        sum += pow(abs(vector_a[i] - vector_b[i]),k);
    }

    return pow(sum,(1.0/k));             //sqrt(sum,k).
}

double dist_k(std::vector<int> vector_a, std::vector<double> vector_b, int size, int k){
    double sum = 0.0;

    for(int i=0; i<size; i++){           //|x_i - y_i|^k.
        sum += pow(abs((double)vector_a[i] - vector_b[i]),k);
    }

    return pow(sum,(1.0/k));             //sqrt(sum,k).
}

double dist_k(std::vector<double> vector_a, std::vector<double> vector_b, int size, int k){
    double sum = 0.0;

    for(int i=0; i<size; i++){           //|x_i - y_i|^k.
        sum += pow(abs(vector_a[i] - vector_b[i]),k);
    }

    return pow(sum,(1.0/k));             //sqrt(sum,k).
}

std::vector<int> hamming_distance_k(int number, int lo, int hi, int k){
    std::vector<int> hamming_k;

    for(int i=lo; i<hi; i++){            //High not included.
        if(hamming_distance(i,number) == k){
            hamming_k.push_back(i);
        }
    }

    return hamming_k;
}

std::vector<double> positive_gaussian_vector(int size){
    std::vector<double>              vector;
    std::random_device               random_seed; 
    std::default_random_engine       generator(random_seed());
    std::normal_distribution<double> N(0.0,1.0);

    vector.resize(size);

    for(int i=0; i<size; i++){           //Generate each vector element
                                         //randomly using the function N(0,1).
        vector[i] = N(generator);
    }

    double min_value = *std::min_element(vector.begin(),vector.end());
                                         //Find the minimum element of the vector.
    
    if(min_value < 0){                   //If it is negative, add its absolute value 
                                         //to all the elements in order to make all
                                         //of them positive (normalisation).
        double abs_min_value = abs(min_value);

        for(int i=0; i<size; i++){
            vector[i] = vector[i] + abs_min_value;
        }
    }
    return vector;
}
