#ifndef __GNNS__
#define __GNNS__

#include "../main_vars/main_vars.hpp"
#include "../../src/LSH/LSH.hpp"

#define GNNS_M 1                           //Method value.

class GNNS{
    private:
        int  k;
        int  E;
        int  R;
        int  N;
        int  q_num;                        //Number of queries.
        LSH *lsh;

        EXEC_TIME      *exec_time;         //Execution time of the algorithm for each query.
        NEIGHBOUR_SET **S;                 //Closest points to the query, sorted.
        NEIGHBOUR_SET **knn_neighbours;    //k neighbours of each point in the input.

        void resize_S(int q);
        void delete_structures();
        void init_structures(int q_num);        
        void init_knn_graph(image_array *input_images);
        void T_loop(int q, int train_size, image *Y, image *curr_q, int IMG_S);
        void R_loop(int q, int train_size, image *curr_q, image_array *input_images);
   public:
        GNNS(int q_num, graph_search_input *graph_vars, image_array *input_images);
        ~GNNS();

        void update_structures(int q_num); //Update the structures of the algorithm to rerun it.
        void algorithm_exec(image_array *input_images, image_array *query_images);

        inline EXEC_TIME      *get_exec_time(){return this->exec_time;};
        inline NEIGHBOUR_SET **get_S(){return this->S;};
};

#endif