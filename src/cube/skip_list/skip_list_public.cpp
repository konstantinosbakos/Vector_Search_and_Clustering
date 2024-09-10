#include <cmath>

#include "skip_list.hpp"
#include "../../math_functions/math_functions.hpp"

skip_list::skip_list(){
    this->Sentinel    = new skip_node(SENTINEL_VALUE,SENTINEL_VALUE); //Initialize sentinel (Dummy Node).
    this->max_level   = 0;
    this->block_num   = 0;
    this->max_element = this->Sentinel;                               //The max element is the Sentinel (When the list is empty).
}

skip_list::~skip_list(){
    skip_node *temp = this->Sentinel;
    skip_node *next_temp;

    this->max_element = NULL;

    while(temp){
        next_temp = temp->get_next_block(0);
        delete temp;
        temp      = next_temp;
    }
}

void skip_list::insert(int ID, int bin_value){
    int      new_block_level = 0;
    skip_node   *new_block       = new skip_node(ID,bin_value);
    skip_node ***positions       = this->find_entry_pos(ID);   //An array with the positions between which
                                                                 //the ID may enter.
    if(!positions){
        delete new_block;
        new_block = NULL;
        return;
    }

    for(int i=this->max_level; i>=0; i--){                       //Choose randomly how many levels will the ID be added to.
        if(coin_toss()){
            new_block_level = i;
            break;
        }
    }    

    for(int i=0; i<=this->max_level; i++){                        //Add the ID to those levels.
        if(new_block_level >= i){
            positions[i][LOW_B]->set_next_block(i,new_block);
            new_block->set_next_block(i,positions[i][HIGH_B]);
        }
    }

    for(int i=0; i<=this->max_level; i++){                        //Free the array with the positions.
        positions[i][LOW_B] = NULL;
        positions[i][HIGH_B] = NULL;
        delete [] positions[i];
        positions[LOW_B]    = NULL;
    }

    delete [] positions;
    positions = NULL;

    if(this->max_element->get_ID() < new_block->get_ID()){        //If new_ID > max_ID set max element.
        this->max_element = new_block;
    }

    this->block_num++;

    if(this->max_level < int(ceil(log2(this->block_num)))){       //If a level can be added, regenerate levels.
        this->max_level = int(ceil(log2(this->block_num)));
        this->generate_levels();
    }
}

skip_node *skip_list::search(int ID){
    int    levels     = this->max_level;
    skip_node *low_bound  = this->Sentinel;
    skip_node *high_bound = this->max_element;

    for(int i=levels; i>=0; i--){
        skip_node *temp  = low_bound->get_next_block(i);

        while(temp){
            int temp_ID = temp->get_ID();
            int high_id = high_bound->get_ID();

            if(temp_ID  > high_id){                             //If temp ID is bigger than the high bound, break and go down a level.
                break;
            }
            else{
                if(temp_ID == ID){                              //If skip_node is found, return it.
                    return temp;
                }
                else if(temp_ID > ID){                          //If temp ID is higher than the ID being searched, and temp_ID < high_ID, then high_ID = temp_ID.
                    high_bound  = temp;
                    break;
                }
                else{                                           //Else set new low ID.
                    low_bound = temp;
                }
            }
            temp = temp->get_next_block(i);
        }
    }

    return NULL;                                                //If ID not found return NULL.
}
