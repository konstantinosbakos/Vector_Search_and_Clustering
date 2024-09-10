#ifndef __SKIP_LIST__
#define __SKIP_LIST__

#include <vector>

#define LOW_B           0
#define HIGH_B          1
#define SENTINEL_VALUE -1

class skip_node{
    private:
        int                  ID;            //The result of the h() function.
        int                  bin_value;     //The value given to the specific result,
                                            //value belongs to {0,1}.
        int                  num_of_levels; //Number of levels this skip_node participates in the skip list.
        std::vector<skip_node*> *level;
    public:
        skip_node(int ID, int bin_value);
        ~skip_node();

        void level_reduction();             //Reduce the array of levels.
        void level_expansion(skip_node *ptr);   //Expand the array of levels.
        void set_next_block(int _level, skip_node *ptr);

        skip_node *get_next_block(int _level);

        inline int get_ID(){return this->ID;};
        inline int get_bin_value(){return this->bin_value;};
};

class skip_list{
    private:
        int    max_level;
        int    block_num;
        skip_node *Sentinel;
        skip_node *max_element;

        int list_length(int level);

        void erase_list(int level);         //Erase chosen skip list level.
        void generate_levels(int level=-1); //Generates or re-generates the levels of the skip list.

        skip_node ***find_entry_pos(int ID);//Create an array of pointers for the insertion of a new 
                                            //skip_node to the right position in the skip list levels.
    public:
        skip_list();
        ~skip_list();

        void insert(int ID, int bin_value); //Insert in O(logn) time (used in user commands).

        skip_node *search(int ID);          //Search in O(logn) time.
};

#endif