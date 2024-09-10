#include <bits/stdc++.h> 

#include "f_g_h.hpp"
#include "../math_functions/math_functions.hpp"

G::G(unsigned int M, int d, int w, int k){
    this->k          = k;
    this->M          = M;
    this->h_         = new H*[k];
    this->r_         = new int[k];

    for(int i=0; i<this->k; i++){ //Initialise the h_i() functions.
        this->h_[i] = new H(d,w);
    }
    for(int i=0; i<this->k; i++){ //Initialise (randomly) the r_i (positive) values.
        this->r_[i] = uniform_int(0);
    }
}

G::~G(){
    for(int i=0; i<this->k; i++){
        delete this->h_[i];
        this->h_[i] = NULL;
    }

    delete [] this->h_;
    this->h_ = NULL;

    delete [] this->r_;
    this->r_ = NULL;
}

unsigned int G::calculate_id(image *p){
    unsigned int sum = 0;

    for(int i=0; i<this->k; i++){                //r_i * h_i(p).
        sum += this->r_[i] * this->h_[i]->calculate(p->get_image_vector());
    }

    return sum % this->M;                        //sum mod M.
}

unsigned int G::calculate_g(image *p, int table_size){
    return (this->calculate_id(p) % table_size); //ID mod table_size.
}
