#include "brute_force.hpp"
#include "../math_functions/math_functions.hpp"

//~Private~//

void brute_force::delete_structures(){
    for(int i=0; i<this->q_num; i++){
        delete this->brute_neighbours[i];
        this->brute_neighbours[i] = NULL;
    }

    delete [] this->brute_neighbours;
    this->brute_neighbours = NULL;

    delete [] this->exec_time;
    this->exec_time = NULL;
}

void brute_force::init_structures(int q_num){
    this->q_num            = q_num;
    this->exec_time        = new EXEC_TIME[this->q_num];
    this->brute_neighbours = new NEIGHBOUR_SET*[this->q_num];

    for(int i=0; i<this->q_num; i++){
        this->brute_neighbours[i] = new NEIGHBOUR_SET;
    }
}

//~Public~//

brute_force::brute_force(int N, int q_num){
    this->N = N;

    this->init_structures(q_num);
}

brute_force::~brute_force(){
    this->delete_structures();    
}

void brute_force::update_structures(int q_num){
    this->delete_structures();    //Firstly delete the structures used for the previous query.
    this->init_structures(q_num); //Then initialise the structures for the new query.
}

void brute_force::find_neighbours(image_array *input_images, image_array *query_images, bool flag){
    int IMG_S = input_images->get_img_s();

    for(int i=0; i<this->q_num; i++){
        int    num_of_images = input_images->get_num_of_images();
        image *q             = query_images->get_images()[i];         //Current query.

        auto start = std::chrono::high_resolution_clock::now();       //Start counting execution time.
        for(int j=0; j<num_of_images; j++){                           //Brute force search for every image.
            image *curr_image = input_images->get_images()[j];        //Curent image.

            double dist = 0.0;

            if(flag){
                dist = dist_k(curr_image->get_image_vector(),q->get_image_vector(),IMG_S,2); 
            }
            else{
                dist = dist_k(curr_image->get_image_vector(),q->get_image_vector_d(),IMG_S,2); 
            }                                                         //Find the distance.

            if(dist != 0.0){                                          //Do not count the image itself if it is 
                                                                      //found in the dataset (check ReadMe for more)
                prio_queue new_d(dist,curr_image);

                this->brute_neighbours[i]->insert(new_d);

                if(int(this->brute_neighbours[i]->size()) > this->N){ //If there are N+1 neighbours in the list, 
                                                                      //remove the farthest vector from q.
                    std::set<prio_queue>::iterator it = --(this->brute_neighbours[i]->end());
                    this->brute_neighbours[i]->erase(it);
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();         //Stop counting execution time.

        this->exec_time[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                                                                      //Store execution time.
    }
}

//~Non-class~//

void delete_brute_force(brute_force *BF_instance){
    delete BF_instance;
    BF_instance = NULL;
}
