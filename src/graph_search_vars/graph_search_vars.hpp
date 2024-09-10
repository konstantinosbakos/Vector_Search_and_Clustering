#ifndef __GSVARS__
#define __GSVARS__

#include "../../src/image/image.hpp"

#define GRAPH_HELP            -3 //If the user asks for help "-h".
#define GRAPH_SoG_VARS         2 //Number of SoG variables the command line should have.
#define GRAPH_GNNS_VARS        4 //Number of GNNS variables the command line should have.
#define GRAPH_NO_CMD_IN       -1 //There is no input (except from the method).
#define GRAPH_NO_METHOD       -2 //There is no method.
#define GRAPH_ONLY_FILES       5 //There are only files (and the method).
#define GRAPH_k_E_MISMATCH     8 //k < E Error.
#define GRAPH_l_N_MISMATCH    -4 //l < N Error.
#define GRAPH_FILES_VARS_SoG   7 //There are files and SoG variables (and the method).
#define GRAPH_FILES_VARS_GNNS  9 //There are files and GNNS variables (and the method).

class graph_search_input{
    private:
        int  k;
        int  E;
        int  R;
        int  N;
        int  l;
        int  method;
        int  cmd_value; //Information about the command line input.
        bool repeat;    //True if the program will be repeated for a different input.

        std::string output_file;
        std::string input_file_l;
        std::string input_file_o;
        std::string query_file_l;
        std::string query_file_o;
        
        std::string request_for_file(std::string file);
    public:
        graph_search_input();

        void request_files();                        //Request the file paths for input, queries and output.
        void request_file(std::string type);         //Request a specific file.
        void cmd_input_graph(int argc, char **argv); //Reade the command line information.

        inline void set_repeat(bool repeat){this->repeat = repeat;};

        inline int get_k(){return this->k;};
        inline int get_E(){return this->E;};
        inline int get_R(){return this->R;};
        inline int get_N(){return this->N;};
        inline int get_l(){return this->l;};
        inline int get_method(){return this->method;};
        inline int get_cmd_value(){return this->cmd_value;};
        inline bool get_repeat(){return this->repeat;};
        inline std::string get_qo(){return this->query_file_o;};
        inline std::string get_io(){return this->input_file_o;};
        inline std::string get_ql(){return this->query_file_l;};
        inline std::string get_il(){return this->input_file_l;};
        inline std::string get_out(){return this->output_file;};
};

void delete_graph_search_vars(graph_search_input *graph_vars, image_array *input_images_l, image_array *query_images_l, image_array *input_images_o, image_array *query_images_o);

#endif