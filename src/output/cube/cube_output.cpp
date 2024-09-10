#include <fstream>
#include <iostream>

#include "cube_output.hpp"

void cube_output(image_array *query_images, std::string output_file, Cube *cube_instance, brute_force *BF_instance){
    int              q_num                = query_images->get_num_of_images();
    EXEC_TIME       *bf_exec              = BF_instance->get_exec_time();
    EXEC_TIME       *cube_exec            = cube_instance->get_exec_time();
    NEIGHBOUR_SET  **bf_neighbours        = BF_instance->get_bf_neighbours();
    NEIGHBOUR_SET  **cube_ann_neighbours  = cube_instance->get_ann_neighbours();
    NEIGHBOUR_SET  **cube_appx_neighbours = cube_instance->get_range_neighbours();
    std::ofstream    out_file;
    std::streambuf  *old_cout = std::cout.rdbuf();

    out_file.open(output_file);

    std::cout.rdbuf(out_file.rdbuf());                      //Redirect cout stream to the file.

    if(out_file.is_open()){                                 //Check if the file is open.
        for(int i=0; i<q_num; i++){                         //For each qeury.
            image                         *curr_q = query_images->get_images()[i];
            std::set<prio_queue>::iterator  b_it   = bf_neighbours[i]->begin();
            std::set<prio_queue>::iterator  ann_it = cube_ann_neighbours[i]->begin();

            std::cout << "Query: "               << curr_q->get_file_id() + 1 << std::endl;
            

            int ann_size = cube_ann_neighbours[i]->size();
            for(int j=0; j<ann_size; j++){                  //Print the ann neighbours 
                                                            //and the brute force neighbours.
                std::cout << "Nearest neighbour-"    << j+1 << ": "    << (*ann_it)._image->get_file_id() << std::endl;
                std::cout << "distance-Cube:       " << (*ann_it).dist << std::endl;
                std::cout << "distance-True:       " << (*b_it).dist   << std::endl;

                b_it++;
                ann_it++;
            }

            std::cout << std::right << "tCube: " << cube_exec[i].count() / 1000000.0 << "s" << std::endl;
            std::cout << std::right << "tTrue: " << bf_exec[i].count()  / 1000000.0 << "s" << std::endl;
                                                            //Print the time it took for the cube algorithm
                                                            //and the brute force one.
 
            std::cout << "R-near neighbours:" << std::endl; //Print the Approximate Range algorithm neighbours.

            for(std::set<prio_queue>::iterator apx = cube_appx_neighbours[i]->begin(); 
                apx != cube_appx_neighbours[i]->end(); apx++){

                std::cout << (*apx)._image->get_file_id() << std::endl;
            }

            std::cout << std::endl;
        }
        out_file.close();
    }

    std::cout.rdbuf(old_cout);                              //Redirect the cout stream to terminal.
}
