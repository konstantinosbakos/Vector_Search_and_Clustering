#include <cmath>
#include <iostream>

#include "LSH_var.hpp"

std::string LSH_main_input::request_for_file(std::string file){
    std::string temp = "";

    while(1){
        std::cout << "> Please provide the " + file + " file full path: ";
        std::cin  >> temp;

        if(temp != ""){
            return temp;
        }
    }
}

LSH_main_input::LSH_main_input(){ //Set the values to their default
    this->k           = 4;        //If the user inputs other values, they will update.
    this->L           = 1;
    this->N           = 5;
    this->R           = 10000;
    this->repeat      = true;
    this->input_file  = "";
    this->query_file  = "";
    this->output_file = "";
}

void LSH_main_input::request_files(){
    this->query_file  = this->request_for_file("query");
    this->input_file  = this->request_for_file("input");
    this->output_file = this->request_for_file("output");
}

void LSH_main_input::request_file(std::string type){
    if(type == "query"){
        this->query_file  = this->request_for_file(type);
    }
    else if(type == "input"){
        this->input_file  = this->request_for_file(type);
    }
    else if(type == "output"){
        this->output_file = this->request_for_file(type);
    }
}

void LSH_main_input::cmd_input_LSH(int argc, char **argv){
    int valid_all  = 0; //Counts all entries.
    int valid_var  = 0; //Counts variable entries.
    int valid_file = 0; //Counts file path entries.

    if(argc > 1){
        if(std::string(argv[1]) == "-h"){
            this->cmd_value = LSH_HELP;
            return;
        }
    }

    for(int i=0; i<2; i++){
        if((i == 1) &&  //This triggers both when there is no input and when the input is wrong.
                        //For example the program does not accept some variables or some file paths.
                        //All variables or file paths should be given for them to be counted.
           (valid_all != LSH_VARS_FILES &&
           valid_file != LSH_ONLY_FILES &&
           valid_var  != LSH_ONLY_VARS)){

            this->cmd_value = LSH_NO_CMD_IN;
            return;
        }

        for(int j=0; j<argc; j++){
            if(std::string(argv[j]) == "-d"){
                if(i){
                    this->input_file = std::string(argv[++j]);
                }
                else{
                    valid_all++;
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-q"){
                if(i){
                    this->query_file = std::string(argv[++j]);
                }
                else{
                    valid_all++;
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-o"){
                if(i){
                    this->output_file = std::string(argv[++j]);
                }
                else{
                    valid_all++;
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-k"){
                if(i){
                    this->k = atoi(argv[++j]);
                }
                else{
                    valid_all++;
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-L"){
                if(i){
                    this->L = atoi(argv[++j]);
                }
                else{
                    valid_all++;
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-N"){
                if(i){
                    this->N = atoi(argv[++j]);
                }
                else{
                    valid_all++;
                    valid_file++;
                }
            }
            if(std::string(argv[j]) == "-R"){
                if(i){
                    this->R = atoi(argv[++j]);
                }
                else{
                    valid_all++;
                    valid_file++;
                }
            }
        }
    }

    if(valid_all == LSH_VARS_FILES){ 
        this->cmd_value =  valid_all;
    }
    else if(valid_file == LSH_ONLY_FILES){
        this->cmd_value = valid_file;
    }
    else if(valid_var  == LSH_ONLY_VARS){
        this->cmd_value = valid_var;
    }
}

void delete_LSH_vars(LSH_main_input *LSH_vars, image_array *input_images, image_array *query_images){
    delete LSH_vars;
    LSH_vars = NULL;

    delete input_images;
    input_images = NULL;
 
    delete query_images;
    query_images = NULL;
}
