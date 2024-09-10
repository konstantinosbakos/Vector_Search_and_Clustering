#include <iostream>

#include "UI_messages.hpp"

bool change_output_file(){
    std::string input = "";

    std::cout << " __________________________________ " << std::endl;
    std::cout << "|> Would you like to:              |" << std::endl;
    std::cout << "|  change the output file      (1) |" << std::endl;
    std::cout << "|  or rewrite the previous one (2)?|" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
    std::cout << " >[1/2]: ";
    std::cin  >> input;

    if(input == "1"){
        return true;
    }
    else{
        return false;
    }
}

bool repetition_question(){
    std::string input = "";

    std::cout << " ___________________________________ " << std::endl;
    std::cout << "|> Would you like to:               |" << std::endl;
    std::cout << "|  repeat for a different query (1) |" << std::endl;
    std::cout << "|  or terminate the program     (2)?|" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << " >[1/2]: ";
    std::cin  >> input;

    if(input == "1"){
        return true;
    }
    else{
        return false;
    }
}

void error_method(){
    std::cout << " ____________________________________________________ " << std::endl;
    std::cout << "|>Please provide the method with a correct value     |" << std::endl;
    std::cout << "| when calling the program.                          |" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
}

void file_mismatch(){
    std::cout << " _________________________________ " << std::endl;
    std::cout << "|>The file sizes must be the same.|" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
}

void file_path_error(){
    std::cout << " ____________________________________ " << std::endl;
    std::cout << "|>Query or Input file does not exist.|" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
}

void lsh_help_prompt(){
    std::cout << " ________________________________________________________ " << std::endl;
    std::cout << "|>To correctly call the LSH executable type as following:|" << std::endl;
    std::cout << "| ./lsh -d <input file> -q <query file> -k <int> -L <int>|" << std::endl;
    std::cout << "| -o <output file> -N <number of nearest> -R <radius>    |" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
}

void SoG_help_prompt(){
    std::cout << " __________________________________________________ " << std::endl;
    std::cout << "|>To correctly call the graph_search executable    |" << std::endl;
    std::cout << "| for SoG, type as following:                      |" << std::endl;
    std::cout << "| ./graph_search -dl <input latent file>           |" << std::endl;
    std::cout << "| -ql <query latent file> -do <input original file>|" << std::endl;
    std::cout << "| -qo <query original file> -N <int> -l <int>      |" << std::endl;
    std::cout << "| -m <2 for SoG> -o <output file>                  |" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;

}

void cube_help_prompt(){
    std::cout << " _________________________________________________________ " << std::endl;
    std::cout << "|>To correctly call the Cube executable type as following:|" << std::endl;
    std::cout << "| ./cube -d <input file> -q <query file> -k <int> -M <int>|" << std::endl;
    std::cout << "| -probes <int> -o <output file> -N <number of nearest>   |" << std::endl;
    std::cout << "| -R <radius>                                             |" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
}

void GNNS_help_prompt(){
    std::cout << " ____________________________________________________ " << std::endl;
    std::cout << "|>To correctly call the graph_search executable      |" << std::endl;
    std::cout << "| for GNNS, type as following:                       |" << std::endl;
    std::cout << "| ./graph_search -dl <input latent file>             |" << std::endl;
    std::cout << "| -ql <query latent file> -do <input original file>  |" << std::endl;
    std::cout << "| -qo <query original file> -k <int> -E <int>        |" << std::endl;
    std::cout << "| -R <int> -N <int> -m <1 for GNNS>  -o <output file>|" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;

}

void error_k_E_mismatch(){
    std::cout << " ____________________________________________ " << std::endl;
    std::cout << "|>Please provide the correct values (k >= E).|" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
}

void error_l_N_mismatch(){
    std::cout << " ____________________________________________ " << std::endl;
    std::cout << "|>Please provide the correct values (l >= N).|" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
}

void cluster_help_prompt(){
    std::cout << " ____________________________________________________________ " << std::endl;
    std::cout << "|>To correctly call the Cluster executable type as following:|" << std::endl;
    std::cout << "| ./cluster -il <input latent file> -c <configuration file>  |" << std::endl;
    std::cout << "| -io <input original file>                                  |" << std::endl;
    std::cout << "| -o <output file> -complete <optional>                      |" << std::endl;
    std::cout << "| -m <method: Classic OR LSH or Hypercube>                   |" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;

}

void file_path_error_cluster(){
    std::cout << " ____________________________________________ " << std::endl;
    std::cout << "|>Configuration or Input file does not exist.|" << std::endl;
    std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
}
