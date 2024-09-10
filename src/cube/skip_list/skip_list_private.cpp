#include <cstddef>

#include "skip_list.hpp"
#include "../../math_functions/math_functions.hpp"

#define LOW_B  0
#define HIGH_B 1

int skip_list::list_length(int level){
    int    length = 0;
    skip_node *temp   = this->Sentinel->get_next_block(level);

    while(temp){
        length++;
        temp = temp->get_next_block(level);
    }

    return length;
}

void skip_list::erase_list(int level){                     //Erase the chosen level of a skip list (essentially always the top level).
    skip_node *temp      = this->Sentinel;
    skip_node *next_temp = NULL;

    while(temp){
        next_temp = temp->get_next_block(level);
        temp->level_reduction();
        temp = next_temp;
    }
}

void skip_list::generate_levels(int level){                //Creates skip list levels.
    int prev_cond_length = this->block_num;

    if(level > 0){                                        //Erase the skip list from the specified level up.
        for(int i=this->max_level; i>=level; i--){
            this->erase_list(i);
        }
    }
    else{
        level = 1;
    }

    for(; level<=this->max_level; level++){                //Process for creating new levels.
        int    condition_length       = 0;
        skip_node *lower_level_block   = this->Sentinel->get_next_block(level-1);
        skip_node *higher_level_block  = this->Sentinel;

        while(lower_level_block){
            if(coin_toss()){                               //Decide randomly if a node will be added to the level.
                higher_level_block->set_next_block(level,lower_level_block);
                higher_level_block = higher_level_block->get_next_block(level);
                higher_level_block->set_next_block(level,NULL);
                condition_length++;
            }
            lower_level_block = lower_level_block->get_next_block(level-1);
        }

        if(this->Sentinel->get_next_block(level) == NULL){ //If the level remained empty, remake it.
            this->Sentinel->level_reduction();             //Reduce the levels of Sentinel to remake level.
            level--;
            continue;
        }

        if((condition_length == prev_cond_length) || (( condition_length) < ( this->max_level - level + 1))){ //If the level is the same length as the previous, 
            this->erase_list(level);                                                                          //or the level is too small, remake it.
            level--;
        }
        else{
            prev_cond_length = condition_length;           //Set previous condition length and continue.
        }
    }
}

skip_node ***skip_list::find_entry_pos(int ID){            //Find the position in each list that the selected ID will be placed to. O(logn).
    int      levels     = this->max_level;
    skip_node   *low_bound  = this->Sentinel;
    skip_node   *high_bound = this->max_element;
    skip_node ***positions  = new skip_node **[this->max_level + 1];

    if(this->max_element->get_ID() == ID || ID < 0){
        delete [] positions;
        return NULL;
    }

    for(int i=0; i<= this->max_level; i++){                //Each level has a low and a high bound.
        positions[i] = new skip_node *[2];
    }

    for(int i=levels; i>=0; i--){
        skip_node *temp  = low_bound->get_next_block(i);

        if(this->max_element->get_ID() < ID){              //If the ID will be the max ID.
            positions[i][LOW_B]  = this->max_element;
            positions[i][HIGH_B] = NULL;
            continue;
        }

        while(temp && (temp != high_bound)){               //Fill the array that will contain the elements before
            int low_id  = low_bound->get_ID();
            int high_id = high_bound->get_ID();
            int temp_ID = temp->get_ID();                  //and after the ID that will enter the list.

            if(temp_ID <= low_id){
                temp = temp->get_next_block(i);
                continue;
            }
            else if(temp_ID >= high_id){                   //If ID is bigger or equal to current high, break loop.
                break;
            }
            else{
                if(temp_ID > ID){                          //If insert_ID is lower than the current ID, make the current ID, the high ID.
                    high_bound = temp;
                    break;
                }
                else if(temp_ID == ID){                    //If the ID already exists
                    for(int i=0; i<=this->max_level; i++){
                        positions[i][LOW_B]  = NULL;
                        positions[i][HIGH_B] = NULL;
                        delete [] positions[i];
                        positions[i]    = NULL;
                    }
                    delete [] positions;
                    positions = NULL;
                    return NULL;
                }
                else{
                    low_bound = temp;                      //If insert_ID is higher than the current ID, make the current ID, the low ID.
                }
            }
            temp = temp->get_next_block(i);
        }

        positions[i][LOW_B] = low_bound;
        if(!temp){
            positions[i][HIGH_B] = NULL;
        }
        else{
            positions[i][HIGH_B] = high_bound;
        }
    }

    return positions;
}