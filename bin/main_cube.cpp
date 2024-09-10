#include "../src/cube/cube.hpp"
#include "../src/output/cube/cube_output.hpp"
#include "../src/brute_force/brute_force.hpp"
#include "../src/UI_messages/UI_messages.hpp"

int main(int argc, char **argv){
    std::srand(time(NULL));

    image_array     *input_images = new image_array;
    image_array     *query_images = new image_array;
    cube_main_input *cube_vars    = new cube_main_input;

    cube_vars->cmd_input_cube(argc,argv);

    if(cube_vars->get_cmd_value() == CUBE_HELP){              //If the user wants the help prompt,
        cube_help_prompt();                                   //print it

        delete_cube_vars(cube_vars,input_images,query_images);
        return EXIT_SUCCESS;                                  //and exit.
    }

    if(cube_vars->get_cmd_value() == CUBE_NO_CMD_IN||         //If there is no command line input,
       cube_vars->get_cmd_value() == CUBE_ONLY_VARS){         //or the input was only variables,
       
       cube_vars->request_files();                            //request the user to input the file paths.
    }

    query_images->read_images(cube_vars->get_q());            //Read the query images.
    input_images->read_images(cube_vars->get_in());           //Read the input images.

    if(query_images->get_images() == NULL ||                  //If the file path for the queries
       input_images->get_images() == NULL){                   //or the input does not exist,
        file_path_error();                                    //print the error

        delete_cube_vars(cube_vars,input_images,query_images);
        return EXIT_SUCCESS;                                  //and exit.
    }

    Cube        *cube_instance = new Cube(cube_vars,query_images->get_num_of_images(),input_images); 
                                                              //Instantiate the LSH class.
    brute_force *BF_instance   = new brute_force(cube_vars->get_N(),query_images->get_num_of_images());
                                                              //Instantiate the brute_force class.
    while(1){
        BF_instance->find_neighbours(input_images,query_images);               //Run the brute force algorithm.
        cube_instance->ANN(query_images);                                      //Run the ANN Hypercube algorithm.
        cube_instance->range_search(query_images);                             //Run the range search Hypercube algorithm.
        
        cube_output(query_images,cube_vars->get_out(),cube_instance,BF_instance);
#ifndef NLOOP
        cube_vars->set_repeat(repetition_question());                          //Ask the user if they want to repeat 
                                                                               //for different queries. 

        if(cube_vars->get_repeat()){                                           //If the user wants to repeat,
            cube_vars->request_file("query");                                  //ask for the new query file,

            query_images->update_images(cube_vars->get_q());                   //Update the objects accordingly
            
            if(query_images->get_images() == NULL){                            //If the file path for the images          
                file_path_error();                                             //does not exist,
                                                                               //print the error

                delete_cube(cube_instance);
                delete_brute_force(BF_instance);
                delete_cube_vars(cube_vars,input_images,query_images);

                return EXIT_SUCCESS;                                           //and exit.
            }

            BF_instance->update_structures(query_images->get_num_of_images());
            cube_instance->update_structures(query_images->get_num_of_images());
     
            if(change_output_file()){                                          //and ask if they want to change the output file,
                                                                               //to avoid overwritting the existing one.
                cube_vars->request_file("output");                             //If yes, save the new output file path.
            }
        }
        else{                                                                  //If the user does not want to repeat,
                                                                               //terminate the loop.
            break;
        }
#else
        break;
#endif
    }

    delete_cube(cube_instance);
    delete_brute_force(BF_instance);
    delete_cube_vars(cube_vars,input_images,query_images);

    return EXIT_SUCCESS;
}