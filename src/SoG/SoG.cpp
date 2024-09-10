#include <iostream>
#include <algorithm>

#include "SoG.hpp"
#include "../../src/brute_force/brute_force.hpp"
#include "../../src/math_functions/math_functions.hpp"

//~Private~//

bool Search_on_Graph::t_loop(int p, image *p_image, image *r_image, double edge_pr, int IMG_S){
    for(std::set<prio_queue>::iterator t = this->L_[p]->begin();
        t != this->L_[p]->end(); t++){

        image *t_image = (*t)._image;

        double edge_pt = dist_k(p_image->get_image_vector(),t_image->get_image_vector(),IMG_S,2);
        double edge_rt = dist_k(r_image->get_image_vector(),t_image->get_image_vector(),IMG_S,2);
                                                                     //Calculate the pt and rt edge of the p-r-t triange.
        if(edge_pr > edge_pt && edge_pr > edge_rt){                  //If pr is the biggest edge of an p-r-t triange,
            return false;                                            //condition = false,
        }
    }

    return true;                                                     //else, condition = true.
}

void Search_on_Graph::resize_R(int q){
    while(int(this->R[q]->size()) > this->N){
                this->R[q]->erase(std::prev(this->R[q]->end()));
    }
}

void Search_on_Graph::delete_structures(){                           //Delete the structures depended on the query number. 
    for(int q=0; q<this->q_num; q++){
        delete this->R[q];
        this->R[q] = NULL;
    }
    
    delete [] this->R;
    this->R = NULL;

    delete [] this->exec_time;
    this->exec_time = NULL;
}

void Search_on_Graph::init_structures(int q_num){                    //Initialize the structures for a different number of queries.
    this->q_num     = q_num;
    this->R         = new NEIGHBOUR_SET*[this->q_num];
    this->exec_time = new EXEC_TIME[this->q_num];

    for(int q=0; q<this->q_num; q++){
        this->R[q] = new NEIGHBOUR_SET;
    }
}

void Search_on_Graph::MRNG(image_array *input_images){
    int IMG_S         = input_images->get_img_s();

    this->train_size  = input_images->get_num_of_images();           //Get the number of the points.
    this->L_          = new NEIGHBOUR_SET*[train_size];              //Create the L_p set for each point (p).

    LSH    *lsh       = new LSH(5,7,train_size,-1,train_size,input_images);
                                                                     //Run knn to find the best candidates.
    image **S         = input_images->get_images();

    lsh->kNN(input_images);                                          //Get the approximate best canditates for each point.

    NEIGHBOUR_SET **Rp_cand = lsh->get_knn_neighbours();             //R_p = l candidates.

    for(int p=0; p<train_size; p++){                                 //Repeat for every point.
        this->L_[p] = new NEIGHBOUR_SET;
        this->L_[p]->insert((*(Rp_cand[p])->begin()));               //Initialize Lp with the closest point to p.
        this->r_loop(p,S[p],Rp_cand[p],IMG_S);
    }

    delete lsh;
    lsh = NULL;
}

void Search_on_Graph::insert_Nav_Node_to_R(int q, image *curr_q, int IMG_S){
    double dist = dist_k(curr_q->get_image_vector(),this->navigation_node->get_image_vector(),IMG_S,2);
                                                                     //Calculate the distance between
                                                                     //the navigation node and the current query.
    prio_queue navigation(dist,this->navigation_node);

    this->R[q]->insert(navigation);                                  //Insert the navigation node and the distance in R.
}

void Search_on_Graph::find_navigation_node(image_array *input_images){
    int                 IMG_S = input_images->get_img_s();
    std::vector<int>    sum(IMG_S,0);
    std::vector<double> mean(IMG_S,0.0);

    for(int i=0; i<this->train_size; i++){
        std::vector<int> curr_image_vector = input_images->get_images()[i]->get_image_vector();

        std::transform(sum.begin(),sum.end(),
            curr_image_vector.begin(),sum.begin(),std::plus<int>());
    }                                                                //Calculate the sum of all the points in the dataset.

    std::fill(mean.begin(),mean.end(),this->train_size);
    std::transform(sum.begin(),sum.end(),mean.begin(),
                    mean.begin(),std::divides<double>());            //Divide it by the dataset size to get the mean.

    image_array *query       = new image_array(1,true);
    brute_force *BF_instance = new brute_force(1,1);

    query->add_image(0,mean);                                        //Create a query with the centroid.
    BF_instance->find_neighbours(input_images,query,false);          //Find the closest real node
                                                                     //to the imaginary centroid.

    this->navigation_node = (*BF_instance->get_bf_neighbours()[0]->begin())._image;
                                                                     //Set the navigation node.

    delete query;
    query = NULL;
    delete BF_instance;
    BF_instance = NULL;
}

void Search_on_Graph::r_loop(int p, image *p_image, NEIGHBOUR_SET *Rp, int IMG_S){
    bool condition = true;

    for(std::set<prio_queue>::iterator r = std::next(Rp->begin());
        r != Rp->end(); r++){

        condition = true;

        image  *r_image = (*r)._image;

        double edge_pr  = (*r).dist;                                 //Calculate the pr edge of the p-r-t triangle.

        condition = this->t_loop(p,p_image,r_image,edge_pr,IMG_S);

        if(condition){                                               //If condition = true,
            prio_queue pr(edge_pr,r_image);                          //add a (directed) edge to the graph,
                                                                     //from p to r (with the distance).

            this->L_[p]->insert(pr);
        }
    }
}

void Search_on_Graph::insert_from_N_to_R(int q, int *i, image *p, image *curr_q, int IMG_S){
    std::set<prio_queue>::iterator it_L = this->L_[p->get_file_id()]->begin();

    while(it_L != this->L_[p->get_file_id()]->end()){
        if(this->R[q]->find((*it_L)) == this->R[q]->end()){          //If the current neighbour is not in R,
            double dist = dist_k(curr_q->get_image_vector(),(*it_L)._image->get_image_vector(),IMG_S,2);
                                                                     //get the distance from the query
            prio_queue R_i(dist,(*it_L)._image);

            this->R[q]->insert(R_i);                                 //and insert it in R.
            
            (*i)++;

            if((*i) == this->l){                                     //Break if i reaches l (so it continues only if i < l).
                return;
            }
        }

        it_L++;
    }
}

image *Search_on_Graph::get_first_unchecked_node(int q, std::set<int> *marked_IDs){
    std::set<prio_queue>::iterator it_Rq = this->R[q]->begin();

    while(it_Rq != this->R[q]->end()){
        if((*marked_IDs).find((*it_Rq)._image->get_file_id()) == (*marked_IDs).end()){
            (*marked_IDs).insert((*it_Rq)._image->get_file_id());

            return (*it_Rq)._image;
        }
        else{
            it_Rq++;
        }
    }

    return NULL;                                                     //If there is no unchecked node in R,
                                                                     //return NULL;
}

//~Public~//

Search_on_Graph::Search_on_Graph(int q_num, graph_search_input *graph_vars, image_array *input_images){
    this->l = graph_vars->get_l();
    this->N = graph_vars->get_N();

    this->init_structures(q_num);

    std::cout << ">Initializing MRNG..."       << std::endl;
    this->MRNG(input_images);
    std::cout << ">Finished."                  << std::endl;

    std::cout << ">Finding Navigation Node..." << std::endl;
    this->find_navigation_node(input_images);
    std::cout << ">Found."                     << std::endl;
}

Search_on_Graph::~Search_on_Graph(){
    this->delete_structures();

    for(int p=0; p<this->train_size; p++){
        delete this->L_[p];
    }

    delete [] this->L_;
    this->L_ = NULL;
}

void Search_on_Graph::update_structures(int q_num){                  //Update the structures for a new query set.
    this->delete_structures();
    this->init_structures(q_num);
}

void Search_on_Graph::algorithm_exec(image_array *input_images, image_array *query_images){
    int            IMG_S      = input_images->get_img_s();
    std::set<int> *marked_IDs = new std::set<int>;                   //Used to save the IDs of the images
    
    for(int q=0; q<this->q_num; q++){
        image *curr_q = query_images->get_images()[q];               //Get the current query.
                                                                     //that the algorithm already used as p.
        auto start = std::chrono::high_resolution_clock::now();

        this->insert_Nav_Node_to_R(q,curr_q,IMG_S);

        for(int i=1; i<this->l; i++){
            image *p = this->get_first_unchecked_node(q,marked_IDs); //Get the next p image.

            if(p == NULL){                                           //No unmarked nodes found.
                break;
            }

            this->insert_from_N_to_R(q,&i,p,curr_q,IMG_S);           //Insert every neighbour of p to R
                                                                     //(that is not already in).
        }

        this->resize_R(q);                                           //Make R to have size N.

        auto end = std::chrono::high_resolution_clock::now();

        this->exec_time[q] = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        marked_IDs->clear();
    }

    delete marked_IDs;
    marked_IDs = NULL;
}
