#include "cluster_var.hpp"
#include "../../UI_messages/UI_messages.hpp"

CLS_main_input::CLS_main_input(){
    this->method          = "";
    this->complete        = false;
    this->conf_file       = "";
    this->output_file     = "";
    this->input_file_lat  = "";
    this->input_file_org  = "";
}

bool CLS_main_input::cmd_input_CLS(int argc, char **argv){
    int valid_comp  = 0;
    int valid_files = 0;

    if(argc > 1){
        if(std::string(argv[1]) == "-h"){
            this->cmd_value = CLS_HELP;
            return true;
        }
    }

    for(int i=0; i<2; i++){
        if((i == 1) && 
           (valid_files != CLS_ONLY_FILES && (valid_files + valid_comp) != CLS_FILES_AND_C)){
                        //This triggers both when there is no input and when the input is wrong.
                        //For example the program does not accept some file paths.
                        //All  file paths should be given for them to be counted.
            cluster_help_prompt();
            return false;
        }
        for(int j=0; j<argc; j++){
            if(std::string(argv[j]) == "-il"){
                if(i){
                    this->input_file_lat = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_files++;
                }
            }
            if(std::string(argv[j]) == "-io"){
                if(i){
                    this->input_file_org = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_files++;
                }
            }
            if(std::string(argv[j]) == "-c"){
                if(i){
                    this->conf_file = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_files++;
                }
            }
            if(std::string(argv[j]) == "-o"){
                if(i){
                    this->output_file = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_files++;
                }
            }
            if(std::string(argv[j]) == "-m"){
                if(i){
                    this->method = std::string(argv[++j]);
                }
                else if(i == 0){
                    valid_files++;
                }
            }
            if(std::string(argv[j]) == "-complete"){
                if(i){
                    this->complete = true;
                }
                else if(i == 0){
                    valid_comp++;
                }
            }
        }
    }

    if(valid_files == CLS_ONLY_FILES){ 
        this->cmd_value =  CLS_ONLY_FILES;
    }
    else if((valid_files + valid_comp) == CLS_FILES_AND_C){
        this->cmd_value = CLS_FILES_AND_C;
    }

    return true;
}

void delete_CLS_vars(CLS_main_input *CLS_vars, image_array *input_images_lat, image_array *input_images_org){
    delete CLS_vars;
    CLS_vars = NULL;

    delete input_images_lat;
    input_images_lat = NULL;

    delete input_images_org;
    input_images_org = NULL;
}