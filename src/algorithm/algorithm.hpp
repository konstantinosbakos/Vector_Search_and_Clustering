#ifndef __ALG__
#define __ALG__

#include "../SoG/SoG.hpp"
#include "../GNNS/GNNS.hpp"
#include "../../src/brute_force/brute_force.hpp"

class algorithm{
    private:
        int   N;
        int   method;

        GNNS            *gnns_algorithm;
        Search_on_Graph *SoG_algorithm;
    public:
        algorithm(int q_num, graph_search_input *graph_vars, image_array *input_images);
        ~algorithm();

        void update_structures(int q_num); //Update the structure for the next execution.
        void algorithm_exec(image_array *input_images, image_array *query_images);
        void output(std::string output_file, brute_force *BF_instance, image_array *input_images, image_array *query_images); 
};

void delete_Algorithm(algorithm *alg_instance);

#endif