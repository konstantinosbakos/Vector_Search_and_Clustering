#include <bits/stdc++.h>

#include "f_g_h.hpp"
#include "../math_functions/math_functions.hpp"

H::H(int d, int w){
    this->d = d;
    this->w = w;
    this->t = uniform_real(0.0,(double)this->w);
    this->v = positive_gaussian_vector(d);
}

int H::calculate(std::vector<int> p){
    double ip = std::inner_product(p.begin(),p.end(),this->v.begin(),0); //Inner product of (p * v).

    return floor(((ip + this->t) / this->w));                            //(ip + t)/w.
}
