#include "f_g_h.hpp"
#include "../math_functions/math_functions.hpp"

F::F(int IMG_S, int k){
    this->k  = k;
    this->h_ = new H*[k];     //Initialize the h_i() functions.

    for(int i=0; i<this->k; i++){
        this->h_[i] = new H(IMG_S,4000);
    }

    this->SL = new skip_list; //Create the skip_list that will save the bool values
                              //for each h() value. O(logn) search time.
}

F::~F(){
    for(int i=0; i<this->k; i++){
        delete this->h_[i];
        this->h_[i] = NULL;
    }

    delete [] this->h_;
    this->h_ = NULL;

    delete this->SL;
    this->SL = NULL;
}

int F::calculate_f(image *p){
    int  dec       = 0;
    int *bin_array = new int[this->k];

    for(int i=0; i<this->k; i++){
        int h_ID        = this->h_[i]->calculate(p->get_image_vector()); //Calculate the value of the h() function.
        skip_node *node = this->SL->search(h_ID);                        //Search if the value is already assigned.

        if(node){                                                        //If it is,
            bin_array[i]  = node->get_bin_value();                       //get the assigned value.
        }
        else{                                                            //If it is not,
            int bin_value = coin_toss();                                 //assign a new value,

            bin_array[i]  = bin_value;
            this->SL->insert(h_ID,bin_value);                            //and insert it in the list.
        }
    }

    dec = bin2dec(this->k, bin_array);                                   //Turn the binary array into a decimal number.

    delete [] bin_array;
    bin_array = NULL;

    return dec;                                                          //Return the number as the hash value of an image.
}
