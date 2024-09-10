#include <cmath>
#include <iostream>

#include "GNNS.hpp"
#include "../../src/math_functions/math_functions.hpp"

//~Private~//

void GNNS::resize_S(int q){
    while(int(this->S[q]->size()) > this->N){               //Keep the N closest points of S.
        this->S[q]->erase(std::prev(this->S[q]->end()));
    }

}

void GNNS::delete_structures(){                             //Delete the structures depended on the query number. 
    for(int q=0; q<this->q_num; q++){
        delete this->S[q];
        this->S[q] = NULL;
    }
    
    delete [] this->S;
    this->S = NULL;

    delete [] this->exec_time;
    this->exec_time = NULL;
}

void GNNS::init_structures(int q_num){                      //Initialize the structures for a different number of queries.
    this->q_num     = q_num;
    this->S         = new NEIGHBOUR_SET*[this->q_num];
    this->exec_time = new EXEC_TIME[this->q_num];

    for(int q=0; q<this->q_num; q++){
        this->S[q] = new NEIGHBOUR_SET;
    }
}

void GNNS::init_knn_graph(image_array *input_images){       //Find k neighbours for each point in the input.
    this->lsh = new LSH(4,5,this->k,-1,input_images->get_num_of_images(),input_images);
    this->lsh->kNN(input_images);
    this->knn_neighbours = this->lsh->get_knn_neighbours(); //Store the result.
}

void GNNS::T_loop(int q, int train_size, image *Y, image *curr_q, int IMG_S){
    if(train_size < 500){                                   //If train size is too small set it to 500 to make
                                                            //at least 2 floor(500/200) steps.
        train_size = 500;
    }

    int            T      = floor(train_size/200);          //Get the t number based on the size of the input set.
    double         y_dist = dist_k(Y->get_image_vector(),curr_q->get_image_vector(),IMG_S,2);
    image         *prev_Y = Y;
    NEIGHBOUR_SET *N_set  = new NEIGHBOUR_SET;              //Priority list "set" that sorts in ascending order

    for(int t=0; t<T; t++){
        int                            knn_size = this->knn_neighbours[Y->get_file_id()]->size();
        std::set<prio_queue>::iterator knn_it   = this->knn_neighbours[Y->get_file_id()]->begin();

        prev_Y = Y;

        for(int i=0; (i<this->E && i<knn_size); i++){       //Check the first E neighbours (or less).
            double dist = dist_k(curr_q->get_image_vector(),(*knn_it)._image->get_image_vector(),IMG_S,2);
                                                            //Find the distance between the query and the
                                                            //neighbours of Y_(t-1).
            prio_queue new_d(dist,(*knn_it)._image);

            N_set->insert(new_d);                           //Add them in the N set
            this->S[q]->insert(new_d);                      //and the S set.

            knn_it++;
        }

        if(N_set->empty() == false){                        //If the N set is not empty, get the
                                                            //first(=closest) point
                                                            //of the N set and set it as the next Y.
            if((*N_set->begin()).dist < y_dist){            //Before the assignment check if the distance
                Y      = (*N_set->begin())._image;          //is smaller than the current Y.
                y_dist = (*N_set->begin()).dist;
            }                                               //If it is not, do not continue.
            
        }

        N_set->clear();                                     //Clear the N set for the next loop.

        if(Y == prev_Y){                                    //Break if it infinitely loops.
            break;
        }
    }

    delete N_set;
    N_set = NULL;
}

void GNNS::R_loop(int q, int train_size, image *curr_q, image_array *input_images){
    int IMG_S = input_images->get_img_s();

    for(int r=0; r<this->R; r++){
        image *Y0 = input_images->get_images()[uniform_int(0,input_images->get_num_of_images()-1)];
                                                            //Get a random point from input.
        this->T_loop(q,train_size,Y0,curr_q,IMG_S);         //Run the T loop.
    }
}

//~Public~//

GNNS::GNNS(int q_num, graph_search_input *graph_vars, image_array *input_images){
    this->k         = graph_vars->get_k();
    this->E         = graph_vars->get_E();
    this->R         = graph_vars->get_R();
    this->N         = graph_vars->get_N();

    this->init_structures(q_num);

    std::cout << ">Initializing knn..." << std::endl;
    this->init_knn_graph(input_images);
    std::cout << ">Finished.          " << std::endl;
}

GNNS::~GNNS(){
    this->delete_structures();

    this->knn_neighbours = NULL;

    delete this->lsh;
    this->lsh = NULL;
}

void GNNS::update_structures(int q_num){                    //Update the structures for a new query set.
    this->delete_structures();
    this->init_structures(q_num);
}

void GNNS::algorithm_exec(image_array *input_images, image_array *query_images){
    int train_size = input_images->get_num_of_images();

    for(int q=0; q<this->q_num; q++){
        image *curr_q = query_images->get_images()[q];      //Get the current query.
       
        auto start = std::chrono::high_resolution_clock::now();
        this->R_loop(q,train_size,curr_q,input_images);     //Run the R loop.

        this->resize_S(q);
        auto end = std::chrono::high_resolution_clock::now();

        this->exec_time[q] = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    }
}