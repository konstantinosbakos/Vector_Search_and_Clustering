#include <cmath>

#include "LSH.hpp"
#include "../math_functions/math_functions.hpp"

//~Private~//

void LSH::delete_structures(bool hash_tables=false){
    if(hash_tables){
        for(int i=0; i<this->L; i++){
            delete this->HT[i];
            this->HT[i] = NULL;
        }
        delete [] this->HT;
        this->HT = NULL;
    }

    for(int i=0; i<this->q_num; i++){
        delete this->knn_neighbours[i];
        this->knn_neighbours[i] = NULL;
        
        delete this->approx_neighbours[i];
        this->approx_neighbours[i] = NULL;
    }
    delete [] this->knn_neighbours;
    this->knn_neighbours = NULL;

    delete [] this->approx_neighbours;
    this->approx_neighbours = NULL;

    delete [] this->exec_time;
    this->exec_time = NULL;
}

void LSH::init_structures(int q_num, int IMG_S=0, image_array *input_images=NULL, bool hash_tables=true){
    if(hash_tables && input_images){
        this->HT                = new hash_table*[this->L];
       
        for(int i=0; i<this->L; i++){
            unsigned int  M = pow(2,12)-5;
            G            *g = new G((unsigned int)M,IMG_S,2000,this->k);
            this->HT[i]     = new hash_table(input_images->get_num_of_images(),g,input_images->get_images());
        }
    }

    this->q_num             = q_num;
    this->exec_time         = new EXEC_TIME[this->q_num];
    this->knn_neighbours    = new NEIGHBOUR_SET*[this->q_num];
    this->approx_neighbours = new NEIGHBOUR_SET*[this->q_num];

    for(int i=0; i<this->q_num; i++){
        this->knn_neighbours[i]    = new NEIGHBOUR_SET;
        this->approx_neighbours[i] = new NEIGHBOUR_SET;
    }
}

//~Public~//

LSH::LSH(LSH_main_input *LSH_vars, int q_num, image_array *input_images){
    this->k = LSH_vars->get_k();
    this->L = LSH_vars->get_L();
    this->N = LSH_vars->get_N();
    this->R = LSH_vars->get_R();

    this->init_structures(q_num,input_images->get_img_s(),input_images);
}

LSH::LSH(int k, int L, int N, int R, int q_num, image_array *input_images){
    this->k = k;
    this->L = L;
    this->N = N;
    this->R = R;

    this->init_structures(q_num,input_images->get_img_s(),input_images);
}

LSH::~LSH(){
    this->delete_structures(true);
}

void LSH::update_approx(){
    for(int i=0; i<this->q_num; i++){
        delete this->approx_neighbours[i];
        this->approx_neighbours[i] = NULL;
    }

    delete [] this->approx_neighbours;

    this->approx_neighbours = new NEIGHBOUR_SET*[this->q_num];

    for(int i=0; i<this->q_num; i++){
        this->approx_neighbours[i] = new NEIGHBOUR_SET;
    }
}

void LSH::update_structures(int q_num){
    this->delete_structures();
    this->init_structures(q_num);
}

void LSH::kNN(image_array *query_images){
    int IMG_S = query_images->get_img_s();

    for(int i=0; i<this->q_num; i++){
        image *q = query_images->get_images()[i];                           //Current query.

        auto start = std::chrono::high_resolution_clock::now();
        for(int j=0; j<L; j++){
            bucket       *q_bucket = HT[j]->search_item_G(q);               //Get the bucket of the query.
            item            *items = q_bucket->get_items();                 //Get the bucket list.
            unsigned int      q_id = HT[j]->get_hash_function_G()->calculate_id(q); 
                                                                            //Calculate the ID of the query.

            while(items){                                                   //For each item,
                unsigned int item_id = HT[j]->get_hash_function_G()->calculate_id(items->get_image());
                                                                            //get the item id,
                if(q_id == item_id){                                        //if it is the same as the query id,
                    double dist = dist_k(q->get_image_vector(),items->get_image()->get_image_vector(),IMG_S,2);
                                                                            //calculate their distance,
                    if(dist != 0.0){
                        prio_queue new_d(dist,items->get_image());

                        this->knn_neighbours[i]->insert(new_d);             //insert the item with its distance

                        if(int(this->knn_neighbours[i]->size()) > this->N){ //and remove the farthest vector from q,
                                                                            //if there are N+1 elements in the list.
                            std::set<prio_queue>::iterator it = --(this->knn_neighbours[i]->end());
                            this->knn_neighbours[i]->erase(it);
                        }
                    }
                }
                items = items->get_next();
            }
        }

        auto end = std::chrono::high_resolution_clock::now();

        this->exec_time[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    }
}

void LSH::Approximate(image_array *query_images){
    int IMG_S = query_images->get_img_s();

    for(int i=0; i<this->q_num; i++){
        image *q = query_images->get_images()[i];              //Current query.

        for(int j=0; j<this->L; j++){
            bucket       *q_bucket = HT[j]->search_item_G(q);  //Find the bucket of the query.
            item            *items = q_bucket->get_items();    //Get the items of the bucket.

            while(items){                                      //For each item,
                double dist = dist_k(q->get_image_vector(),items->get_image()->get_image_vector(),IMG_S,2);
                                                               //calculate its distance from the query
                if(dist != 0.0 && dist < this->R){             //and if it is smaller from the Radius(R)
                    prio_queue new_d(dist,items->get_image());

                    this->approx_neighbours[i]->insert(new_d); //insert the image with its distance
                                                               //from the query in the approximate 
                                                               //neighbours list.
                }

                items = items->get_next();
            }
        }
    }
}

//~Non-class~//

void delete_lsh(LSH *LSH_instance){
    delete LSH_instance;
    LSH_instance = NULL;
}
