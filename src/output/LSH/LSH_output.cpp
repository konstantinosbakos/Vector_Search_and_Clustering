#include <fstream>
#include <iostream>

#include "LSH_output.hpp"

void LSH_output(image_array *query_images, std::string output_file, LSH *LSH_instance, brute_force *BF_instance){
    int             N                   = LSH_instance->get_N();
    int             q_num               = query_images->get_num_of_images();
    EXEC_TIME      *bf_exec             = BF_instance->get_exec_time();
    EXEC_TIME      *lsh_exec            = LSH_instance->get_exec_time();
    NEIGHBOUR_SET **bf_neighbours       = BF_instance->get_bf_neighbours();
    NEIGHBOUR_SET **lsh_knn_neighbours  = LSH_instance->get_knn_neighbours();
    NEIGHBOUR_SET **lsh_appx_neighbours = LSH_instance->get_approx_neighbours();
    std::ofstream   out_file;
    std::streambuf *old_cout = std::cout.rdbuf();

    out_file.open(output_file);

    std::cout.rdbuf(out_file.rdbuf());                      //Redirect cout stream to the file.

    if(out_file.is_open()){                                 //Check if the file is open.
        for(int i=0; i<q_num; i++){                         //For each qeury.
            int                            knn_size = lsh_knn_neighbours[i]->size();
            image                         *curr_q   = query_images->get_images()[i];
            std::set<prio_queue>::iterator  b_it    = bf_neighbours[i]->begin();
            std::set<prio_queue>::iterator  knn_it  = lsh_knn_neighbours[i]->begin();

            std::cout << "Query: "               << curr_q->get_file_id() + 1 << std::endl;
            
            for(int j=0; j<N && j<knn_size; j++){           //Print the kNN neighbours
                                                            //and the brute force neighbours.
                std::cout << "Nearest neighbour-"    << j+1 << ": "    << (*knn_it)._image->get_file_id() << std::endl;
                std::cout << "distance-LSH:        " << (*knn_it).dist << std::endl;
                std::cout << "distance-True:       " << (*b_it).dist   << std::endl;

                b_it++;
                knn_it++;
            }

            std::cout << std::right << "tLSH:  " << lsh_exec[i].count() / 1000000.0 << "s" << std::endl;
            std::cout << std::right << "tTrue: " << bf_exec[i].count()  / 1000000.0 << "s" << std::endl;
                                                            //Print the time it took for the LSH algorithm
                                                            //and the brute force one.
            std::cout << "R-near neighbours:" << std::endl; //Print the Approximate Range algorithm neighbours.

            for(std::set<prio_queue>::iterator apx = lsh_appx_neighbours[i]->begin(); 
                apx != lsh_appx_neighbours[i]->end(); apx++){

                std::cout << (*apx)._image->get_file_id() << std::endl;
            }

            std::cout << std::endl;
        }
        out_file.close();
    }

    std::cout.rdbuf(old_cout);                              //Redirect the cout stream to terminal.
}
