#ifndef __CVAR__
#define __CVAR__

#define CUBE_HELP       -1
#define CUBE_ONLY_VARS   5     //If the command line input contains only
                               //variables.
#define CUBE_NO_CMD_IN   0     //If it contains nothing.
#define CUBE_ONLY_FILES  3     //If it contains only the file paths.
#define CUBE_VARS_FILES  8     //If it contains both files and variables.

#include "../../image/image.hpp"

class cube_main_input{
    private:
        int         k;
        int         M;
        int         N;
        int         R;
        int         probes;
        int         cmd_value; //Information about the command line input. 
        bool        repeat;    //True if the program will be repeated for
                               //a different input.
        std::string input_file;
        std::string query_file;
        std::string output_file;

        std::string request_for_file(std::string file); //Ask the user to input a file path.
    public:
        cube_main_input();

        void request_files();                           //Ask the user for all file paths.
        void request_file(std::string type);            //Ask the user for a specific file path.
        void cmd_input_cube(int argc, char **argv);     //Read the command line input.

        inline void set_repeat(bool repeat){this->repeat = repeat;};

        inline int get_k(){return this->k;};
        inline int get_M(){return this->M;};
        inline int get_N(){return this->N;};
        inline int get_R(){return this->R;};
        inline int get_probes(){return this->probes;};
        inline int get_cmd_value(){return this->cmd_value;};
        inline bool get_repeat(){return this->repeat;};
        inline std::string get_q(){return this->query_file;};
        inline std::string get_in(){return this->input_file;};
        inline std::string get_out(){return this->output_file;};
};

void delete_cube_vars(cube_main_input *cube_vars, image_array *input_images, image_array *query_images);

#endif