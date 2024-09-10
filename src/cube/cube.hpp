#ifndef __CUBE__
#define __CUBE__

#include "../utils.hpp"
#include "../hash_table/hash_table.hpp"
#include "../main_var_classes/cube/cube_var.hpp"

class Cube{
    private:
        int k;
        int M;
        int N;
        int R;
        int q_num;                        //Number of queries.
        int probes;

        EXEC_TIME      *exec_time;        //The execution time of the algorithm for each query.
        hash_table     *HT;
        NEIGHBOUR_SET **ann_neighbours;   //The result of the ANN algorithm.
        NEIGHBOUR_SET **range_neighbours; //The result of the approximate range algorithm.

        void delete_structures(bool HT_del);
        void init_structures(int q_num, int IMG_S, image_array *input_images);
        void check_items_ann(int IMG_S, item *items, int q, image **queries, int *curr_m);
        void check_items_range(int IMG_S, item *items, int q, image **queries, int *curr_m);
    public:
        Cube(cube_main_input *cube_vars, int q_num, image_array *input_images);
        Cube(int k, int M, int N, int R, int probes, int q_num, image_array *input_images);
        ~Cube();

        void update_approx();
        void update_structures(int q_num);
        void ANN(image_array *query_images);
        void range_search(image_array *query_images);

        inline void update_R(int R){this->R = R;};

        inline int             get_N(){return this->N;};
        inline EXEC_TIME      *get_exec_time(){return this->exec_time;};
        inline NEIGHBOUR_SET **get_ann_neighbours(){return this->ann_neighbours;};
        inline NEIGHBOUR_SET **get_range_neighbours(){return this->range_neighbours;};
};

void delete_cube(Cube *cube_instance);

#endif