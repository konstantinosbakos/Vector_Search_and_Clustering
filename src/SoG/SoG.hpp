#ifndef __SOG__
#define __SOG__

#include "../main_vars/main_vars.hpp"
#include "../../src/LSH/LSH.hpp"

#define SoG_M 2                    //Method value.

class Search_on_Graph{
    private:
        int    l;
        int    N;
        int    q_num;              //Number of queries.
        int    train_size;         //Input size
        image *navigation_node;

        EXEC_TIME      *exec_time; //Execution time of the algorithm for each query.
        NEIGHBOUR_SET **R;         //Closest points to the query, sorted.
        NEIGHBOUR_SET **L_;

        bool t_loop(int p, image *p_image, image *r_image, double edge_pr, int IMG_S);

        void resize_R(int q);
        void delete_structures();
        void init_structures(int q_num);
        void MRNG(image_array *input_images);
        void insert_Nav_Node_to_R(int q, image *curr_q, int IMG_S);
        void find_navigation_node(image_array *input_images);
        void r_loop(int p, image *p_image, NEIGHBOUR_SET *Rp, int IMG_S);
        void insert_from_N_to_R(int q, int *i, image *p, image *curr_q, int IMG_S);

        image *get_first_unchecked_node(int q, std::set<int> *marked_IDs);
    public:
        Search_on_Graph(int q_num, graph_search_input *graph_vars, image_array *input_images);
        ~Search_on_Graph();

        void update_structures(int q_num);
        void algorithm_exec(image_array *input_images, image_array *query_images);

        inline EXEC_TIME      *get_exec_time(){return this->exec_time;};
        inline NEIGHBOUR_SET **get_R(){return this->R;};
};

#endif