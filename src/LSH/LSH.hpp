#ifndef __LSH__
#define __LSH__

#include "../utils.hpp"
#include "../hash_table/hash_table.hpp"
#include "../main_var_classes/LSH/LSH_var.hpp"

class LSH{
    private:
        int k;
        int L;
        int N;
        int R;
        int q_num;                         //Number of queries.

        EXEC_TIME      *exec_time;         //The execution time of the algorithm for each query.
        hash_table    **HT;
        NEIGHBOUR_SET **knn_neighbours;    //The result of the kNN algorithm.
        NEIGHBOUR_SET **approx_neighbours; //The result of the approximate range algorithm.

        void delete_structures(bool hash_tables);
        void init_structures(int q_num, int IMG_S, image_array *input_images, bool hash_tables);
    public:
        LSH(LSH_main_input *LSH_vars, int q_num, image_array *input_images);
        LSH(int k, int L, int N, int R, int q_num, image_array *input_images);
        ~LSH();

        void update_approx();
        void update_structures(int q_num);
        void kNN(image_array *query_images);
        void Approximate(image_array *query_images);

        inline int             get_N(){return this->N;};
        inline void            update_R(int R){this->R = R;};
        inline EXEC_TIME      *get_exec_time(){return this->exec_time;};
        inline NEIGHBOUR_SET **get_knn_neighbours(){return this->knn_neighbours;};
        inline NEIGHBOUR_SET **get_approx_neighbours(){return this->approx_neighbours;};
};

void delete_lsh(LSH *LSH_instance);

#endif