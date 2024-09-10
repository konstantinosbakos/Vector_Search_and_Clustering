#include <iostream>

#include "graph_search_vars.hpp"

//~Private~//

std::string graph_search_input::request_for_file(std::string file){
    std::string temp = "";

    while(1){
        std::cout << "> Please provide the " + file + " file full path: ";
        std::cin  >> temp;

        if(temp != ""){
            return temp;
        }
    }
}

//~Public~//

graph_search_input::graph_search_input(){ //Default values.
    this->k            = 50;
    this->E            = 30;
    this->R            = 01;
    this->N            = 01;
    this->l            = 20;
    this->method       = -1;
    this->output_file  = "";
    this->input_file_l = "";
    this->input_file_o = "";
    this->query_file_l = "";
    this->query_file_o = "";
}

void graph_search_input::request_files(){
    this->output_file   = this->request_for_file("output");
    this->query_file_o  = this->request_for_file("query (original dimension)");
    this->query_file_l  = this->request_for_file("query (latent dimension)");
    this->input_file_o  = this->request_for_file("input (original dimension)");
    this->input_file_l  = this->request_for_file("input (latent dimension)");
}

void graph_search_input::request_file(std::string type){
    if(type == "query (latent dimension)"){
        this->query_file_l  = this->request_for_file(type);
    }
    else if(type == "input (latent dimension)"){
        this->input_file_l  = this->request_for_file(type);
    }
    if(type == "query (original dimension)"){
        this->query_file_o  = this->request_for_file(type);
    }
    else if(type == "input (original dimension)"){
        this->input_file_o  = this->request_for_file(type);
    }
    else if(type == "output"){
        this->output_file = this->request_for_file(type);
    }
}

void graph_search_input::cmd_input_graph(int argc, char **argv){
    int valid_file     = 0; //Counts file path entries.
    int valid_method   = 0; //Check if method is given.
    int valid_sog_var  = 0; //Counts SoG variables.
    int valid_gnns_var = 0; //Counts GNNS variables.

    if(argc > 1){
        if(std::string(argv[1]) == "-h"){
            this->cmd_value = GRAPH_HELP;
            return;
        }
    }

    for(int i=0; i<2; i++){
        if((i == 1) && (valid_method == 0 || 
            (((2 * (valid_method + valid_file + valid_sog_var)  + 1) != argc) &&
             ((2 * (valid_method + valid_file + valid_gnns_var) + 1) != argc)))){
            this->cmd_value = GRAPH_NO_METHOD;
            return;
        }
        else if((i == 1) &&  //This triggers both when there is no input and when the input is wrong.
                             //For example the program does not accept some variables or some file paths.
                             //All variables or paths should be given for them to be counted.
            ( valid_file                   != GRAPH_ONLY_FILES &&
              valid_sog_var                != GRAPH_SoG_VARS &&
              valid_gnns_var               != GRAPH_GNNS_VARS &&
             (valid_file + valid_sog_var)  != GRAPH_FILES_VARS_SoG &&
             (valid_file + valid_gnns_var) != GRAPH_FILES_VARS_GNNS)){

            this->cmd_value = GRAPH_NO_CMD_IN;
            return;
        }
        for(int j=0; j<argc; j++){
            if(std::string(argv[j]) == "-dl"){
                if(i){
                    this->input_file_l = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-ql"){
                if(i){
                    this->query_file_l = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-do"){
                if(i){
                    this->input_file_o = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-qo"){
                if(i){
                    this->query_file_o = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-o"){
                if(i){
                    this->output_file = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-k"){
                if(i && (valid_gnns_var == GRAPH_GNNS_VARS)){
                    this->k = atoi(argv[++j]);
                }
                else if(i == 0){
                    valid_gnns_var++;
                }
            }
            if(std::string(argv[j]) == "-E"){
                if(i && (valid_gnns_var == GRAPH_GNNS_VARS)){
                    this->E = atoi(argv[++j]);
                }
                else if(i == 0){
                    valid_gnns_var++;
                }
            }
            if(std::string(argv[j]) == "-R"){
                if(i && (valid_gnns_var == GRAPH_GNNS_VARS)){
                    this->R = atoi(argv[++j]);
                }
                else if(i == 0){
                    valid_gnns_var++;
                }
            }
            if(std::string(argv[j]) == "-N"){
                if(i && ((valid_sog_var == GRAPH_SoG_VARS) || (valid_gnns_var == GRAPH_GNNS_VARS))){
                    this->N = atoi(argv[++j]);
                }
                else if(i == 0){
                    valid_sog_var++;
                    valid_gnns_var++;
                }
            }
            if(std::string(argv[j]) == "-l"){
                if(i && (valid_sog_var == GRAPH_SoG_VARS)){
                    this->l = atoi(argv[++j]);
                }
                else if(i == 0){
                    valid_sog_var++;
                }
            }
            if(std::string(argv[j]) == "-m"){
                if(i){
                    this->method = atoi(argv[++j]);
                }
                else if(i == 0){
                    j++;

                    if(j<argc){
                        if(atoi(argv[j]) == 1 || atoi(argv[j]) == 2){
                            valid_method++;
                        }
                    }
                }
            }
        }
    }

    if(this->k < this->E){
        this->cmd_value = GRAPH_k_E_MISMATCH;

        return;
    }

    if(this->l < this->N){
        this->cmd_value = GRAPH_l_N_MISMATCH;

        return;
    }

    if(valid_file == GRAPH_ONLY_FILES){
        this->cmd_value = GRAPH_ONLY_FILES;
    }
    else if(valid_sog_var == GRAPH_SoG_VARS){
        this->cmd_value = GRAPH_SoG_VARS;
    }
    else if(valid_gnns_var == GRAPH_GNNS_VARS){
        this->cmd_value = GRAPH_GNNS_VARS;
    }
    else if((valid_file + valid_sog_var)  == GRAPH_FILES_VARS_SoG){
        this->cmd_value = GRAPH_ONLY_FILES;
    }
    else if((valid_file + valid_gnns_var) == GRAPH_FILES_VARS_GNNS){
        this->cmd_value = GRAPH_ONLY_FILES;
    }
}

//~Non-Class~//

void delete_graph_search_vars(graph_search_input *graph_vars, image_array *input_images_l, image_array *query_images_l, image_array *input_images_o, image_array *query_images_o){
    delete graph_vars;
    graph_vars = NULL;

    delete input_images_l;
    input_images_l = NULL;

    delete query_images_l;
    query_images_l = NULL;

    delete input_images_o;
    input_images_o = NULL;

    delete query_images_o;
    query_images_o = NULL;
}