#include <cstddef>

#include "skip_list.hpp"

skip_node::skip_node(int ID, int bin_value){
    this->ID            = ID;
    this->bin_value     = bin_value;
    this->level         = new std::vector<skip_node*>;
    this->num_of_levels = 1;                        //Initialize the number of levels that the skip_node is in.

    this->level->push_back(NULL);                   //Initialize next pointer.
}

skip_node::~skip_node(){
    delete this->level;
    this->level = NULL; 
}

void skip_node::level_reduction(){                      //Remove the top level that the skip_node participates in.
    if(this->num_of_levels > 1){
        this->level->pop_back();
        this->num_of_levels -= 1;
    }
}

void skip_node::level_expansion(skip_node *ptr){            //Add a new top level for the skip_node to participate in.
    this->level->push_back(ptr);
    this->num_of_levels++;
}

void skip_node::set_next_block(int _level, skip_node *ptr){ //Set the next skip_node of the specific level.
    if(_level >= this->num_of_levels){
        this->level_expansion(ptr);
    }
    else{
        (*this->level)[_level] = ptr;
    }
}

skip_node *skip_node::get_next_block(int _level){           //Get the next skip_node of the specific level.
    if(_level >= this->num_of_levels){
        this->level_expansion(NULL);
    }

    return (*this->level).at(_level);
}