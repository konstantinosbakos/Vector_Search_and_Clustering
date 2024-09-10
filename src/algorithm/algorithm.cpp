#include <fstream>
#include <iostream>

#include "algorithm.hpp"
#include "../../src/math_functions/math_functions.hpp"
//~Public~//

algorithm::algorithm(int q_num, graph_search_input *graph_vars, image_array *input_images){
    this->N      = graph_vars->get_N();
    this->method = graph_vars->get_method();

    if(this->method == GNNS_M){
        this->SoG_algorithm  = NULL;
        this->gnns_algorithm = new GNNS(q_num,graph_vars,input_images);
    }
    else if(this->method == SoG_M){
        this->SoG_algorithm  = new Search_on_Graph(q_num,graph_vars,input_images);
        this->gnns_algorithm = NULL;
    }
}

algorithm::~algorithm(){
    if(this->SoG_algorithm){
        delete this->SoG_algorithm;
        this->SoG_algorithm = NULL;
    }

    if(this->gnns_algorithm){
        delete this->gnns_algorithm;
        this->gnns_algorithm = NULL;
    }
}

void algorithm::update_structures(int q_num){
    if(this->method == GNNS_M){
        this->gnns_algorithm->update_structures(q_num);
    }
    else if(this->method == SoG_M){
        this->SoG_algorithm->update_structures(q_num);
    }
}

void algorithm::algorithm_exec(image_array *input_images, image_array *query_images){
    if(this->method == GNNS_M){
        this->gnns_algorithm->algorithm_exec(input_images,query_images);
    }
    else if(this->method == SoG_M){
        this->SoG_algorithm->algorithm_exec(input_images,query_images);
    }
}

void algorithm::output(std::string output_file, brute_force *BF_instance, image_array *input_images, image_array *query_images){
    int              q_num     = query_images->get_num_of_images();
    double           mAF       = 0.0;
    double           tAvgAppx  = 0.0;
    double           tAvgTrue  = 0.0;
    std::string      algorithm = "";
    std::ofstream    out_file;
    std::streambuf  *old_cout  = std::cout.rdbuf();
    EXEC_TIME       *T_alg     = NULL;
    EXEC_TIME       *T_brute   = BF_instance->get_exec_time();
    NEIGHBOUR_SET  **N_alg     = NULL;
    NEIGHBOUR_SET  **N_brute   = BF_instance->get_bf_neighbours();

    out_file.open(output_file);

    std::cout.rdbuf(out_file.rdbuf());                      //Redirect cout stream to the file.

    if(out_file.is_open()){                                 //Check if the file is open.

        if(this->method == GNNS_M){                         //Initialize for the implemented method.
            N_alg     = this->gnns_algorithm->get_S();
            T_alg     = this->gnns_algorithm->get_exec_time();
            algorithm = "GNNS";
        }
        else if(this->method == SoG_M){
            N_alg     = this->SoG_algorithm->get_R();
            T_alg     = this->SoG_algorithm->get_exec_time();
            algorithm = "MRNG";
        }

        std::cout << algorithm + " Results" << std::endl;

        for(int q=0; q<q_num; q++){                         //For each qeury.
            int                             N_size = N_alg[q]->size();
            image                          *curr_q = query_images->get_images()[q];
            std::set<prio_queue>::iterator  a_it   = N_alg[q]->begin();
            std::set<prio_queue>::iterator  b_it   = N_brute[q]->begin();

            std::cout << "Query: "               << curr_q->get_file_id() << std::endl;
            
            for(int i=0; (i<this->N && i<N_size); i++){     //Print the Approx neighbours 
                                                            //and the brute force neighbours.
                image  *original_image    = input_images->get_images()[(*a_it)._image->get_file_id()];
                double  original_dim_dist = dist_k(original_image->get_image_vector(),curr_q->get_image_vector(),input_images->get_img_s(),2);

                std::cout << "Nearest neighbour-"     << i + 1 << ": "     << (*a_it)._image->get_file_id() << std::endl;
                std::cout << "distanceApproximate:  " << original_dim_dist << std::endl;
                std::cout << "distanceTrue:         " << (*b_it).dist      << std::endl;

                if(i == 0){
                    mAF += original_dim_dist / (*b_it).dist;
                }

                a_it++;
                b_it++;
            }
            std::cout << std::endl;

            tAvgAppx += (  T_alg[q].count() / 1000000.0);   //Add the execution time to find the average.
            tAvgTrue += (T_brute[q].count() / 1000000.0);
        }

        std::cout << "tAverageApproximate: " << tAvgAppx / q_num << "s" << std::endl;
        std::cout << "tAverageTrue:        " << tAvgTrue / q_num << "s" << std::endl;
        std::cout << "mAF:                 " <<      mAF / q_num        << std::endl;

        out_file.close();
    }

    std::cout.rdbuf(old_cout);                              //Redirect the cout stream to terminal.
}

//~Non-class~//

void delete_Algorithm(algorithm *alg_instance){
    delete alg_instance;
    alg_instance = NULL;
}