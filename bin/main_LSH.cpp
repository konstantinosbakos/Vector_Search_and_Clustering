#include "../src/LSH/LSH.hpp"
#include "../src/output/LSH/LSH_output.hpp"
#include "../src/brute_force/brute_force.hpp"
#include "../src/UI_messages/UI_messages.hpp"

int main(int argc, char **argv){
    image_array    *input_images = new image_array;
    image_array    *query_images = new image_array;
    LSH_main_input *LSH_vars     = new LSH_main_input;        //For abstraction in the main function.

    LSH_vars->cmd_input_LSH(argc,argv);                       //Read the command line input.

    if(LSH_vars->get_cmd_value() == LSH_HELP){                //If the user wants the help prompt,
        lsh_help_prompt();                                    //print it

        delete_LSH_vars(LSH_vars,input_images,query_images);
        return EXIT_SUCCESS;                                  //and exit.
    }

    if(LSH_vars->get_cmd_value() == LSH_NO_CMD_IN||           //If there is no command line input,
       LSH_vars->get_cmd_value() == LSH_ONLY_VARS){           //or the input was only variables,
       
       LSH_vars->request_files();                             //request the user to input the file paths.
    }

    query_images->read_images(LSH_vars->get_q());            //Read the query images.
    input_images->read_images(LSH_vars->get_in());           //Read the input images.

    if(query_images->get_images() == NULL ||                 //If the file path for the queries
       input_images->get_images() == NULL){                  //or the input file does not exist,
        file_path_error();                                   //print the error

        delete_LSH_vars(LSH_vars,input_images,query_images);
        return EXIT_SUCCESS;                                 //and exit.
    }

    LSH         *LSH_instance = new LSH(LSH_vars,query_images->get_num_of_images(),input_images); 
                                                             //Instantiate the LSH class.
    brute_force *BF_instance  = new brute_force(LSH_vars->get_N(),query_images->get_num_of_images());
                                                             //Instantiate the brute_force class.
    while(1){
        BF_instance->find_neighbours(input_images,query_images);               //Run the brute force algorithm.
        LSH_instance->kNN(query_images);                                       //Run the kNN algorithm.
        LSH_instance->Approximate(query_images);                               //Run the Approximate Range algorithm.

        LSH_output(query_images,LSH_vars->get_out(),LSH_instance,BF_instance); //Print the results to the output file.
#ifndef NLOOP
        LSH_vars->set_repeat(repetition_question());                           //Ask the user if they want to repeat 
                                                                               //for different queries. 

        if(LSH_vars->get_repeat()){                                            //If the user wants to repeat,
            LSH_vars->request_file("query");                                   //ask for the new query file,

            query_images->update_images(LSH_vars->get_q());                    //Update the objects accordingly
            
            if(query_images->get_images() == NULL){                            //If the file path for the images          
                file_path_error();                                             //does not exist,
                                                                               //print the error
                delete_lsh(LSH_instance);
                delete_brute_force(BF_instance);
                delete_LSH_vars(LSH_vars,input_images,query_images);

                return EXIT_SUCCESS;                                           //and exit.
            }

            BF_instance->update_structures(query_images->get_num_of_images());
            LSH_instance->update_structures(query_images->get_num_of_images());

            if(change_output_file()){                                          //and ask if they want to change the output file,
                                                                               //to avoid overwritting the existing one.
                LSH_vars->request_file("output");                              //If yes, save the new output file path.
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

    delete_lsh(LSH_instance);                                                //Free any allocated memory used by the program.
    delete_brute_force(BF_instance);
    delete_LSH_vars(LSH_vars,input_images,query_images);

    return EXIT_SUCCESS;
}

