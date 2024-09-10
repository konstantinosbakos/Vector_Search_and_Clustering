#include "../src/algorithm/algorithm.hpp"
#include "../src/brute_force/brute_force.hpp"
#include "../src/UI_messages/UI_messages.hpp"

int main(int argc, char **argv){
    image_array        *input_images_o = new image_array;
    image_array        *input_images_l = new image_array;
    image_array        *query_images_o = new image_array;
    image_array        *query_images_l = new image_array;
    graph_search_input *graph_vars     = new graph_search_input;

    graph_vars->cmd_input_graph(argc,argv);

    if(graph_vars->get_cmd_value() == GRAPH_HELP){                             //If the user wants the help prompt,
        SoG_help_prompt();
        GNNS_help_prompt();                                                    //print it

        delete_graph_search_vars(graph_vars,input_images_l,query_images_l,input_images_o,query_images_o);
        return EXIT_SUCCESS;                                                   //and exit.
    }

    if(graph_vars->get_cmd_value() == GRAPH_NO_METHOD){                        //If there is no method,
        error_method();                                                        //print the error
        
        delete_graph_search_vars(graph_vars,input_images_l,query_images_l,input_images_o,query_images_o);
        return EXIT_SUCCESS;                                                   //and exit.
    }
    else if(graph_vars->get_cmd_value() == GRAPH_k_E_MISMATCH){                //If k < E,
        error_k_E_mismatch();                                                  //print the error
        
        delete_graph_search_vars(graph_vars,input_images_l,query_images_l,input_images_o,query_images_o);
        return EXIT_SUCCESS;                                                   //and exit.
    }
    else if(graph_vars->get_cmd_value() == GRAPH_l_N_MISMATCH){                //If l < N,
        error_l_N_mismatch();                                                  //print the error
        
        delete_graph_search_vars(graph_vars,input_images_l,query_images_l,input_images_o,query_images_o);
        return EXIT_SUCCESS;                                                   //and exit.
    }
    else if(graph_vars->get_cmd_value() == GRAPH_NO_CMD_IN ||                  //If there is no input,
        graph_vars->get_cmd_value() != GRAPH_ONLY_FILES){                      //or there is no file input,
       
        graph_vars->request_files();                                           //ask for the file paths.
    }

    query_images_l->read_images(graph_vars->get_ql());                         //Read the query images.
    query_images_o->read_images(graph_vars->get_qo());
    input_images_l->read_images(graph_vars->get_il());                         //Read the input images.
    input_images_o->read_images(graph_vars->get_io());

    if(query_images_l->get_images() == NULL ||                                 //If the file path for the images
       query_images_o->get_images() == NULL ||
       input_images_l->get_images() == NULL ||
       input_images_o->get_images() == NULL){                                  //does not exist,
        file_path_error();                                                     //print the error

        delete_graph_search_vars(graph_vars,input_images_l,query_images_l,input_images_o,query_images_o);
        return EXIT_SUCCESS;                                                   //and exit.
    }

    if(query_images_l->get_num_of_images() != query_images_o->get_num_of_images() ||
       input_images_l->get_num_of_images() != input_images_o->get_num_of_images()){
        file_mismatch();                                                       //Check if respective files are of
                                                                               //the same size.
        delete_graph_search_vars(graph_vars,input_images_l,query_images_l,input_images_o,query_images_o);
        return EXIT_SUCCESS;                                                   //If not, exit.
    }

    algorithm   *alg_instance = new algorithm(query_images_l->get_num_of_images(),graph_vars,input_images_l);
                                                                               //Create the algorithm instance using
                                                                               //the requested algorithm.
    brute_force *BF_instance  = new brute_force(graph_vars->get_N(),query_images_l->get_num_of_images());
                                                                               //Create the brute force instance.
    while(1){
        BF_instance->find_neighbours(input_images_o,query_images_o);           //Run the brute force algorithm.
        alg_instance->algorithm_exec(input_images_l,query_images_l);           //Run the requested algorithm.

        alg_instance->output(graph_vars->get_out(),BF_instance,input_images_o,query_images_o);  //Output the results to 
                                                                               //the specified output file.
#ifndef NLOOP
        graph_vars->set_repeat(repetition_question());                         //Ask the user if they want to repeat
                                                                               //for different queries.
        if(graph_vars->get_repeat()){                                          //If the user wants to repeat,
            graph_vars->request_file("query (original dimension)");            //request the new queries and
            graph_vars->request_file("query (latent dimension)");              //request the new queries and
            
            query_images_l->update_images(graph_vars->get_ql());               //update the instances accordingly.
            query_images_o->update_images(graph_vars->get_qo());

            if(query_images_l->get_images() == NULL  ||                        //If the file path for the images          
               query_images_o->get_images() == NULL){          
                file_path_error();                                             //does not exist,
                                                                               //print the error

                delete_Algorithm(alg_instance);
                delete_brute_force(BF_instance);
                delete_graph_search_vars(graph_vars,input_images_l,query_images_l,input_images_o,query_images_o);

                return EXIT_SUCCESS;                                           //and exit.
            }

            BF_instance->update_structures(query_images_l->get_num_of_images());
            alg_instance->update_structures(query_images_l->get_num_of_images());

            if(change_output_file()){                                          //Ask the user if they want to change
                                                                               //the output file path.
                graph_vars->request_file("output");                            //If yes, request it.
            }
        }
        else{                                                                  //If the user does not want to repeat,
            break;                                                             //terminate the loop.
        }
#else
        break;
#endif
    }

    delete_Algorithm(alg_instance);
    delete_brute_force(BF_instance);
    delete_graph_search_vars(graph_vars,input_images_l,query_images_l,input_images_o,query_images_o);

    return EXIT_SUCCESS;
}