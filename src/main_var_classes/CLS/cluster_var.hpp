#ifndef __CLS_VAR__
#define __CLS_VAR__

#include "../../image/image.hpp"

#define CLS_HELP        -1 //If the user wants the help prompt.
#define CLS_ONLY_FILES   5 //If it contains only the file paths.
#define CLS_FILES_AND_C  6 //If it contains both files and "complete" value.

class CLS_main_input{
    private:
        int         cmd_value;
        bool        complete;
        std::string method;
        std::string conf_file;
        std::string input_file_org;
        std::string input_file_lat;
        std::string output_file;
    public:
        CLS_main_input();

        bool cmd_input_CLS(int argc, char **argv);

        inline int get_cmd_value(){return this->cmd_value;};
        inline bool get_complete(){return this->complete;};
        inline std::string get_method(){return this->method;};
        inline std::string get_conf(){return this->conf_file;};
        inline std::string get_out(){return this->output_file;};
        inline std::string get_in_org(){return this->input_file_org;};
        inline std::string get_in_lat(){return this->input_file_lat;};
};

void delete_CLS_vars(CLS_main_input *CLS_vars, image_array *input_images_lat, image_array *input_images_org);

#endif
