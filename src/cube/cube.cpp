#include <cmath>

#include "cube.hpp"
#include "../math_functions/math_functions.hpp"

//~Private~//

void Cube::delete_structures(bool HT_del=false){
    if(HT_del){
        delete this->HT;
        this->HT = NULL;
    }

    for(int i=0; i<this->q_num; i++){
        delete this->ann_neighbours[i];
        this->ann_neighbours[i] = NULL;
        
        delete this->range_neighbours[i];
        this->range_neighbours[i] = NULL;
    }

    delete [] this->exec_time;
    this->exec_time = NULL;

    delete [] this->ann_neighbours;
    this->ann_neighbours = NULL;

    delete [] this->range_neighbours;
    this->range_neighbours = NULL;

}

void Cube::init_structures(int q_num, int IMG_S=0, image_array *input_images=NULL){
    this->q_num = q_num;
    
    if(input_images){
        F *f         = new F(IMG_S,this->k);
        this->HT     = new hash_table(input_images->get_num_of_images(),this->k,f,input_images->get_images());
    }

    this->exec_time         = new EXEC_TIME[this->q_num];
    this->ann_neighbours    = new NEIGHBOUR_SET*[this->q_num];
    this->range_neighbours  = new NEIGHBOUR_SET*[this->q_num];

    for(int i=0; i<this->q_num; i++){
        this->ann_neighbours[i]   = new NEIGHBOUR_SET;
        this->range_neighbours[i] = new NEIGHBOUR_SET;
    }
}

void Cube::check_items_ann(int IMG_S, item *items, int q, image **queries, int *curr_m){
    while(items){
        double dist = dist_k(queries[q]->get_image_vector(),
                                items->get_image()->get_image_vector(),IMG_S,2);

        if(dist != 0.0){
            prio_queue new_d(dist,items->get_image());

            this->ann_neighbours[q]->insert(new_d);             //Insert the item with its distance
                                                                //to the neighbours list

            if(int(this->ann_neighbours[q]->size()) > this->N){ //and remove the farthest vector from q,
                                                                //if there are N+1 elements in the list.
                std::set<prio_queue>::iterator it = --(this->ann_neighbours[q]->end());
                this->ann_neighbours[q]->erase(it);
            }
        }

        if(++(*curr_m) == this->M){                             //Update how many points have been checked.
            break;
        }

        items = items->get_next();
    }
}

void Cube::check_items_range(int IMG_S, item *items, int q, image **queries, int *curr_m){
    
    while(items){
        double dist = dist_k(queries[q]->get_image_vector(),
                                items->get_image()->get_image_vector(),IMG_S,2);

        if(dist != 0.0 && dist <= this->R){
            prio_queue new_d(dist,items->get_image());

            this->range_neighbours[q]->insert(new_d);           //Insert the item with its distance
                                                                //to the neighbours list.
        }

        if(++(*curr_m) == this->M){                             //Update how many points have been checked.
            break;
        }

        items = items->get_next();
    }
}

//~Public~//

Cube::Cube(cube_main_input *cube_vars, int q_num, image_array *input_images){
    this->k      = cube_vars->get_k();
    this->M      = cube_vars->get_M();
    this->N      = cube_vars->get_N();
    this->R      = cube_vars->get_R();
    this->probes = cube_vars->get_probes();

    this->init_structures(q_num,input_images->get_img_s(),input_images);
}

Cube::Cube(int k, int M, int N, int R, int probes, int q_num, image_array *input_images){
    this->k      = k;
    this->M      = M;
    this->N      = N;
    this->R      = R;
    this->probes = probes;

    this->init_structures(q_num,input_images->get_img_s(),input_images);
}

Cube::~Cube(){
    this->delete_structures(true);
}

void Cube::update_approx(){              //Reinitialize the range_search neighbour set.
    for(int i=0; i<this->q_num; i++){
        delete this->range_neighbours[i];
        this->range_neighbours[i] = NULL;
    }

    delete [] this->range_neighbours;

    this->range_neighbours = new NEIGHBOUR_SET*[this->q_num];

    for(int i=0; i<this->q_num; i++){
        this->range_neighbours[i] = new NEIGHBOUR_SET;
    }
}

void Cube::update_structures(int q_num){ //Reinitialize the structures for a different number of queries.
    this->delete_structures();
    this->init_structures(q_num);
}

void Cube::ANN(image_array *query_images){
    int     IMG_S   = query_images->get_img_s();
    int     q_num   = query_images->get_num_of_images();
    image **queries = query_images->get_images();

    for(int q=0; q<q_num; q++){
        auto start  = std::chrono::high_resolution_clock::now();
  
        int     curr_m           = 0;
        int     hamming_0        = this->HT->get_hash_function_F()->calculate_f(queries[q]); 
                                                                                     //Find the hash of the query,
        int     curr_probes      = 1; 
        int     hamming_distance = 1;
        item   *items            = this->HT->search_item_F(queries[q])->get_items(); //get the bucket with that has value and

        this->check_items_ann(IMG_S,items,q,queries,&curr_m);                        //check the points of the bucket.

        while((curr_m < this->M) && (curr_probes < this->probes)){                   //Untill either M points are checked, 
                                                                                     //or probes buckets are checked.
            std::vector<int> hamming_probe = hamming_distance_k(hamming_0,0,pow(2,k),hamming_distance);
                                                                                     //Get the hamming distance values (in the 0-2^k range).

            for(int i : hamming_probe){
                item   *items    = this->HT->get_bucket(i)->get_items();

                this->check_items_ann(IMG_S,items,q,queries,&curr_m);

                if(curr_m == this->M){
                    break;
                }
                
                if(++curr_probes == this->probes){
                    break;
                }
            }

            hamming_distance++;
        }

        auto end = std::chrono::high_resolution_clock::now();

        this->exec_time[q] = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    }
}

void Cube::range_search(image_array *query_images){
    int IMG_S = query_images->get_img_s();

    for(int q=0; q<this->q_num; q++){
        int     curr_m           = 0;
        int     curr_probes      = 1; 
        int     hamming_distance = 1;
        image **queries          = query_images->get_images();
        int     hamming_0        = this->HT->get_hash_function_F()->calculate_f(queries[q]); //Get the has value of the query.
        item   *items            = this->HT->search_item_F(queries[q])->get_items();         //Find the query bucket.

        this->check_items_range(IMG_S,items,q,queries,&curr_m);                              //Check all the items in the bucket.

        while((curr_m < this->M) && (curr_probes < this->probes)){                           //Continue checking untill M or probes flags are met.
            std::vector<int> hamming_probe = hamming_distance_k(hamming_0,0,pow(2,k),hamming_distance);

            for(int i : hamming_probe){
                item   *items    = this->HT->get_bucket(i)->get_items();

                this->check_items_range(IMG_S,items,q,queries,&curr_m);

                if(curr_m == this->M){
                    break;
                }
                
                if(++curr_probes == this->probes){
                    break;
                }
            }

            hamming_distance++;
        }
    }
}

//~Non-Class~//

void delete_cube(Cube *cube_instance){
    delete cube_instance;
    cube_instance = NULL;
}
