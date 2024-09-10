#ifndef __LSH_VAR__
#define __LSH_VAR__

#include "../../image/image.hpp"

#define LSH_HELP       -1      //If the user asks for help "-h".
#define LSH_ONLY_VARS   4      //If the command line input contains only
                               //variables.
#define LSH_NO_CMD_IN   0      //If it contains nothing.
#define LSH_ONLY_FILES  3      //If it contains only the file paths.
#define LSH_VARS_FILES  7      //If it contains both files and variables.

class LSH_main_input{
    private:
        int         k;
        int         L;
        int         N;
        int         R;
        int         cmd_value; //Information about the command line input. 
        bool        repeat;    //True if the program will be repeated for
                               //a different input.
        std::string input_file;
        std::string query_file;
        std::string output_file;

        std::string request_for_file(std::string file); //Ask the user to input a file path.
    public:
        LSH_main_input();

        void request_files();                           //Ask the user for all file paths.
        void request_file(std::string type);            //Ask the user for a specific file path.
        void cmd_input_LSH(int argc, char **argv);      //Read the command line input.

        inline void set_repeat(bool repeat){this->repeat = repeat;};

        inline int get_k(){return this->k;};
        inline int get_L(){return this->L;};
        inline int get_N(){return this->N;};
        inline int get_R(){return this->R;};
        inline int get_cmd_value(){return this->cmd_value;};
        inline bool get_repeat(){return this->repeat;};
        inline std::string get_q(){return this->query_file;};
        inline std::string get_in(){return this->input_file;};
        inline std::string get_out(){return this->output_file;};
};

void delete_LSH_vars(LSH_main_input *LSH_vars, image_array *input_images, image_array *query_images);

#endif