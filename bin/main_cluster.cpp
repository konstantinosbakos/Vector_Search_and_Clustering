#include "../src/CLS/CLS.hpp"
#include "../src/UI_messages/UI_messages.hpp"

int main(int argc, char **argv){
    image_array    *input_images_lat = new image_array;    //For abstraction in the main function.
    image_array    *input_images_org = new image_array;    //For abstraction in the main function.
    CLS_main_input *CLS_vars         = new CLS_main_input;

    if(CLS_vars->cmd_input_CLS(argc,argv) == false){       //If there is a mistake with the cmd
                                                           //input, exit.
        delete_CLS_vars(CLS_vars,input_images_lat,input_images_org);
        return EXIT_SUCCESS;
    }

    if(CLS_vars->get_cmd_value() == CLS_HELP){             //If the user wants the help prompt,
        cluster_help_prompt();                             //print it

        delete_CLS_vars(CLS_vars,input_images_lat,input_images_org);
        return EXIT_SUCCESS;                               //and exit.
    }

    input_images_lat->read_images(CLS_vars->get_in_lat()); //Read the input images.
    input_images_org->read_images(CLS_vars->get_in_org()); //Read the input images.

    if(input_images_lat->get_images() == NULL ||
       input_images_org->get_images() == NULL){
        file_path_error();                                 //Check if respective files are of
                                                           //the same size.

        delete_CLS_vars(CLS_vars,input_images_lat,input_images_org);
        return EXIT_SUCCESS;                               //If not, exit.
    }

    CLS *CLS_instance_l = new CLS(CLS_vars);               //Clustering in the latent dimension.
    CLS *CLS_instance_o = new CLS(CLS_vars);               //Clustering in the original dimension.

    if(CLS_instance_l->get_init_status() == false ||
       CLS_instance_o->get_init_status() == false ||
       input_images_lat->get_images()  == NULL  ||
       input_images_org->get_images()  == NULL){           //If something went wrong during
                                                           //initialisation (with .conf file),
                                                           //or the input file does not exist,
                                                           //exit.
        file_path_error_cluster();

        delete_CLS(CLS_instance_l);
        delete_CLS(CLS_instance_o);
        delete_CLS_vars(CLS_vars,input_images_lat,input_images_org);
        return EXIT_SUCCESS;
    }

    if(input_images_lat->get_num_of_images() != input_images_org->get_num_of_images()){
        file_mismatch();

        delete_CLS_vars(CLS_vars,input_images_lat,input_images_org);
        return EXIT_SUCCESS;        
    }

    if(CLS_instance_l->get_method() == CLASSIC_M){           //Execute the requested algorithm.
        CLS_instance_l->classic_algorithm(input_images_lat,input_images_org,CLS_vars->get_out());
        CLS_instance_o->classic_algorithm(input_images_org,CLS_vars->get_out());
    }
    else if(CLS_instance_l->get_method() == LSH_M){
        CLS_instance_l->lsh_approx_algorithm(input_images_lat,input_images_org,CLS_vars->get_out());
        CLS_instance_o->lsh_approx_algorithm(input_images_org,CLS_vars->get_out());
    }
    else if(CLS_instance_l->get_method() == CUBE_M){
        std::srand(time(NULL));

        CLS_instance_l->cube_approx_algorithm(input_images_lat,input_images_org,CLS_vars->get_out());
        CLS_instance_o->cube_approx_algorithm(input_images_org,CLS_vars->get_out());
    }

    delete_CLS(CLS_instance_l);
    delete_CLS(CLS_instance_o);
    delete_CLS_vars(CLS_vars,input_images_lat,input_images_org);

    return EXIT_SUCCESS;
}