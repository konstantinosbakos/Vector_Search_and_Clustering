#ifndef __BRUTE__
#define __BRUTE__

#include "../utils.hpp"

class brute_force{
    private:
        int N;                             //Neighbour number.
        int q_num;                         //Number of queries.

        EXEC_TIME      *exec_time;         //Execution time of the algorithm for each query.
        NEIGHBOUR_SET **brute_neighbours;  //find_neighbours of each query.

        void delete_structures();          //Free allocated memory that was used in the class.
        void init_structures(int q_num);   //Initialize the structures.
    public:
        brute_force(int N, int q_num);
        ~brute_force();

        void update_structures(int q_num); //Update the structures for new query file.
        void find_neighbours(image_array *input_images, image_array *query_images, bool flag=true); 
                                           //Run the algorithm.

        inline EXEC_TIME      *get_exec_time(){return this->exec_time;};
        inline NEIGHBOUR_SET **get_bf_neighbours(){return this->brute_neighbours;};

};

void delete_brute_force(brute_force *BF_instance);

#endif